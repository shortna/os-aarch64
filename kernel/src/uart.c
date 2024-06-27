#include "uart/uart.h"
#include "uart/uart_internal.h"

Uart uart_setup(u64 uart_base, u64 uart_clock, u64 baud_rate) {
  u32 tmp_reg;
  struct Uart *uart = (void *)uart_base;

  // disable uart
  tmp_reg = uart->CR;
  tmp_reg &= ~UART_CR_UARTEN;

  // uart busy, wait
  while (uart->FR & UART_FR_BUSY) {}

  // setting baud_rate
  const float baud_div = (float)uart_clock / (16 * baud_rate);
  uart->IBRD |= (u16)baud_div;

  // extracts fractional bits from baud_rate
  // then multiplies by 2^n where n is 6 because fbrd 6 bits wide
  // and adds 0.5 to account for rounding error
  uart->FBRD |= (u8)((baud_div - (u16)baud_div) * 64 + 0.5);

  // disable everything and flush transmit FIFO
  tmp_reg = uart->LCRH;
  tmp_reg = bits_clear(tmp_reg, 0, 8);
  uart->LCRH = tmp_reg;

  // change lcrh
  tmp_reg |= UART_LCRH_WLEN8 | UART_LCRH_FEN;
  uart->LCRH = tmp_reg;

  // set flood level
  tmp_reg = uart->IFLS;
  tmp_reg |= UART_IFLS_RX_1_4 | UART_IFLS_TX_1_4;
  uart->IFLS = tmp_reg;

  // clear interrupts
  uart->ICR |= 0x7ff;

  // enable UARTTXINTR
  tmp_reg = uart->IMSC;
  tmp_reg |= BIT(5);
  uart->IMSC = tmp_reg;

  // change cr parameters to receive & transmit + flow control for both
  tmp_reg = uart->CR;
  tmp_reg = bits_clear(tmp_reg, 7, 8);
  tmp_reg |= UART_CR_RTSEn | UART_CR_CTSEn;
  uart->CR = tmp_reg;

  // enable uart
  uart->CR |= UART_CR_UARTEN;
  return uart;
}

void uart_write_byte(Uart uart, u8 data) { 
  ((struct Uart *)uart)->DR |= data;
}

void uart_write(Uart uart, const u8 *data, u32 size) {
  for (u32 i = 0; i < size; i++) {
    uart_write_byte(uart, data[i]);
  }
}

u8 uart_read_byte(Uart uart) {
  (void)uart;
  return 0;
}

u32 uart_read(Uart uart, u8 *data, u32 size) {
  (void)uart;
  (void)data;
  (void)size;
  return 0;
}

bool uart_data_avaliable(Uart uart) {
  (void)uart;
  return false;
}
