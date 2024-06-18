#ifndef _UART_DEFS_H
#define _UART_DEFS_H

// pl011 - Peripheral Controller
// address and clock deduced from dump of device tree blob (dtb)
#define UART_CLOCK U64(0x16e3600)

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

// IFLS
#define UART_IFLS_RX_1_8  0x0
#define UART_IFLS_RX_1_4  (0x1 << 3)
#define UART_IFLS_RX_1_2  (0x2 << 3)
#define UART_IFLS_RX_3_4  (0x3 << 3)
#define UART_IFLS_RX_7_8  (0x4 << 3)

#define UART_IFLS_TX_1_8  0x0
#define UART_IFLS_TX_1_4  0x1
#define UART_IFLS_TX_1_2  0x2
#define UART_IFLS_TX_3_4  0x3
#define UART_IFLS_TX_7_8  0x4

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

#endif
