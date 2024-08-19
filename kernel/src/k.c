#include "drivers.h"
#include "drivers/gic/gic.h"
#include "drivers/rtc/rtc.h"
#include "drivers/time_physical.h"
#include "drivers/uart/uart.h"
#include "drivers/virtio/virtio.h"
#include "drivers/mmu/mmu.h"

// 24MHz
#define APB_CLOCK (uint64_t)(24000000)

#define RTC_BASE_ADDRESS (uint64_t)(0x9010000)

#define UART_CLOCK APB_CLOCK
#define UART_BAUD_RATE (uint64_t)(115200)
#define UART_BASE_ADDRESS (uint64_t)(0x9000000)

#define GICD_BASE_ADDRESS (uint64_t)(0x8000000)
#define GICR_BASE_ADDRESS (uint64_t)(0x80a0000)

#define VIRTIO_CONSOLE_ADDRESS (uint64_t)(0xa003e00)

uint64_t BREAKPOINT_IND = 0;

void kmain(uint64_t fdt_address) {
  (void)fdt_address;
  /* GET INFO FROM FDT */
  // fdt_walk(fdt_address);
  mmu_init_kernel(31, GRANULARITY_4KB);
  mmu_enable();

  /* ENBALE INTERRUPT ROUTING */
  uint32_t redist_id = get_redistributor_id(GICR_BASE_ADDRESS, get_affinity());
  if (redist_id == UINT32_MAX) {
    goto hang;
  }

  const GICD dist = get_distributor(GICD_BASE_ADDRESS);
  gic_init(dist);

  GICR redist = get_redistributor(GICR_BASE_ADDRESS, redist_id);
  wake_redistributor(redist);

  set_priority_mask(0xFF);
  enable_group0_ints();
  enable_group1_ints();

  struct InterruptParameters int_p = {.priority = 0,
                                      .security = GICV3_GROUP1_NON_SECURE,
                                      .triger = GICV3_LEVEL_SENSITIVE,
                                      .route = false,
                                      .affinity = 0};

  // enable timer
  // timer_interrupts = <0x01 0x0d 0x04 0x01 0x0e 0x04 0x01 0x0b 0x04 0x01 0x0a 0x04>;
  uint8_t physical_counter_ns_int = 30;
  register_interrupt(dist, redist, physical_counter_ns_int, int_p);

  // interrupt each second
  timer_physical_add_threshold(timer_physical_frequency());
  timer_physical_enable();
  driver_add(DT_TIMER, 0, physical_counter_ns_int);

  // pl031 interrupts
  // interrupts = <0x00 0x02 0x04>;
  uint8_t pl031_int = 34;
  register_interrupt(dist, redist, pl031_int, int_p);

  RTC r = rtc(RTC_BASE_ADDRESS);
  rtc_disable_interrupt(r);
  driver_add(DT_RTC, r, pl031_int);

  // pl011 interrupts
  // uart_interrupts = <0x00 0x01 0x04>;
  uint8_t pl011_int = 33;
  register_interrupt(dist, redist, pl011_int, int_p);

  UART uart0 = uart_init(UART_BASE_ADDRESS);
  uart_set_baud_rate(uart0, UART_CLOCK, UART_BAUD_RATE);
  //  uart_enable_interrupt(uart0, UART_INT_TXIM);
  uart_enable(uart0);
  driver_add(DT_UART, uart0, pl011_int);
  uart_write_byte(uart0, 'U');

//  uint8_t virtio_console_int = 0x2f;
//  VirtioDevice console = virtio(VIRTIO_CONSOLE_ADDRESS, VIRTIO_CONSOLE_F_EMERG_WRITE);
//  if (console == NULL) {
//    goto hang;
//  }
//  register_interrupt(dist, redist, virtio_console_int, int_p);
//  driver_add(DT_VIRTIO, console, virtio_console_int);
//
//  virtio_console_emerge_write(console, 'W');
//  virtio_console_emerge_write(console, 'H');
//  virtio_console_emerge_write(console, 'Y');

hang:
  __asm__("mov x0, 0xdead");
  while (1);
}
