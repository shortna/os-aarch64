#ifndef _MEM_H
#define _MEM_H

#include "types.h"
#define BLOCK_SIZE (512)

extern void blocks_init(void);
extern void *block_alloc(u8 n);
extern void block_free(void *block, u8 n);
extern void memset(void *p, u8 v, u64 n);


#endif
