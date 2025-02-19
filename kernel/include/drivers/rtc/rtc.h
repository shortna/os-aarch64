#ifndef _RTC_H
#define _RTC_H

#include "types.h"

#define MINUTE (60)
#define HOUR   (3600)
#define DAY    (8640000)

struct RTC;
typedef struct RTC *RTC;

// return interface for rtc
extern RTC rtc(uint64_t rtc_base);

// reset rtc
extern void rtc_reset(RTC r);

// get current rtc value
extern uint32_t rtc_get_current(RTC r);

// set match value that will generate interrupt
extern void rtc_set_match(RTC r, uint32_t match_value);

// adds `offset` to `current value` and `match value`
extern void rtc_add_offset(RTC r, uint32_t offset);

// enable interrupt generation
extern void rtc_enable_interrupt(RTC r);

// disable interrupt generation
extern void rtc_disable_interrupt(RTC r);

// clear interrupt
extern void rtc_clear_interrupt(RTC r);

#endif
