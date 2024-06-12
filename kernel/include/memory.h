#ifndef _MEMORY_H
#define _MEMORY_H

#include "types.h"

#define ALLOC_MAX U16(1024) // 1Kb

extern void *mem_alloc(u16 size);
extern void *mem_realloc(void *p, u16 size);
extern void mem_free(void *p);

/*
 * Shifts `n` bytes from `p` which has size of `size`.
 */
extern void mem_shift_right(void *p, u16 size, u16 n);
extern void mem_shift_left(void *p, u16 size, u16 n);

extern void mem_strap(void);

#endif
