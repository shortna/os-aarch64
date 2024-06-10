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

#define U8(x) ((u8)x)
#define U16(x) ((u16)x)
#define U32(x) ((u32)x)
#define U64(x) ((u64)x)

#define BIT(x) (U64(1) << x)
#define __GENMASK(x) (BIT(x) - 1)
// mask bits from s to f
#define GENMASK(s, f) ((~__GENMASK(s)) & __GENMASK(f))

// defined in init.S
extern u16 __reverse_u16(u16 n);
extern u32 __reverse_u32(u32 n);
extern u64 __reverse_u64(u64 n);

#define REVERSE_U16(n) (__reverse_u16(n))
#define REVERSE_U32(n) (__reverse_u32(n))
#define REVERSE_U64(n) (__reverse_u64(n))

#endif /* TYPES */
