#ifndef _VIRTIO_DEFS_H
#define _VIRTIO_DEFS_H

#include "types.h"

#define VIRTIO_MAGIC              0x74726976

enum VIRTIO_STATUS {
  VIRTIO_STATUS_ACKNOWLEDGE        = 0x1,
  VIRTIO_STATUS_DRIVER             = 0x4,
  VIRTIO_STATUS_DRIVER_OK          = 0x8,
  VIRTIO_STATUS_DEVICE_NEEDS_RESET = 0x40,
  VIRTIO_STATUS_FAILED             = 0x80,
};

struct VirtioMMIO {/*{{{*/
  uint32_t magic;                     /* 0x000 */
  uint32_t version;                   /* 0x004 */
  uint32_t device_id;                 /* 0x008 */
  uint32_t vendor_id;                 /* 0x00C */
  uint32_t host_features;             /* 0x010 */
  uint32_t host_featurse_selector;    /* 0x014 */
  uint8_t  padding0[8];
  uint32_t guest_features;            /* 0x020 */
  uint32_t guest_features_selector;   /* 0x024 */
  uint32_t guest_page_size;           /* 0x028 */
  uint8_t  padding1[4];
  uint32_t queue_selector;            /* 0x030 */
  uint32_t queue_num_max;             /* 0x034 */
  uint32_t queue_num;                 /* 0x038 */
  uint32_t queue_align;               /* 0x03C */
  uint32_t queue_pfn;                 /* 0x040 */
  uint8_t  padding2[12];
  uint32_t queue_notify;              /* 0x050 */
  uint8_t  padding3[12];
  uint32_t interrupt_status;          /* 0x060 */
  uint32_t interrupt_acknowledge;     /* 0x064 */
  uint8_t  padding4[8];
  uint32_t device_status;             /* 0x070 */ 
  uint8_t  padding5[0x8C];
  uint32_t configuration_space[];     /* 0x100 */
};/*}}}*/

/* This marks a buffer as continuing via the next field. *//*{{{*/
#define VIRTQ_DESC_F_NEXT   1
/* This marks a buffer as device write-only (otherwise device read-only). */
#define VIRTQ_DESC_F_WRITE  2
struct VirtQueueDesc {
  /* Address (guest-physical). */
  uint64_t addr;
  /* Length. */
  uint32_t len;
  /* The flags as indicated above. */
  uint16_t flags;
  /* Next field if flags & NEXT */
  uint16_t next;
};

#define VIRTQ_AVAIL_F_NO_INTERRUPT      1
struct VirtQueueAvail {
  uint16_t flags;
  uint16_t idx;
  uint16_t ring[];
};

struct VirtQueueUsedElem {
  /* Index of start of used descriptor chain. */
  uint16_t id;
  /*
   * The number of bytes written into the device writable portion of
   * the buffer described by the descriptor chain.
   */
  uint16_t len;
};

#define VIRTQ_USED_F_NO_NOTIFY  1
struct VirtQueueUsed {
  uint16_t flags;
  uint16_t idx;
  struct VirtQueueUsedElem ring[];
};/*}}}*/

struct VirtQueue {
  struct VirtQueueDesc  *desc;
  struct VirtQueueAvail *avail;
  struct VirtQueueUsed  *used;
  uint32_t size;
};

struct VirtioDevice {
  uint32_t n_queues;
  enum VIRTIO_DEVICE type;
  struct VirtioMMIO *regs;
  struct VirtQueue queues[];
};

#endif
