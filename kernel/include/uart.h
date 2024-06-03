#ifndef UART
#define UART

#include "types.h"

// pl011 - Peripheral Controller
// address deduced from dump of device tree blob (dtb)
#define UART_BAUD_RATE   115200ull
#define UART_BASE_ADDRES 0x9000000ull
#define UART_CLOCK       24000000ull

/* pl011 UART register offset table */
#define UART_DR      0x00
#define UART_RSR     0x04
#define UART_ESR     0x04
#define UART_FR      0x18
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

// Data register
#define UART_DR_FE        BIT(8)
#define UART_DR_PE        BIT(9)
#define UART_DR_BE        BIT(10)
#define UART_DR_OE        BIT(11)

// RSR,ESR
#define UART_RSR_FE        BIT(0)
#define UART_RSR_PE        BIT(1)
#define UART_RSR_BE        BIT(2)
#define UART_RSR_OE        BIT(3)

#define UART_ESR_FE        BIT(0)
#define UART_ESR_PE        BIT(1)
#define UART_ESR_BE        BIT(2)
#define UART_ESR_OE        BIT(3)

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

// Line control register high
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

// Control register
#define UART_CR_UARTEN    BIT(0)
#define UART_CR_SIREN     BIT(1)
#define UART_CR_SIRLP     BIT(2)
#define UART_CR_LBE       BIT(7)
#define UART_CR_TXE       BIT(8)
#define UART_CR_RXE       BIT(9)
#define UART_CR_DTR       BIT(10)
#define UART_CR_RTS       BIT(11)
#define UART_CR_OUT1      BIT(12)
#define UART_CR_OUT2      BIT(13)
#define UART_CR_RTSEn     BIT(14)
#define UART_CR_CTSEn     BIT(15)

// IMSC
#define UART_IMSC_RIMIM   BIT(0)
#define UART_IMSC_CTSMIM  BIT(1)
#define UART_IMSC_DCDMIM  BIT(2)
#define UART_IMSC_DSRMIM  BIT(3)
#define UART_IMSC_RXIM    BIT(4)
#define UART_IMSC_TXIM    BIT(5)
#define UART_IMSC_RTIM    BIT(6)
#define UART_IMSC_FEIM    BIT(7)
#define UART_IMSC_PEIM    BIT(8)
#define UART_IMSC_BEIM    BIT(9)
#define UART_IMSC_OEIM    BIT(10)

// RIS
#define UART_RIS_RIRMIS   BIT(0)
#define UART_RIS_CTSRMIS  BIT(1)
#define UART_RIS_DCDRMIS  BIT(2)
#define UART_RIS_DSRRMIS  BIT(3)
#define UART_RIS_RXRIS    BIT(4)
#define UART_RIS_TXRIS    BIT(5)
#define UART_RIS_RTRIS    BIT(6)
#define UART_RIS_FERIS    BIT(7)
#define UART_RIS_PERIS    BIT(8)
#define UART_RIS_BERIS    BIT(9)
#define UART_RIS_OERIS    BIT(10)

// MIS 
#define UART_MIS_RIMMIS   BIT(0)
#define UART_MIS_CTSMMIS  BIT(1)
#define UART_MIS_DCDMMIS  BIT(2)
#define UART_MIS_DSRMMIS  BIT(3)
#define UART_MIS_RXMIS    BIT(4)
#define UART_MIS_TXMIS    BIT(5)
#define UART_MIS_RTMIS    BIT(6)
#define UART_MIS_FEMIS    BIT(7)
#define UART_MIS_PEMIS    BIT(8)
#define UART_MIS_BEMIS    BIT(9)
#define UART_MIS_OEMIS    BIT(10)

// ICR
#define UART_ICR_RIMIC    BIT(0)
#define UART_ICR_CTSMIC   BIT(1)
#define UART_ICR_DCDMIC   BIT(2)
#define UART_ICR_DSRMIC   BIT(3)
#define UART_ICR_RXIC     BIT(4)
#define UART_ICR_TXIC     BIT(5)
#define UART_ICR_RTIC     BIT(6)
#define UART_ICR_FEIC     BIT(7)
#define UART_ICR_PEIC     BIT(8)
#define UART_ICR_BEIC     BIT(9)
#define UART_ICR_OEIC     BIT(10)

// DMACR
#define UART_DMACR_RXDMAE   BIT(0)
#define UART_DMACR_TXDMAE   BIT(1)
#define UART_DMACR_DMAONERR BIT(2)

inline static void *get_register(uart_reg reg) {
  return (void *)(UART_BASE_ADDRES + pl011_offsets[reg]);
}

static void uart_setup() {
  u16 *uart_cr = get_register(REG_CR);
  u16 *uart_ibrd = get_register(REG_IBRD);
  u8 *uart_fr = get_register(REG_FR);
  u8 *uart_lcrh = get_register(REG_LCRH);
  u8 *uart_fbrd = get_register(REG_FBRD);

  // disable uart (just in case)
  *uart_cr = (~UART_CR_UARTEN & *uart_cr);

  // uart busy wait
  while (UART_FR_BUSY & *uart_fr) {}

  // setting baud_rate
  const float baud_rate = (float)UART_CLOCK / (16 * UART_BAUD_RATE);
  *uart_ibrd = (u16)baud_rate; // store int part
                               
  // fbrd is only 6 bits
  // set all fbrd bits to 0 and leave last 2 bits untouched 
  *uart_fbrd &= 3; 

  // mask extracts mantisa from baud_rate
  // fbrd only 6 bits so shift by 
  u32 l = GENMASK(0, 23) & *(u32*)&baud_rate;

  // flush transmit FIFO
  *uart_lcrh = (~UART_LCRH_FEN & *uart_lcrh);
}

static void uart_write_byte(u8 data) {}
static void uart_write(const u8 *data, u32 size) {}

#endif /* UART */
