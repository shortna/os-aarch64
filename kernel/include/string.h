#ifndef _STRING_H
#define _STRING_H

#include "types.h"

/*
 * Sets `n` bytes in `p` with `value` 
 */
extern void memset(void *p, u32 n, u8 value);

/*
 * Compares `n` bytes in `a` with `b`
 */
extern bool memcmp(void *a, void *b, u32 n);

/*
 * Finds byte `b` in `p` and places in `res`.
 * Scans at most `n` bytes.
 */
extern bool memfind(void *p, u32 n, u8 b, u32 *res);

/*
 * count chars in `s` till \0
 */
extern u32 strlen(u8 *s);

#endif
