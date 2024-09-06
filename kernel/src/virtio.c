#include "bits.h"
#include "drivers/virtio/virtio.h"
#include "drivers/virtio/virtio_internal.h"
#include "drivers/mmu/mmu.h"

static bool virtio_console_check_features(struct VirtioMMIO *d, uint32_t desired_features[4]);
static struct VirtioDevice* virtio_init_legacy_queues(struct VirtioMMIO *d, uint32_t n_queues);
static void virtio_reset(struct VirtioMMIO *d);

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
  d->device_status[0] = VIRTIO_STATUS_ACKNOWLEDGE;
  d->device_status[0] |= VIRTIO_STATUS_DRIVER;

  bool features_supported = false;
  switch (device_type) {
    case VIRTIO_CONSOLE:
      features_supported = virtio_console_check_features(d, desired_features);
      break;
    default:
      s = VIRTIO_INIT_ERROR_UNKNOWN_DEIVCE;
      goto init_err;
  }

  if (!features_supported) {
    d->device_status[4] = VIRTIO_STATUS_FAILED;
    s = VIRTIO_INIT_ERROR_FEATURES_UNSUPPORTED;
    goto init_err;
  }

  // write provided features
  for (uint8_t i = 0; i < 4; i++) {
    d->guest_features_selector = i;
    d->guest_features = desired_features[i];
  }

  struct VirtioDevice *device = virtio_init_legacy_queues(d, 2);
  device->regs->device_status[0] |= VIRTIO_STATUS_DRIVER_OK;
  return device;

init_err:
  if (err) {
    *err = s;
  }

  return NULL;
}

static struct VirtioDevice* virtio_init_legacy_queues(struct VirtioMMIO *d, uint32_t n_queues) {
  uint64_t size = n_queues * sizeof(struct VirtQueue) + sizeof(struct VirtioDevice);
  struct VirtioDevice *device = malloc(size / PAGE_SIZE + (bool)(size % PAGE_SIZE));

  device->n_queues = n_queues;
  for (uint32_t i = 0; i < n_queues; i++) {
    d->queue_selector = i;
    uint32_t queue_num = d->queue_num_max;

    size = sizeof(struct VirtQueueDesc) * queue_num + sizeof(struct VirtQueueAvail) + 2 * queue_num + sizeof(struct VirtQueueUsed) + 8 * queue_num;
//    void *q = malloc();
//    device->queues[i].desc = q;
//    device->queues[i].avail = q + ;
//    device->queues[i].used = q + ;
//    device->queues[i].size = ;
//
//    d->queue_num = queue_num;
//    d->queue_align = ;
//    d->queue_pfn = device->queues[i].desc / PAGE_SIZE;
  }

  device->regs = d;
  return device;
}

static bool virtio_console_check_features(struct VirtioMMIO *d, uint32_t desired_features[4]) {
  for (uint8_t i = 0; i < 4; i++) {
    d->host_featurse_selector = i;
    if ((d->host_features & desired_features[i]) != desired_features[i])
        return false;
  }
  return true;
}

static void virtio_reset(struct VirtioMMIO *d) {
  for (uint8_t i = 0; i < sizeof(d->device_status) / sizeof(*d->device_status); i++) 
    d->device_status[i] = 0x0;
}
