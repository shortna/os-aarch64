#ifndef _MMU_H
#define _MMU_H

#include "types.h"

enum GRANULARITY_TYPE {
  GRANULARITY_16KB = 0x1,
  GRANULARITY_4KB = 0x2,
  GRANULARITY_64KB = 0x3,
};

extern bool mmu_init_kernel(enum GRANULARITY_TYPE granularity);
extern bool mmu_enable(void);

extern void* kmalloc(uint64_t size);
extern void* mmap(void *out_addr);

#endif
