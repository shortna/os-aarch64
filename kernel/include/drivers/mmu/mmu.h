#ifndef _MMU_H
#define _MMU_H

#include "types.h"

enum GRANULARITY_TYPE {
  GRANULARITY_16KB = 0x1,
  GRANULARITY_4KB = 0x2,
  GRANULARITY_64KB = 0x3,
};

extern bool mmu_init_kernel(uint8_t address_space_n_bits, enum GRANULARITY_TYPE granularity);
extern bool mmu_init_user(uint8_t address_space_n_bits, enum GRANULARITY_TYPE granularity);
extern bool mmu_enable(void);

void *mmu_get_page(void *addr, uint16_t upper_attrs, uint16_t lower_attrs);
void *mmu_get_block(void *addr, uint16_t upper_attrs, uint16_t lower_attrs);
void *mmu_get_table(void *addr, uint16_t attrs);

#endif
