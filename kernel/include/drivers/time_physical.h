#ifndef _TIMER_PHYSICAL_H
#define _TIMER_PHYSICAL_H

#include "types.h"

extern uint64_t timer_physical_frequency(void);
extern uint64_t timer_physical_get_value(void);
extern void timer_physical_enable(void);
extern void timer_physical_set_threshold(uint64_t t);
extern void timer_physical_add_threshold(uint32_t t);
extern uint32_t timer_physical_time_passed(void);

#endif
