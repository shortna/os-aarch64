#ifndef _UART_H
#define _UART_H

#include "types.h"
#define PRINT(x) uart_write((const u8 *)x, sizeof(x))

struct UART;
typedef struct UART *UART;

extern void uart_transmit(UART u);
extern UART uart_setup(u64 uart_base, u64 uart_clock, u64 baud_rate);
extern void uart_write_byte(UART u, u8 data);
extern void uart_write(UART u, const u8 *data, u32 size);
extern u8 uart_read_byte(UART u);
extern u32 uart_read(UART u, u8 *data, u32 size);
extern bool uart_data_avaliable(UART u);

#endif
