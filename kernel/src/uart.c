#include "drivers/uart/uart.h"
#include "drivers/uart/uart_internal.h"

UART uart_setup(u64 uart_base, u64 uart_clock, u64 baud_rate) {
  u32 tmp_reg;
  UART uart = (void *)uart_base;

  // disable uart
  tmp_reg = uart->CR;
  tmp_reg &= ~UART_CR_UARTEN;

  // uart busy, wait
  while (uart->FR & UART_FR_BUSY) {}

  // setting baud_rate
  // since FBRD is 6 bits by multiplying clk / (16 * br) by 64 (1 << 6)
  // all fractional bits are now integer bits
  u32 baud_div = 4 * uart_clock / baud_rate;
  // to get integer lower 6 bits are discarded and 16 bits are taken ibrd
  uart->IBRD = (baud_div >> 6) & 0xffff;
  // to get fractional lower 6 bits needs to be extracted
  uart->FBRD |= baud_div & 0x3f;

  // disable everything and flush transmit FIFO
  tmp_reg = uart->LCRH;
  tmp_reg = bits_clear(tmp_reg, 0, 8);
  uart->LCRH = tmp_reg;

  // change lcrh
  tmp_reg |= UART_LCRH_WLEN8 | UART_LCRH_FEN;
  uart->LCRH = tmp_reg;

  // set flood level
  tmp_reg = uart->IFLS;
  tmp_reg &= ~0x1f;
  tmp_reg |= UART_IFLS_TX_1_2;
  uart->IFLS = tmp_reg;

  // change cr parameters to transmit flow control
  tmp_reg = uart->CR;
  tmp_reg &= ~UINT16_MAX;
  tmp_reg |= UART_CR_TXE;
  uart->CR = tmp_reg;

  // clear interrupts
  uart->ICR |= 0x7ff;

  // enable interrupts
  tmp_reg = uart->IMSC;
  tmp_reg |= UART_IMSC_TXIM;
  uart->IMSC = tmp_reg;

  return uart;
}

void uart_transmit(UART u) {
  u->CR |= UART_CR_UARTEN;
}

void uart_write_byte(UART u, u8 data) { 
  u->DR |= data;
}

void uart_write(UART u, const u8 *data, u32 size) {
  for (u32 i = 0; i < size; i++) {
    uart_write_byte(u, data[i]);
  }
}

u8 uart_read_byte(UART u) {
  (void)u;
  return 0;
}

u32 uart_read(UART u, u8 *data, u32 size) {
  (void)u;
  (void)data;
  (void)size;
  return 0;
}

bool uart_data_avaliable(UART u) {
  (void)u;
  return false;
}
