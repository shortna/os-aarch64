#include "uart/uart.h"
#include "uart/uart_internal.h"

static u64 UART_BASE_ADDRESS = 0;
static u64 UART_CLOCK = 0;
static u64 UART_BAUD_RATE = 0;

inline static struct uart_reg get_register(u16 reg) { return uart_regs[reg]; }

inline static void pl011_write_register(u16 reg, u32 value) {
  struct uart_reg r = get_register(reg);
  volatile u32 *mem = (void *)(UART_BASE_ADDRESS + r.offset);
  // (*mem & ~r.mask) - clears bits that can be modified and preserves that
  // can`t (value & r.mask) - removes bits from value that should not be there
  *mem = (*mem & ~r.mask) | (value & r.mask);
}

inline static u32 pl011_read_register(u16 reg) {
  struct uart_reg r = get_register(reg);
  volatile u32 *mem = (void *)(UART_BASE_ADDRESS + r.offset);
  return *mem & r.mask;
}

void uart_setup(u64 base_address, u64 clock, u64 baud_rate) {
  UART_BAUD_RATE = baud_rate;
  UART_BASE_ADDRESS = base_address;
  UART_CLOCK = clock;

  // disable uart with remaining transmit bits
  pl011_write_register(REG_CR, UART_CR_TXE | UART_CR_RXE);

  // uart busy, wait
  while (pl011_read_register(REG_FR) & UART_FR_BUSY) {
  }

  // setting baud_rate
  const float baud_div = (float)UART_CLOCK / (16 * UART_BAUD_RATE);
  pl011_write_register(REG_IBRD, (u16)baud_div);

  // extracts fractional bits from baud_rate
  // then multiplies by 2^n where n is 6 because fbrd 6 bits wide
  // and adds 0.5 to account for rounding error
  pl011_write_register(REG_FBRD, (u8)((baud_div - (u16)baud_div) * 64 + 0.5));

  // disable everything and flush transmit FIFO
  pl011_write_register(REG_LCRH, 0);

  // change lcrh
  pl011_write_register(REG_LCRH, UART_LCRH_FEN);

  // change cr parameters to receive & transmit + flow control for both
  pl011_write_register(REG_CR, UART_CR_TXE | UART_CR_RXE);

  // mask all interrupts
  pl011_write_register(REG_IMSC, 0x3ff);

  // enable uart
  pl011_write_register(REG_CR, UART_CR_UARTEN);
}

inline void uart_write_byte(u8 data) {
  pl011_write_register(REG_DR, data); // data
}

void uart_write(const u8 *data, u32 size) {
  for (u32 i = 0; i < size; i++) {
    uart_write_byte(data[i]);
  }
}
