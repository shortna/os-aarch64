#include "drivers/gic/gic.h"
#include "drivers/rtc/rtc.h"
#include "fdt.h"
#include "drivers/uart/uart.h"
#include "drivers/time_physical.h"

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
  bool err = false;
  FDT t = fdt_init(fdt_address, &err);
  if (err) {
    while(1);
  }

  u32 size_cells;
  u32 address_cells;

  struct FdtNode node;
  struct FdtProperty property;
  while (fdt_get_node(t, &node)) {
    while (fdt_get_property(t, &node, &property)) {
    }
  }

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

/*
  // enable timer
  // timer_interrupts = <0x01 0x0d 0x04 0x01 0x0e 0x04 0x01 0x0b 0x04 0x01 0x0a
  // 0x04>;
  u8 physical_counter_ns_int = 30;
  register_interrupt(dist, redist, physical_counter_ns_int,
                     (struct InterruptParameters){0, GICV3_GROUP1_NON_SECURE,
                                                  GICV3_LEVEL_SENSITIVE, false,
                                                  0});

  timer_physical_add_threshold(100);
  timer_physical_enable();
*/

/*
  // pl031 interrupts
  // interrupts = <0x00 0x02 0x04>;
  u8 pl031_int = 34;

  register_interrupt(dist, redist, pl031_int,
                     (struct InterruptParameters){0, GICV3_GROUP1_NON_SECURE,
                                                  GICV3_LEVEL_SENSITIVE, true,
                                                  get_affinity()});

  RTC r = rtc(RTC_BASE_ADDRESS);
  rtc_reset(r);
  rtc_set_match(r, rtc_get_current(r) + 5);
  rtc_enable_interrupt(r);
*/

/*
  // pl011 interrupts
  // uart_interrupts = <0x00 0x01 0x04>;
  u8 pl011_int = 33;
  register_interrupt(dist, redist, pl011_int,
                     (struct InterruptParameters){0, GICV3_GROUP1_NON_SECURE,
                                                  GICV3_LEVEL_SENSITIVE, false,
                                                  0});

  UART uart0 = uart_setup(UART_BASE_ADDRESS, UART_CLOCK, UART_BAUD_RATE);

  for (u8 i = 'A'; i < 'z'; i++) {
    uart_write_byte(uart0, i);
  }
*/

  while (1);
}
