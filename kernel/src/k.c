#include "uart/uart.h"
#include "rtc/rtc.h"
#include "gic/gic.h"

// 24MHz
#define APB_CLOCK U64(24000000)

#define RTC_BASE_ADDRESS U64(0x9010000)

#define UART_CLOCK APB_CLOCK
#define UART_BAUD_RATE U64(115200)
#define UART_BASE_ADDRESS U64(0x9000000)

#define GICD_BASE_ADDRESS U64(0x8000000)
#define GICR_BASE_ADDRESS U64(0x80a0000)

extern u64 timer_system_get_frequency(void);
extern u64 timer_physical_get_value(void);
extern void timer_physical_enable(void);
extern void timer_physical_set_threshold(u64 t);
extern void timer_physical_add_threshold(u32 t);
extern u32 timer_physical_time_passed(void);
extern u32 get_int_id(void);

void kmain() {
  GICD dist = gicd(GICD_BASE_ADDRESS);
  GICR redists = gicr(GICD_BASE_ADDRESS);

  enableGIC(dist);

  u32 redistributor_id = getRedistributorId(redists, getAffinity());
  if (redistributor_id == UINT32_MAX) {
    while (1);
  }

  wakeUpRedistributor(redists, redistributor_id);

  // enable SRE bits 
  configureICC();
  setPriorityMask(0xFF);
  enableGroup0Ints();
  enableGroup1Ints();

/*
  // enable timer
  // timer_interrupts = <0x01 0x0d 0x04 0x01 0x0e 0x04 0x01 0x0b 0x04 0x01 0x0a 0x04>;
  u8 physical_counter_ns_int = 30;
  int_set_priority(dist, redists, physical_counter_ns_int, redistributor_id, 0);
  int_set_group(dist, redists, physical_counter_ns_int, redistributor_id, GICV3_GROUP1_NON_SECURE);
  int_set_triger(dist, redists, physical_counter_ns_int, redistributor_id, GICV3_LEVEL_SENSITIVE);
  int_enable(dist, redists, physical_counter_ns_int, redistributor_id);

  timer_physical_add_threshold(100);
  timer_physical_enable();
*/

  // pl031 interrupts
  // interrupts = <0x00 0x02 0x04>;
  u8 pl031_int = 34;
  int_set_priority(dist, redists, pl031_int, redistributor_id, 0);
  int_set_group(dist, redists, pl031_int, redistributor_id, GICV3_GROUP1_NON_SECURE);
  int_set_triger(dist, redists, pl031_int, redistributor_id, GICV3_LEVEL_SENSITIVE);
  int_enable(dist, redists, pl031_int, redistributor_id);

  RTC r = rtc(RTC_BASE_ADDRESS);
  rtc_reset(r);
  rtc_set_match(r, rtc_get_current(r) + 5);
  rtc_enable_interrupt(r);

/*
  // pl011 interrupts
  // uart_interrupts = <0x00 0x01 0x04>;
  u8 pl011_int = 33;
  int_set_priority(dist, redists, pl011_int, redistributor_id, 0);
  int_set_group(dist, redists, pl011_int, redistributor_id, GICV3_GROUP1_NON_SECURE);
  int_set_triger(dist, redists, pl011_int, redistributor_id, GICV3_LEVEL_SENSITIVE);
  int_set_route(dist, pl011_int, getAffinity());
  int_enable(dist, redists, pl011_int, redistributor_id);

  Uart uart0 = uart_setup(UART_BASE_ADDRESS, UART_CLOCK, UART_BAUD_RATE);

  for (u8 i = 'A'; i < 'z'; i++) {
    uart_write_byte(uart0, i);
  }
*/

  while (1);
}

void clear_source(u32 int_id) {
}

void handle(void) {
  u32 int_id = get_int_id();
  clear_source(int_id);
  while(1);
}

