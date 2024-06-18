#include "uart/uart.h"
#include "uart/uart_internal.h"

static volatile void *UART_BASE;

void uart_setup(volatile void *uart_base, u64 baud_rate) {
  UART_BASE = uart_base;
  u32 tmp_reg;

  // disable uart
  tmp_reg = mmio_register_read(UART_BASE, UART_CR) & ~U32(1);
  mmio_register_write(UART_BASE, UART_CR, tmp_reg);

  // uart busy, wait
  while (mmio_register_read(UART_BASE, UART_FR) & UART_FR_BUSY) {}

  // setting baud_rate
  const float baud_div = (float)UART_CLOCK / (16 * baud_rate);
  mmio_register_write(UART_BASE, UART_IBRD, (u16)baud_div);

  // extracts fractional bits from baud_rate
  // then multiplies by 2^n where n is 6 because fbrd 6 bits wide
  // and adds 0.5 to account for rounding error
  mmio_register_write(UART_BASE, UART_FBRD, (u8)((baud_div - (u16)baud_div) * 64 + 0.5));

  // disable everything and flush transmit FIFO
  tmp_reg = mmio_register_read(UART_BASE, UART_LCRH);
  mmio_register_write(UART_BASE, UART_LCRH, bits_clear(tmp_reg, 0, 8));

  // change lcrh
  tmp_reg |= UART_LCRH_WLEN8 | UART_LCRH_FEN;
  mmio_register_write(UART_BASE, UART_LCRH, tmp_reg);

  // set flood level
  tmp_reg = mmio_register_read(UART_BASE, UART_IFLS);
  tmp_reg |= UART_IFLS_RX_1_4 | UART_IFLS_TX_1_4;
  mmio_register_write(UART_BASE, UART_IFLS, tmp_reg);

  // enable interrupts
  tmp_reg = mmio_register_read(UART_BASE, UART_IMSC);
  tmp_reg &= ~(BIT(11) - 1);
  mmio_register_write(UART_BASE, UART_IMSC, tmp_reg);

  // change cr parameters to receive & transmit + flow control for both
  tmp_reg = mmio_register_read(UART_BASE, UART_CR);
  tmp_reg |= UART_CR_TXE | UART_CR_RXE | UART_CR_RTSEn | UART_CR_CTSEn;
  mmio_register_write(UART_BASE, UART_CR, tmp_reg);

  // enable uart
  mmio_register_write(UART_BASE, UART_CR, tmp_reg | 1);
}

inline void uart_write_byte(u8 data) {
  u32 tmp = mmio_register_read(UART_BASE, UART_DR) | data;
  mmio_register_write(UART_BASE, UART_DR, tmp);
}

void uart_write(const u8 *data, u32 size) {
  for (u32 i = 0; i < size; i++) {
    uart_write_byte(data[i]);
  }
}
