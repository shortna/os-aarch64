#include "memory.h"

extern u64 FREE_MEMORY;
// granularity 16b 32b 64b 128b 256b 512b 1024b

void *memalloc(u32 size) {
  (void)size;
  return NULL;
}

void memfree(void *p) {
  (void)p;
}
