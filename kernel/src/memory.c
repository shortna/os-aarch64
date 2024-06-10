#include "memory.h"

static u64 MEMORY_ADDRESS = 0;
// granularity 16b 32b 64b 128b 256b 512b 1024b

void *memalloc(u32 size) {
  if (MEMORY_ADDRESS == 0) {
    return NULL;
  }
  (void)size;
  return NULL;
}

void memfree(void *p) {
  (void)p;
}

void set_memory_address(u64 address) {
  MEMORY_ADDRESS = address;
}
