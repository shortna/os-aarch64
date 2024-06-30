#ifndef _TYPES_H
#define _TYPES_H

#include <float.h>
#include <stdalign.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

#define U8(x) ((u8)(x))
#define U16(x) ((u16)(x))
#define U32(x) ((u32)(x))
#define U64(x) ((u64)(x))

#define BIT(x) (U64(1) << (x))

static inline void mmio_register_write(volatile void *base, u32 offset, u32 value) {
  volatile u32 *addr = base + offset;
  *addr = value;
}

static inline u32 mmio_register_read(volatile void *base, u32 offset) {
  return *(volatile u32 *)(base + offset);
}

// clears `width` bits in `value` from `start_bit` 
static inline u64 bits_clear(u64 value, u8 start_bit, u8 width) {
  u64 mask = (BIT(width) - 1) << start_bit;
  return value & ~mask;
}

#endif /* TYPES */
