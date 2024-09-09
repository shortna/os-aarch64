#ifndef _VIRTIO_H
#define _VIRTIO_H

#include "types.h"

typedef void* VirtioDevice;

enum VIRTIO_DEVICE {
  VIRTIO_CONSOLE,
};

enum VIRTIO_CONSOLE_FEATURES {
  VIRTIO_CONSOLE_FEATURE_SIZE        = 0x1,
  VIRTIO_CONSOLE_FEATURE_MULTIPORT   = 0x2,
  VIRTIO_CONSOLE_FEATURE_EMERG_WRITE = 0x4,
};

enum VIRTIO_INIT_STATUS {
  VIRTIO_INIT_SUCCESS,
  VIRTIO_INIT_ERROR_INCORRECT_MAGIC,
  VIRTIO_INIT_ERROR_UNKNOWN_DEIVCE,
  VIRTIO_INIT_ERROR_INCORRECT_VERSION,
  VIRTIO_INIT_ERROR_FEATURES_UNSUPPORTED,
  VIRTIO_INIT_ERROR_FAILED_TO_INIT_QUEUES,
};

VirtioDevice virtio_init(uint64_t device_base, enum VIRTIO_DEVICE device, uint32_t desired_features[4], enum VIRTIO_INIT_STATUS* err);
void virtio_console_write(VirtioDevice console, uint8_t *msg);
void virtio_console_emerge_write(VirtioDevice console, uint8_t c);

#endif
