#include "mem.h"

extern void *MEMORY_START;
extern u64 MEMORY_SIZE;

static bool *free_blocks;
static u8 n_blocks;
static void *free_memory;

void blocks_init(void) {
  free_memory = &MEMORY_START;
  u64 free_memory_size = (u64)&MEMORY_SIZE;

  // how many block fitting - 1 for occupied_blocks
  n_blocks = (free_memory_size / BLOCK_SIZE) - 1;

  free_blocks = free_memory;
  for (u8 i = 0; i < n_blocks; i++) {
    free_blocks[i] = true;
  }
  free_memory += BLOCK_SIZE;
}

void *block_alloc(u8 n) {
  u8 i = 0;
  bool free;
  while (i < n_blocks - n) {
    free = true;
    u8 j = i;
    while (j < i + n) {
      if (free_blocks[j] != true) {
        free = false;
        break;
      }
      free_blocks[j] = false;
      j++;
    }

    if (free) {
      break;
    }

    for (; i < j; i++) {
      free_blocks[i] = true;
    }
    i++;
  }

  return free_memory + i * BLOCK_SIZE;
}

void block_free(void *block, u8 n) {
  if (block < free_memory) {
    return;
  }

  if ((block - (void *)free_memory) % BLOCK_SIZE != 0) {
    return;
  }

  u8 block_ind = (block - (void *)free_memory) / BLOCK_SIZE;
  if (block_ind + n >= n_blocks) {
    return;
  }

  for (u8 i = 0; i < n; i++) {
    free_blocks[block_ind] = true;
    block_ind++;
  }
}

void memset(void *p, u8 v, u64 n) {
  while (n) {
    *(u8 *)p = v;
    n--;
  }
}
