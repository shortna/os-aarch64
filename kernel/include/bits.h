#ifndef _BITS_H
#define _BITS_H

#include "types.h"

#define GENMASK(n_bits, position) ((((u64)1 << (n_bits)) - 1) << (position))

// example: x = 0xf, a = 0x8
// 0xf + (0x8 - (0xf % 0x8)) = 0x10
//            ^      ^     
//           0x1    0x7   
#define ALLIGN(x, a) ((x) + ((a) - ((x) % (a))))

// clears `width` bits in `value` from `start_bit` 
static inline u64 bits_clear(u64 value, u8 start_bit, u8 width) {
  u64 mask = (BIT(width) - 1) << start_bit;
  return value & ~mask;
}

static inline u16 bits_rotate_u16(u16 value) {
  __asm__ volatile("rev16 %w0, %w0" : "=r"(value));
  return value;
}

static inline u32 bits_rotate_u32(u64 value) {
  __asm__ volatile("rev32 %0, %0" : "=r"(value));
  return value;
}

static inline u64 bits_rotate_u64(u64 value) {
  __asm__ volatile("rev64 %0, %0" : "=r"(value));
  return value;
}

#endif
