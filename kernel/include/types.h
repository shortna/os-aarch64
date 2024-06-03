#ifndef TYPES
#define TYPES

#include <float.h>
#include <stdalign.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define BIT(x)     (1ull << x)
#define __GENMASK(x) (BIT(x) - 1)
// mask bits from s to f
#define GENMASK(s, f) ((~__GENMASK(s)) & __GENMASK(f))

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

#endif
