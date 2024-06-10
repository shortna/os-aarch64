#ifndef _MEMORY_H
#define _MEMORY_H

#include "types.h"

#define ALLOC_MAX U16(1024) // 1Kb

extern void *memalloc(u32 size);
extern void memfree(void *p);

#endif
