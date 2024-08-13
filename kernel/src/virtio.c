#include "drivers/virtio/virtio.h"
#include "drivers/virtio/virtio_internal.h"
#include "mem.h"

static bool virtio_check_features(u64 device_features, u64 desired_features);

VirtioDevice virtio(u64 base, u64 desired_features) {
  VirtioDevice d = block_alloc(1);
  if (!d) {
    return NULL;
  }

  d->registers = (void *)base;
  d->n_queues = 0;
  d->max_n_queues = (BLOCK_SIZE - sizeof(*d)) / sizeof(*d->queues);

  if (d->registers->magic != VIRTIO_MAGIC || d->registers->version != VIRTIO_VERSION) {
    goto error;
  }
  u8 id = d->registers->device_id;
  if (id == 0) {
    goto error;
  }

  virtio_reset(d);
  d->registers->status |= VIRTIO_STATUS_ACKNOWLEDGE;
  d->registers->status |= VIRTIO_STATUS_DRIVER;

  // setup features
  u64 features;
  // bits 0-31
  d->registers->device_features_selector = 0;
  features = d->registers->device_features;

  // bits 32-63
  d->registers->device_features_selector = 1;
  features |= (u64)d->registers->device_features << 32;

  if (!virtio_check_features(features, desired_features)) {
    d->registers->status |= VIRTIO_STATUS_FAILED;
    goto error;
  }

  // bits 0-31
  d->registers->driver_features_selector = 0;
  d->registers->driver_features = features & (BIT(32) - 1);

  // bits 32-63
  d->registers->driver_features_selector = 1;
  d->registers->driver_features = features >> 32;

  d->registers->status |= VIRTIO_STATUS_FEATURES_OK;
  // device does not support our subset of features and the device is unusable
  if ((d->registers->status & VIRTIO_STATUS_FEATURES_OK) == 0) {
    d->registers->status |= VIRTIO_STATUS_FAILED;
    goto error;
  }

  // device initialization
  u64 i = 0;
  bool device_initialized = false;
  while (i < sizeof(DEVICES) / sizeof(*DEVICES)) {
    if (DEVICES[i].id == id) {
      device_initialized = DEVICES[i].init(d, features);
      break;
    }
    i++;
  }

  if (!device_initialized) {
    d->registers->status |= VIRTIO_STATUS_FAILED;
    goto error;
  }

  d->registers->status |= VIRTIO_STATUS_DRIVER_OK;
  return d;

error:
  block_free(d, 1);
  return NULL;
}

void virtio_reset(VirtioDevice device) {
  device->registers->status = 0; // Writing zero (0x0) to this register triggers a device reset
  while (device->registers->status != 0)
    ; // wait reset completion
}

static bool virtio_check_features(u64 device_features, u64 desired_features) {
  for (u8 i = 0; i < sizeof(device_features) * 8; i++) {
    // desired feature not supported by device
    if ((device_features & BIT(i)) == 0 && (desired_features & BIT(i)) != 0) {
      return false;
    }
  }

  return true;
}

static u64 get_queue_descriptor_size(u32 queue_len) { return queue_len * sizeof(struct VirtQueueDesc); }

static u64 get_queue_avail_size(u32 queue_len) { return sizeof(struct VirtQueueAvail) + sizeof(u16) * (queue_len + 1); }

static u64 get_queue_used_size(u32 queue_len) {
  return sizeof(struct VirtQueueUsed) + sizeof(struct VirtQueueUsedElem) * queue_len + sizeof(u16);
}

static u64 get_queue_size(u32 queue_len) {
  return get_queue_descriptor_size(queue_len) + get_queue_avail_size(queue_len) + get_queue_used_size(queue_len);
}

/*
 * General queue alloc.
 * Allocate at most `n_queues` with sizeof `get_queue_size(device->register->queue_num_max)` each.
*/
static u32 virtio_queues_allocate(VirtioDevice device, u32 n_queues) {
  device->registers->queue_selector = 0;

  u32 n_allocated = 0;
  u32 n_elements = device->registers->queue_num_max;
  u64 queue_size = get_queue_size(n_elements);

  bool err = false;
  while (n_allocated < n_queues && n_elements != 0) {
    device->queues[n_allocated].n_elements = n_elements;
    device->queues[n_allocated].q = block_alloc(queue_size / BLOCK_SIZE);
    if (!device->queues[n_allocated].q) {
      err = true;
      break;
    }
    memset(device->queues[n_allocated].q, 0, queue_size);
    device->registers->queue_num = n_elements;

    n_allocated++;
    device->registers->queue_selector = n_allocated;
    n_elements = device->registers->queue_num_max;
    queue_size = get_queue_size(n_elements);
  }

  if (err) {
    for (u32 i = 0; i < n_allocated; i++) {
      block_free(device->queues[i].q, get_queue_size(device->registers->queue_num_max) / BLOCK_SIZE);
    }
    return 0;
  }

  device->n_queues = n_allocated;
  return n_allocated;
}

/* Writes queues to device and marks them as `ready` */
static void virtio_queues_write(VirtioDevice device) {
  u64 queue_avail_offset, queue_used_offset;
  for (u32 i = 0; i < device->n_queues; i++) {
    device->registers->queue_selector = i;
    device->registers->queue_num = device->queues[i].n_elements;

    device->registers->queue_desc_low = (u64)device->queues[i].q & (BIT(32) - 1);
    device->registers->queue_desc_high = (u64)device->queues[i].q >> 32;

    queue_avail_offset = get_queue_descriptor_size(device->queues[i].n_elements);
    queue_used_offset = queue_avail_offset + get_queue_avail_size(device->queues[i].n_elements);

    device->registers->queue_driver_low = (u64)(device->queues[i].q + queue_avail_offset) & (BIT(32) - 1);
    device->registers->queue_driver_high = (u64)(device->queues[i].q + queue_avail_offset) >> 32;

    device->registers->queue_device_low = (u64)(device->queues[i].q + queue_used_offset) & (BIT(32) - 1);
    device->registers->queue_device_high = (u64)(device->queues[i].q + queue_used_offset) >> 32;

    device->registers->queue_ready = 1;
  }
}

bool virtio_console_init(VirtioDevice console, u64 features) {
  u32 n_queues = 2; // transmit & receive
  if (features & VIRTIO_CONSOLE_F_MULTIPORT) {
    u32 before, after;
    do {
      before = console->registers->config_generation;
      n_queues = console->registers->config[1]; // read max_nr_ports
      after = console->registers->config_generation;
    } while (after != before);
  }

  n_queues = virtio_queues_allocate(console, n_queues);
  if (n_queues == 0) {
    return false;
  }

  // The driver MUST NOT put a device-readable buffer in a receiveq.
  // The driver MUST NOT put a device-writable buffer in a transmitq.
  for (u32 i = 0; i < n_queues; i += 2) {
    for (u32 j = 0; j < console->queues[i].n_elements; j++) {
      ((struct VirtQueueDesc *)console->queues[i].q)[j].flags = VIRTQ_DESC_F_WRITE;
    }
  }

  virtio_queues_write(console);
  return true;
}

void virtio_console_emerge_write(VirtioDevice console, u32 c) {
  console->registers->config[2] = c;
}

void virtio_console_write(VirtioDevice console, u8 *buffer, u16 n) {
  (void)console;
  (void)buffer;
  (void)n;
}

void virtio_clear_interrupt(VirtioDevice device) { (void)device; }

u16 virtio_console_read(VirtioDevice console, u8 **buffer, u16 size) {
  (void)console;
  (void)buffer;
  (void)size;
  return 0;
}
