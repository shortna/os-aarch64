#include "memory.h"

/*
 * Idea is to keep pointers to allocated chunks prefixed with u16 size
 */

void* MEMORY_START = 0;
void* MEMORY_END = 0;

#define MAX_CHUNKS (256)

struct Chunks {
  void **occupied; // array of pointers to chunks
  u16 n;
};

static struct Chunks chunks;

void *mem_alloc(u16 size) {
  if (chunks.n == MAX_CHUNKS || size > ALLOC_MAX) {
    return NULL;
  }

  if (chunks.n == 0) {
    *chunks.occupied = (void *)MEMORY_START;
    **(u16 **)chunks.occupied = size + sizeof(u16);
    chunks.n++;
    return *chunks.occupied + sizeof(u16);
  }

  void *chunk_end;
  for (u16 i = 0; i < chunks.n - 1; i++) {
    chunk_end = chunks.occupied[i] + *(u16 *)chunks.occupied[i];
    if ((u32)(chunks.occupied[i + 1] - chunk_end) >= size + sizeof(u16)) {
      mem_shift_right(chunks.occupied + i,
                      (chunks.n - i) * sizeof(*chunks.occupied), 1);
      chunks.occupied[i] = chunk_end;
      *(u16 *)chunk_end = size + sizeof(u16);
      chunks.n++;
      return chunk_end + sizeof(u16);
    }
  }

  chunk_end =
      chunks.occupied[chunks.n - 1] + *(u16 *)chunks.occupied[chunks.n - 1];

  if ((u32)((void *)MEMORY_END - chunk_end) >= size + sizeof(u16)) {
    chunks.n++;
    chunks.occupied[chunks.n] = chunk_end;
    *(u16 *)chunk_end = size + sizeof(u16);
    return chunk_end + sizeof(u16);
  }
  return NULL;
}

void mem_free(void *p) {
  if (chunks.n == 0) {
    return;
  }

  for (u16 i = 0; i < chunks.n; i++) {
    if (p - sizeof(u16) == chunks.occupied[i]) {
      mem_shift_left(chunks.occupied + i,
                     (chunks.n - i) * sizeof(*chunks.occupied), 1);
    }
  }
}

void mem_shift_right(void *src, u16 size, u16 n) {
  for (u8 *i = src; i < (u8 *)src + size - n; i++) {
    *(i + n) = *i;
  }
}

void mem_shift_left(void *src, u16 size, u16 n) {
  for (u8 *i = src; i < (u8 *)src + size - n; i++) {
    *i = *(i + n);
  }
}

void *mem_realloc(void *p, u16 size) {
  (void)p;
  (void)size;
  return NULL;
}

void mem_strap(void) {
  chunks.occupied = (void *)MEMORY_START;
  chunks.n = 0;
  MEMORY_START += MAX_CHUNKS * sizeof(*chunks.occupied);
}
