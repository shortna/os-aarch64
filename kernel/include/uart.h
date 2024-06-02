#ifndef UART
#define UART

#include "types.h"

// pl011 - Peripheral Controller
// address deduced from dump of device tree blob (dtb)
#define UART_BASE_ADDRES (ULL(0x9000000))
#define UART_CLOCK (ULL(24000000))

/* pl011 UART register offset table */
#define UART_DR      0x00
#define UART_RSR     0x04
#define UART_ESR     0x04
#define UART_FR      0x18
#define UART_ILPR    0x20
#define UART_IBRD    0x24
#define UART_FBRD    0x28
#define UART_LCRH    0x2C
#define UART_CR      0x30
#define UART_IFLS    0x34
#define UART_IMSC    0x38
#define UART_RIS     0x3C
#define UART_MIS     0x40
#define UART_ICR     0x44
#define UART_DMACR   0x48

typedef enum {
  REG_DR,
  REG_RSR,
  REG_ESR,
  REG_FR,
  REG_ILPR,
  REG_IBRD,
  REG_FBRD,
  REG_LCRH,
  REG_CR,
  REG_IFLS,
  REG_IMSC,
  REG_RIS,
  REG_MIS,
  REG_ICR,
  REG_DMACR,
} uart_reg;

static const u16 pl011_offsets[] = {
  [REG_DR]    = UART_DR,
  [REG_RSR]   = UART_RSR,
  [REG_ESR]   = UART_ESR,
  [REG_FR]    = UART_FR,
  [REG_ILPR]  = UART_ILPR,
  [REG_IBRD]  = UART_IBRD,
  [REG_FBRD]  = UART_FBRD,
  [REG_LCRH]  = UART_LCRH,
  [REG_CR]    = UART_CR,
  [REG_IFLS]  = UART_IFLS,
  [REG_IMSC]  = UART_IMSC,
  [REG_RIS]   = UART_RIS,
  [REG_MIS]   = UART_MIS,
  [REG_ICR]   = UART_ICR,
  [REG_DMACR] = UART_DMACR,
};

#define UART_DR_FE        BIT(8)
#define UART_DR_PE        BIT(9)
#define UART_DR_BE        BIT(10)
#define UART_DR_OE        BIT(11)

// Flag register
#define UART_FR_CTS       BIT(0)
#define UART_FR_DSR       BIT(1)
#define UART_FR_DCD       BIT(2)
#define UART_FR_BUSY      BIT(3)
#define UART_FR_RXFE      BIT(4)
#define UART_FR_TXFF      BIT(5)
#define UART_FR_RXFF      BIT(6)
#define UART_FR_TXFE      BIT(7)
#define UART_FR_RI        BIT(8)

#define UART_LCRH_BRK     BIT(0)
#define UART_LCRH_PEN     BIT(1)
#define UART_LCRH_EPS     BIT(2)
#define UART_LCRH_STP2    BIT(3)
#define UART_LCRH_FEN     BIT(4)
#define UART_LCRH_WLEN5   0x00
#define UART_LCRH_WLEN6   BIT(5)
#define UART_LCRH_WLEN7   BIT(6)
#define UART_LCRH_WLEN8   (BIT(5) | BIT(6))
#define UART_LCRH_SPS     BIT(7)

#define UART_CR_UARTEN BIT(0)
#define UART_CR_LBE    BIT(7)
#define UART_CR_TXE    BIT(8)
#define UART_CR_RXE    BIT(9)

static void * get_register(uart_reg reg) {
  return (void *)(UART_BASE_ADDRES + pl011_offsets[reg]);
}

static void uart_setup() {
  u8 *uart_fr = get_register(REG_FR);
  u8 *uart_lcrh = get_register(REG_LCRH);
  u8 *uart_fbrd = get_register(REG_FBRD);
  u16 *uart_cr = get_register(REG_CR);
  u16 *uart_ibrd = get_register(REG_IBRD);

  // disable uart (just in case)
  *uart_cr = ((~UART_CR_UARTEN) & *uart_cr);

  // uart busy wait
  while (*uart_fr & UART_FR_BUSY) {}

  // flush transmit FIFO
  *uart_lcrh = ((~UART_LCRH_FEN) & *uart_lcrh);
}

static void uart_write_byte(u8 data) {}
static void uart_write(const u8 *data, u32 size) {}

#endif /* UART */
