#ifndef _TYPES_H
#define _TYPES_H

#include <float.h>
#include <stdalign.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define BREAKPOINT(v) __asm__ volatile("brk " #v)
extern void* FREE;

#endif /* TYPES */
