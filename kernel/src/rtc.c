#include "rtc/rtc_internal.h"
#include "rtc/rtc.h"

RTC rtc(u64 rtc_base) { 
  RTC r = (void *)rtc_base; 
  r->ICR |= RTC_INT_CLR;
  return r;
}

void rtc_reset(RTC r) {
}

void rtc_enable(RTC r) {
  r->CR |= RTC_CR_START;
}

u32 rtc_get_current(RTC r) {
  return r->DR; 
}

void rtc_set_match(RTC r, u32 match_value) {
  r->MR = rtc_get_current(r) + match_value;
}

void rtc_add_offset(RTC r, u32 offset) {
  r->LR = offset;
}

void rtc_enable_interrupt(RTC r) {
  r->IMSC |= RTC_IM_SET_MASK;
}

void rtc_disable_interrupt(RTC r) {
  r->IMSC &= RTC_IM_CLR_MASK;
}

void rtc_clear_interrupt(RTC r) { 
  r->ICR |= RTC_INT_CLR; 
}
