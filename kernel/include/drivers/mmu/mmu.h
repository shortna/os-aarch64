#ifndef _MMU_H
#define _MMU_H

#include "types.h"

enum GRANULARITY_TYPE {
  GRANULARITY_16KB = 0x1,
  GRANULARITY_4KB = 0x2,
  GRANULARITY_64KB = 0x3,
};

extern bool mmu_init(enum GRANULARITY_TYPE granularity);
extern bool mmu_enable(void);

extern uint64_t PAGE_SIZE;
extern void* malloc(uint16_t n_blocks);
extern void free(void *addr);

#endif
