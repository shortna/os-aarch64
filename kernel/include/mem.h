#ifndef _MEM_H
#define _MEM_H

#define BLOCK_SIZE (512)
#include "types.h"

struct MemBlock {
  void *start;
  u8 n_blocks;
};

void blocks_init(void);
void *block_alloc(void);
void block_free(void *block);

#endif
