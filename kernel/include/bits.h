#ifndef _BITS_H
#define _BITS_H

#include "types.h"

#define GENMASK(n_bits, position) ((((uint64_t)1 << (n_bits)) - 1) << (position))
#define ALIGN(x, a) (((x) + ((a) - 1)) & ~((a) - 1))
#define BIT(x) ((uint64_t)1 << (x))

// clears `width` bits in `value` from `start_bit` 
static inline uint64_t bits_clear(uint64_t value, uint8_t start_bit, uint8_t width) {
  uint64_t mask = (BIT(width) - 1) << start_bit;
  return value & ~mask;
}

static inline uint8_t ctz(uint64_t value) {
  uint8_t i = 0;
  while (!((value >> i) & 1)) {
    i++;
  }
  return i;
}

static inline uint8_t clz(uint64_t value) {
  __asm__ volatile("clz %0, %0" : "=r"(value));
  return value;
}

static inline uint16_t bits_rotate_uint16_t(uint16_t value) {
  __asm__ volatile("rev16 %w0, %w0" : "=r"(value));
  return value;
}

static inline uint32_t bits_rotate_uint32_t(uint64_t value) {
  __asm__ volatile("rev32 %0, %0" : "=r"(value));
  return value;
}

static inline uint64_t bits_rotate_uint64_t(uint64_t value) {
  __asm__ volatile("rev64 %0, %0" : "=r"(value));
  return value;
}

#endif
