#include "drivers/rtc/rtc_internal.h"
#include "drivers/rtc/rtc.h"

RTC rtc(uint64_t rtc_base) { 
  RTC r = (void *)rtc_base; 
  return r;
}

void rtc_reset(RTC r) {
  r->ITCR = 0; // disable test
  rtc_disable_interrupt(r);
  rtc_add_offset(r, UINT32_MAX);
  rtc_add_offset(r, 0);
  rtc_set_match(r, 0);
  rtc_clear_interrupt(r);
}

uint32_t rtc_get_current(RTC r) {
  return r->DR; 
}

void rtc_set_match(RTC r, uint32_t match_value) {
  r->MR = match_value;
}

void rtc_add_offset(RTC r, uint32_t offset) {
  r->LR = offset;
}

void rtc_enable_interrupt(RTC r) {
  r->IMSC = RTC_IM_SET_MASK;
}

void rtc_disable_interrupt(RTC r) {
  r->IMSC = RTC_IM_CLR_MASK;
}

void rtc_clear_interrupt(RTC r) { 
  r->ICR = RTC_INT_CLR; 
}
