#include "string.h"

bool strncmp(const u8 *s1, const u8 *s2, u32 len) {
  u8 c1, c2;
  while (len) {
    c1 = *s1++;
    c2 = *s2++;
    if (c1 != c2) {
      return false;
    }
    len--;
  }
  return true;
}

u32 strlen(const u8 *s) {
  const u8 *tmp_s;
  for (tmp_s = s; *tmp_s != '\0'; tmp_s++) {}
  return tmp_s - s;
}
