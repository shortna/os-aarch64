#include "drivers.h"
#include "drivers/rtc/rtc.h"
#include "drivers/time_physical.h"
#include "drivers/uart/uart.h"
#include "types.h"

extern uint32_t get_int_id(void);

void handle(void) {
  uint32_t id = get_int_id();
  void *d;
  enum DEVICE_TYPE t = driver_get(&d, id);
  switch (t) {
  case DT_UART:
    uart_clear_interrupt(d, uart_get_interrupt(d));
    break;
  case DT_RTC:
    rtc_clear_interrupt(d);
    break;
  case DT_VIRTIO:
    __asm__("mov x0, 0xbeef");
    while(1);
  default:
    break;
  }

  timer_physical_add_threshold(timer_physical_frequency());
  return;
}
