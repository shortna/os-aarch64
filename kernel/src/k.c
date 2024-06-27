//#include "uart/uart.h"
//#include "gpio/gpio.h"
#include "rtc/rtc.h"
#include "gic/gic.h"

#define GPIO_BASE_ADDRESS U64(0x9030000)

#define RTC_BASE_ADDRESS U64(0x9010000)

#define UART_CLOCK U64(0x16e3600)
#define UART_BAUD_RATE U64(115200)
#define UART_BASE_ADDRESS U64(0x9000000)

#define GICD_BASE_ADDRESS U64(0x8000000)
#define GICR_BASE_ADDRESS U64(0x80a0000)

extern u32 timer_physical_time_passed(void);
extern u64 timer_physical_value(void);
extern void timer_physical_enable(void);
extern void timer_physical_absolute_threshold(u64 threshold);
extern void timer_physical_add_threshold(u32 v);

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

/*
  // pl031 interrupts
  // interrupts = <0x00 0x02 0x04>;
  u8 pl031_int = 34;
  int_set_priority(dist, redists, pl031_int, redistributor_id, 0);
  int_set_group(dist, redists, pl031_int, redistributor_id, GICV3_GROUP1_NON_SECURE);
  int_set_triger(dist, redists, pl031_int, redistributor_id, GICV3_LEVEL_SENSITIVE);
  int_enable(dist, redists, pl031_int, redistributor_id);

  RTC r = rtc(RTC_BASE_ADDRESS);
  rtc_add_offset(r, 0);
  rtc_set_match(r, MINUTE);
  rtc_enable_interrupt(r);
  rtc_enable(r);
*/

/*
  // pl011 interrupts
  // uart_interrupts = <0x00 0x01 0x04>;
  u8 uart_int = 33;
  int_set_priority(uart_int, redistributor_id, 0);                    // GIC(D|R)_IPRIORITYRn
  int_set_group(uart_int, redistributor_id, GICV3_GROUP1_NON_SECURE); // GIC(D|R)_IGROUPn GIC(D|R)_IGRPMODn
  int_set_triger(uart_int, redistributor_id, GICV3_LEVEL_SENSITIVE);  // GIC(D|R)_ICFGRn
  int_set_route(uart_int, getAffinity());                             // GICD_IROUTERn (only for SPI`s)
  int_enable(uart_int, redistributor_id);                             // GIC(D|R)_ISENABLERn

  Uart *uart0 = uart_setup(UART_BASE_ADDRESS, UART_CLOCK, UART_BAUD_RATE);

  for (u8 i = 'A'; i < 'z'; i++) {
    uart_write_byte(uart0, i);
  }
*/

  while (1);
}

