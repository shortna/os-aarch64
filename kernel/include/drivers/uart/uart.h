#ifndef _UART_H
#define _UART_H

#include "types.h"

struct UART;
typedef struct UART *UART;

enum UART_INT_TYPE {
  UART_INT_RIMIM = 0,
  UART_INT_CTSMI = 1,
  UART_INT_DCDMI = 2,
  UART_INT_DSRMI = 3,
  UART_INT_RXIM = 4,
  UART_INT_TXIM = 5,
  UART_INT_RTIM = 6,
  UART_INT_FEIM = 7,
  UART_INT_PEIM = 8,
  UART_INT_BEIM = 9,
  UART_INT_OEIM = 10,
  UART_INT_NONE = 11,
};

extern UART uart_init(u64 uart_base);

extern void uart_enable(UART u);
extern void uart_disable(UART u);

extern enum UART_INT_TYPE uart_get_interrupt(UART u);
extern void uart_enable_interrupt(UART u, enum UART_INT_TYPE t);
extern void uart_disable_interrupt(UART u, enum UART_INT_TYPE t);
extern void uart_clear_interrupt(UART u, enum UART_INT_TYPE t);

extern void uart_set_baud_rate(UART u, u64 uart_clock, u64 baud_rate);

extern void uart_write_byte(UART u, u8 data);
extern void uart_write(UART u, const u8 *data, u32 size);

#endif
