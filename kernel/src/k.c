#include "memory.h"

void kmain() {
  mem_strap();
  u8 **m = mem_alloc(UINT8_MAX * sizeof(*m));
  for (u8 i = 0; i < UINT8_MAX; i++) {
    m[i] = mem_alloc(UINT8_MAX * sizeof(*m[i]));
    for (u8 j = 0; j < UINT8_MAX; j++) {
      m[i][j] = j;
    }
  }

  u8 *a = mem_alloc(sizeof(*a));
  u16 *b = mem_alloc(sizeof(*b));
  u8 *c = mem_alloc(sizeof(*c));

  *a = 'M';
  *b = 'E';
  *(b + 1) = 'E';
  *c = 'M';

  mem_free(b);
  u8 *d = mem_alloc(sizeof(*d));
  u8 *e = mem_alloc(sizeof(*e));

  *d = 'N';
  *e = 'N';

  for (u8 i = 0; i < UINT8_MAX; i++) {
    mem_free(m[i]);
  }
  mem_free(m);

  while (1);
}
