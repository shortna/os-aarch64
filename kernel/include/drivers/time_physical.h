#ifndef _TIMER_PHYSICAL_H
#define _TIMER_PHYSICAL_H

#include "types.h"

extern u64 timer_physical_frequency(void);
extern u64 timer_physical_get_value(void);
extern void timer_physical_enable(void);
extern void timer_physical_set_threshold(u64 t);
extern void timer_physical_add_threshold(u32 t);
extern u32 timer_physical_time_passed(void);

#endif
