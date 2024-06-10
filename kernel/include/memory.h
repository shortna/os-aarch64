#ifndef _MEMORY_H
#define _MEMORY_H

#include "types.h"

#define ALLOC_MAX U16(1024) // 1Kb

extern void *memalloc(u32 size);
extern void memfree(void *p);

/*
 * Sets `address` of memory chunk so it can be allocated
 */
extern void set_memory_address(u64 address);

#endif
