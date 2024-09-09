#include "bits.h"
#include "drivers/virtio/virtio.h"
#include "drivers/virtio/virtio_internal.h"
#include "drivers/mmu/mmu.h"

static bool virtio_console_check_features(struct VirtioMMIO *d, uint32_t desired_features[4]);
static struct VirtioDevice* virtio_allocate_legacy_queues(struct VirtioMMIO *d, uint32_t n_queues);
static void virtio_reset(struct VirtioMMIO *d);
static void virtio_console_init_queues(struct VirtioDevice *d);

VirtioDevice virtio_init(uint64_t device_base, enum VIRTIO_DEVICE device_type, uint32_t desired_features[4], enum VIRTIO_INIT_STATUS* err) {
  enum VIRTIO_INIT_STATUS s = VIRTIO_INIT_SUCCESS;
  struct VirtioMMIO *d = (void*)device_base;
  if (d->magic != VIRTIO_MAGIC)  {
    s = VIRTIO_INIT_ERROR_INCORRECT_MAGIC;
    goto init_err;
  }

  if(d->version != 1) {
    s = VIRTIO_INIT_ERROR_INCORRECT_VERSION;
    goto init_err;
  }

  if (d->device_id == 0) {
    s = VIRTIO_INIT_ERROR_UNKNOWN_DEIVCE;
    goto init_err;
  }

  virtio_reset(d);
  d->device_status = VIRTIO_STATUS_ACKNOWLEDGE;
  d->device_status |= VIRTIO_STATUS_DRIVER;

  uint32_t n_queues = 0;
  bool features_supported = false;
  switch (device_type) {
    case VIRTIO_CONSOLE:
      features_supported = virtio_console_check_features(d, desired_features);
      n_queues = 2;
      break;
    default:
      s = VIRTIO_INIT_ERROR_UNKNOWN_DEIVCE;
      goto init_err;
  }

  if (!features_supported) {
    d->device_status = VIRTIO_STATUS_FAILED;
    s = VIRTIO_INIT_ERROR_FEATURES_UNSUPPORTED;
    goto init_err;
  }

  // write provided features
  for (uint8_t i = 0; i < 4; i++) {
    d->guest_features_selector = i;
    d->guest_features = desired_features[i];
  }

  struct VirtioDevice *device = virtio_allocate_legacy_queues(d, n_queues);
  if (device->n_queues == 0) {
    d->device_status = VIRTIO_STATUS_FAILED;
    s = VIRTIO_INIT_ERROR_FAILED_TO_INIT_QUEUES;
    free(device);
    goto init_err;
  }

  device->type = device_type;
  device->regs->device_status |= VIRTIO_STATUS_DRIVER_OK;
  return device;

init_err:
  if (err) {
    *err = s;
  }

  return NULL;
}

static struct VirtioDevice* virtio_allocate_legacy_queues(struct VirtioMMIO *d, uint32_t n_queues) {
  uint64_t size = n_queues * sizeof(struct VirtQueue) + sizeof(struct VirtioDevice);
  struct VirtioDevice *device = malloc(size_to_pages(size));

  uint32_t n_allocated_queues = 0;
  for (uint32_t i = 0; i < n_queues; i++) {
    d->queue_selector = i;
    uint32_t queue_num = d->queue_num_max;
    if (queue_num == 0) {
      continue;
    }

    n_allocated_queues++;
    size = ALIGN(sizeof(struct VirtQueueDesc) * queue_num + sizeof(struct VirtQueueAvail) + sizeof(uint16_t) * queue_num, PAGE_SIZE) 
      + sizeof(struct VirtQueueUsed) + sizeof(struct VirtQueueUsedElem) * queue_num;
    size = size_to_pages(size);

    void *q = malloc(size);

    device->queues[i].desc = q;
    device->queues[i].avail = q + sizeof(struct VirtQueueDesc) * queue_num;
    device->queues[i].used = ALIGN_ADDR(device->queues[i].avail + sizeof(struct VirtQueueAvail) + sizeof(uint16_t) * queue_num, PAGE_SIZE);
    device->queues[i].size = size * PAGE_SIZE;

    d->queue_num = queue_num;
    d->queue_align = PAGE_SIZE;
    d->queue_pfn = (uint64_t)q;
  }

  device->regs = d;
  device->n_queues = n_allocated_queues;
  return device;
}

static void virtio_reset(struct VirtioMMIO *d) {
  d->device_status = 0x0;
}

static bool virtio_console_check_features(struct VirtioMMIO *d, uint32_t desired_features[4]) {
  for (uint8_t i = 0; i < 4; i++) {
    d->host_featurse_selector = i;
    if ((d->host_features & desired_features[i]) != desired_features[i])
        return false;
  }
  return true;
}

void virtio_console_write(VirtioDevice console, uint8_t *msg) {
  struct VirtioDevice *d = console;
  d->regs->queue_selector = 1;
  d->queues[1].desc[0] = (struct VirtQueueDesc) {.addr = (uint64_t)msg, .len = 7};
  d->queues[1].avail->ring[0] = 0;
  __asm__ volatile("dsb SY");
  d->queues[1].avail->idx = 1;
  __asm__ volatile("dsb SY");
  d->regs->queue_notify = 1;
}

void virtio_console_emerge_write(VirtioDevice console, uint8_t c) {
  ((struct VirtioDevice*)console)->regs->configuration_space[2] = c;
}
