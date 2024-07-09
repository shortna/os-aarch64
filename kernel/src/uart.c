#include "drivers/uart/uart.h"
#include "drivers/uart/uart_internal.h"

UART uart_init(u64 uart_base) {
  u32 tmp_reg;
  UART uart = (void *)uart_base;

  uart_disable(uart);

  while (uart->FR & UART_FR_BUSY) {}

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

  // enable transmit
  tmp_reg = uart->CR;
  tmp_reg &= ~UINT16_MAX;
  tmp_reg |= UART_CR_TXE;
  uart->CR = tmp_reg;

  // clear interrupts and disable them
  uart->IMSC &= ~0x7ff;
  uart->ICR |= 0x7ff;

  return uart;
}

void uart_enable_interrupt(UART u, enum UART_INT_TYPE t) {
  t = 1 << t;
  u32 tmp_reg;
  tmp_reg = u->IMSC;
  tmp_reg |= t;
  u->IMSC = tmp_reg;
}

void uart_disable_interrupt(UART u, enum UART_INT_TYPE t) {
  t = 1 << t;
  u->IMSC &= ~t;
}

void uart_clear_interrupt(UART u, enum UART_INT_TYPE t) {
  t = 1 << t;
  u->ICR |= t;
}

enum UART_INT_TYPE uart_get_interrupt(UART u) {
  u32 tmp_reg = u->MIS & 0x7ff;
  for (u8 i = 0; i < 11; i++) {
    if ((tmp_reg >> i) & 1) {
      return i;
    }
  }
  return UART_INT_NONE;
}

void uart_set_baud_rate(UART u, u64 uart_clock, u64 baud_rate) {
  // setting baud_rate
  // since FBRD is 6 bits by multiplying clk / (16 * br) by 64 (1 << 6)
  // all fractional bits are now integer bits
  u32 baud_div = 4 * uart_clock / baud_rate;
  // to get integer lower 6 bits are discarded and 16 bits are taken ibrd
  u->IBRD = (baud_div >> 6) & 0xffff;
  // to get fractional lower 6 bits needs to be extracted
  u->FBRD |= baud_div & 0x3f;
}

void uart_enable(UART u) { u->CR |= UART_CR_UARTEN; }

void uart_disable(UART u) {
  u32 tmp = u->CR;
  tmp &= ~1;
  u->CR = tmp;
}

void uart_write_byte(UART u, u8 data) { u->DR |= data; }

void uart_write(UART u, const u8 *data, u32 size) {
  for (u32 i = 0; i < size; i++) {
    uart_write_byte(u, data[i]);
  }
}
