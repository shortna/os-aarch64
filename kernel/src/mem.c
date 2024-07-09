#include "mem.h"

extern void *FREE_MEMORY;
extern u64 FREE_MEMORY_SIZE;

static bool *free_blocks;
static u8 n_blocks;
static void *free_memory;

void blocks_init(void) {
  free_memory = &FREE_MEMORY;
  u64 free_memory_size = (u64)&FREE_MEMORY_SIZE;

  // how many block fitting - 1 for occupied_blocks
  n_blocks = (free_memory_size / BLOCK_SIZE) - 1;

  free_blocks = free_memory;
  for (u8 i = 0; i < n_blocks; i++) {
    free_blocks[i] = true;
  }
  free_memory += BLOCK_SIZE;
}

void *block_alloc(void) {
  u8 i = 0;
  while (free_blocks[i] != true && i < n_blocks) {
    i++;
  }

  if (i == n_blocks) {
    return NULL;
  }

  return free_memory + i * BLOCK_SIZE;
}

void block_free(void *block) {
  if ((block - (void *)free_memory) % BLOCK_SIZE != 0) {
    return;
  }

  u8 block_ind = (block - (void *)free_memory) / BLOCK_SIZE;
  free_blocks[block_ind] = true;
}
