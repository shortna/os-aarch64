#include "string.h"

void memset(void *s, u32 n, u8 value) {
  for (u32 i = 0; i < n; i++) {
    *((u8 *)s + i) = value;
  }
}

bool memcmp(void *a, void *b, u32 n) {
  for (u32 i = 0; i < n; i++) {
    if (((u8 *)a)[i] != ((u8 *)b)[i]) {
      return false;
    }
  }
  return true;
}

bool memfind(void *p, u32 n, u8 b, u32 *res) {
  for (u32 i = 0; i < n; i++) {
    if (((u8 *)p)[i] == b) {
      *res = i;
      return true;
    }
  }
  return false;
}

u32 strlen(u8 *s) {
  u32 i = 0;
  while (*s) {
    i++;
    s++;
  }
  return i;
}
