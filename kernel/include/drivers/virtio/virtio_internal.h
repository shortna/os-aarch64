#ifndef _VIRTIO_DEFS_H
#define _VIRTIO_DEFS_H

#include "types.h"

#define VIRTIO_MAGIC (0x74726976)
#define VIRTIO_VERSION (1)

#define VIRTIO_STATUS_ACKNOWLEDGE (1)
#define VIRTIO_STATUS_DRIVER (2)
#define VIRTIO_STATUS_FAILED (128)
#define VIRTIO_STATUS_FEATURES_OK (8)
#define VIRTIO_STATUS_DRIVER_OK (4)
#define VIRTIO_STATUS_DEVICE_NEEDS_RESET (64)

#define VIRTQ_USED_BUFFER (1)

#define VIRTQ_DESC_F_INDIRECT (4)
#define VIRTQ_DESC_F_WRITE (2)
#define VIRTQ_DESC_F_NEXT (1)

#define VIRTQ_AVAIL_F_NO_INTERRUPT (1)
#define VIRTQ_USED_F_NO_NOTIFY (1)

struct VirtQueueDesc {
  u64 addr;
  u32 len;
  u16 flags;
  u16 next;
};

struct VirtQueueAvail {
  u16 idx;
  u16 flags;
  u16 ring[]; // u16 used_event is always last element in this array
};

struct VirtQueueUsedElem {
  u32 id;
  u32 len;
};

struct VirtQueueUsed {
  u16 flags;
  u16 idx;
  struct VirtQueueUsedElem ring[]; // u16 avail_event always last element in this array
};

struct VirtioMMIO {
  const volatile u32 magic;                   /* +0x000 R  */
  const volatile u32 version;                 /* +0x004 R  */
  const volatile u32 device_id;               /* +0x008 R  */
  const volatile u32 vendor_id;               /* +0x00C R  */
  const volatile u32 device_features;         /* +0x010 R  */
  volatile u32 device_features_selector;      /* +0x014 W  */
  const volatile u8 padding_0[8];             /* +0x018    */
  volatile u32 driver_features;               /* +0x020 W  */
  volatile u32 driver_features_selector;      /* +0x024 W  */
  const volatile u8 padding_1[8];             /* +0x028    */
  volatile u32 queue_selector;                /* +0x030 W  */
  const volatile u32 queue_num_max;           /* +0x034 R  */
  volatile u32 queue_num;                     /* +0x038 W  */
  const volatile u8 padding_2[8];             /* +0x03C    */
  volatile u32 queue_ready;                   /* +0x044 RW */
  const volatile u8 padding_3[8];             /* +0x048    */
  volatile u32 queue_notify;                  /* +0x050 W  */
  const volatile u8 padding_4[12];            /* +0x054    */
  const volatile u32 interrupt_status;        /* +0x060 R  */
  volatile u32 interrupt_acknowledge;         /* +0x064 W  */
  const volatile u8 padding_5[8];             /* +0x068    */
  volatile u32 status;                        /* +0x070 RW */
  const volatile u8 padding_6[12];            /* +0x074    */
  volatile u32 queue_desc_low;                /* +0x080 W  */
  volatile u32 queue_desc_high;               /* +0x084 W  */
  const volatile u8 padding_7[8];             /* +0x088    */
  volatile u32 queue_driver_low;              /* +0x090 W  */
  volatile u32 queue_driver_high;             /* +0x094 W  */
  const volatile u8 padding_8[8];             /* +0x098    */
  volatile u32 queue_device_low;              /* +0x0A0 W  */
  volatile u32 queue_device_high;             /* +0x0A4 W  */
  const volatile u8 padding_9[4];             /* +0x0A8    */
  volatile u32 shared_memory_id;              /* +0x0AC W  */
  const volatile u32 shared_memory_len_low;   /* +0x0B0 R  */
  const volatile u32 shared_memory_len_high;  /* +0x0B4 R  */
  const volatile u32 shared_memory_base_low;  /* +0x0B8 R  */
  const volatile u32 shared_memory_base_high; /* +0x0BC R  */
  volatile u32 queue_reset;                   /* +0x0C0 RW */
  const volatile u8 padding_10[54];           /* +0x0C4    */
  const volatile u32 config_generation;       /* +0x0FC R  */
  volatile u32 config[0x40];                  /* +0x100 RW */
};

struct Virtio {
  struct VirtioMMIO *registers;
  u32 n_queues;
  u32 max_n_queues;
  struct {
    void *q;
    u32 n_elements;
  } queues[];
};

extern void virtio_reset(struct Virtio *device);
extern bool virtio_console_init(struct Virtio *device, u64 features);

struct Device {
  u64 id;
  // function that takes device, features of device and returns number of initialized queues
  bool (*init)(struct Virtio *, u64);
};

static const struct Device DEVICES[] = {
    {1, NULL},                // Network Device
    {2, NULL},                // Block Device
    {3, virtio_console_init}, // Console Device
    {4, NULL},                // Entropy Device
    {5, NULL},                // Traditional Memory Balloon Device
    {8, NULL},                // SCSI Host Device
    {16, NULL},               // GPU Device
    {18, NULL},               // Input Device
    {19, NULL},               // Socket Device
    {20, NULL},               // Crypto Device
    {23, NULL},               // IOMMU Device
    {24, NULL},               // Memory Device
    {25, NULL},               // Sound Device
    {26, NULL},               // File System Device
    {27, NULL},               // PMEM Device
    {28, NULL},               // RPMB Device
    {32, NULL},               // SCMI Device
    {34, NULL},               // I2C Adapter Device
    {41, NULL},               // GPIO Device
};

#endif
