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
#define BREAKPOINT(v) __asm__ volatile("brk " #v)

#define GLOBAL_VARIABLES (0x40000000)

#endif /* TYPES */
