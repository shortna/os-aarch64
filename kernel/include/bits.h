#ifndef _BITS_H
#define _BITS_H

#include "types.h"

#define BIT(x) (U64(1) << (x))

// clears `width` bits in `value` from `start_bit` 
static inline u64 bits_clear(u64 value, u8 start_bit, u8 width) {
  u64 mask = (BIT(width) - 1) << start_bit;
  return value & ~mask;
}

static inline u16 rotate_u16(u16 n) {
  __asm__("rev16 %w0, %w0" : "=r"(n));
  return n;
}

static inline u32 rotate_u32(u32 n) {
  __asm__("rev32 %0, %0" : "=r"(n));
  return n;
}

static inline u64 rotate_u64(u64 n) {
  __asm__("rev64 %0, %0" : "=r"(n));
  return n;
}

#endif
