#include "uart.h"

inline static struct uart_reg get_register(u16 reg) {
  return pl011_offsets[reg];
}

inline static void pl011_write_register(u16 reg, u32 value) {
  struct uart_reg r = get_register(reg);
  volatile u32 *mem = (void*)(UART_BASE_ADDRES + r.offset);
  // (*mem & ~r.mask) - clears bits that can be modified
  // (value & r.mask) - removes bits from value that should not be there
  *mem |= (*mem & ~r.mask) | (value & r.mask);
}

inline static u32 pl011_read_register(u16 reg) {
  struct uart_reg r = get_register(reg);
  volatile u32 *mem = (void*)(UART_BASE_ADDRES + r.offset);
  return *mem & r.mask;
}

// setups uart in fifo mode with flow control and dma
// 8bit data without parity with 1bit stop
void uart_setup() {
  // disable uart with remaining transmit bits
  pl011_write_register(REG_CR, UART_CR_RXE | UART_CR_TXE);

  // uart busy, wait
  while (pl011_read_register(REG_FR) & UART_FR_BUSY) {}

  // setting baud_rate
  const float baud_rate = (float)UART_CLOCK / (16 * UART_BAUD_RATE);
  pl011_write_register(REG_IBRD, (u16)baud_rate);

  // extracts fractional bits from baud_rate
  // then multiplies by 2^n where n is 6 because fbrd 6 bits wide
  // and adds 0.5 to account for rounding error
  pl011_write_register(REG_FBRD, (u8)((baud_rate - (u16)baud_rate) * 64 + 0.5));

  // disable everything and flush transmit FIFO
  pl011_write_register(REG_LCRH, 0);

  // change lcrh parameters
  pl011_write_register(REG_LCRH, UART_LCRH_FEN | UART_LCRH_WLEN8);

  // change cr parameters
  pl011_write_register(REG_CR, UART_CR_TXE | UART_CR_RXE | UART_CR_RTSEn | UART_CR_CTSEn);

  // set transmit and recieve trigger for interrupt
  pl011_write_register(REG_IFLS, UART_IFLS_TX_1_2 | UART_IFLS_RX_1_2); // half filled

  // dma
  // pl011_write_register(REG_DMACR, UART_DMACR_RXDMAE | UART_DMACR_TXDMAE); 

  // enable uart
  pl011_write_register(REG_CR, UART_CR_UARTEN);
}

void uart_write_byte(u8 data) {
  pl011_write_register(REG_DR, data); // data
}

void uart_write(const u8 *data, u32 size) {
  for (u32 s = 0; s < size; s++) {
    pl011_write_register(REG_DR, data[s]);
  }
}
