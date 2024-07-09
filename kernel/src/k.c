#include "drivers.h"
#include "mem.h"
#include "drivers/gic/gic.h"
// #include "drivers/rtc/rtc.h"
#include "drivers/time_physical.h"
#include "drivers/uart/uart.h"

// 24MHz
#define APB_CLOCK U64(24000000)

#define RTC_BASE_ADDRESS U64(0x9010000)

#define UART_CLOCK APB_CLOCK
#define UART_BAUD_RATE U64(115200)
#define UART_BASE_ADDRESS U64(0x9000000)

#define GICD_BASE_ADDRESS U64(0x8000000)
#define GICR_BASE_ADDRESS U64(0x80a0000)

void kmain(u64 fdt_address) {
  /* GET INFO FROM FDT */
  // fdt_walk(fdt_address);

  // init memory blocks
  blocks_init();
  driver_list_init();

  /* ENBALE INTERRUPT ROUTING */
  u32 redist_id = get_redistributor_id(GICR_BASE_ADDRESS, get_affinity());
  if (redist_id == UINT32_MAX) {
    while (1);
  }

  const GICD dist = get_distributor(GICD_BASE_ADDRESS);
  gic_init(dist);

  GICR redist = get_redistributor(GICR_BASE_ADDRESS, redist_id);
  wake_redistributor(redist);

  set_priority_mask(0xFF);
  enable_group0_ints();
  enable_group1_ints();

  struct InterruptParameters int_p = {0, GICV3_GROUP1_NON_SECURE,
                                      GICV3_LEVEL_SENSITIVE, false, 0};

  // enable timer
  // timer_interrupts = <0x01 0x0d 0x04 0x01 0x0e 0x04 0x01 0x0b 0x04 0x01 0x0a
  // 0x04>;
  u8 physical_counter_ns_int = 30;
  register_interrupt(dist, redist, physical_counter_ns_int, int_p);

  driver_add(DT_TIMER, 0, physical_counter_ns_int);
  // interrupt each second
  timer_physical_add_threshold(timer_physical_frequency());
  timer_physical_enable();

/*
  // pl031 interrupts
  // interrupts = <0x00 0x02 0x04>;
  u8 pl031_int = 34;

  register_interrupt(dist, redist, pl031_int,
                     (struct InterruptParameters){0, GICV3_GROUP1_NON_SECURE,
                                                  GICV3_LEVEL_SENSITIVE, true,
                                                  get_affinity()});

  RTC r = rtc(RTC_BASE_ADDRESS);
  rtc_set_match(r, rtc_get_current(r) + 5);
  rtc_enable_interrupt(r);

  driver_add(DT_RTC, RTC, pl031_int);
*/

  // pl011 interrupts
  // uart_interrupts = <0x00 0x01 0x04>;
  u8 pl011_int = 33;
  register_interrupt(dist, redist, pl011_int, int_p);

  UART uart0 = uart_init(UART_BASE_ADDRESS);
  uart_set_baud_rate(uart0, UART_CLOCK, UART_BAUD_RATE);
  uart_enable_interrupt(uart0, UART_INT_TXIM);
  uart_enable(uart0);

  driver_add(DT_UART, uart0, pl011_int);

  for (u8 i = 'A'; i < 'z'; i++) {
    uart_write_byte(uart0, i);
  }

  while (1);
}
