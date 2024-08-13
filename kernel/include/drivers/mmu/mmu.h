#ifndef _MMU_H
#define _MMU_H

#include "types.h"

enum GRANULARITY_TYPE {
  GRANULARITY_16KB = 0x1,
  GRANULARITY_4KB = 0x2,
  GRANULARITY_64KB = 0x3,
};

extern bool mmu_init_kernel(u8 address_space_n_bits, enum GRANULARITY_TYPE granularity);
extern bool mmu_init_user(u8 address_space_n_bits, enum GRANULARITY_TYPE granularity);
extern bool mmu_enable(void);

#endif
