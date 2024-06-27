#ifndef _UART_H
#define _UART_H

#include "types.h"
#define PRINT(x) uart_write((const u8 *)x, sizeof(x))

typedef void *Uart;
extern Uart uart_setup(u64 uart_base, u64 uart_clock, u64 baud_rate);
extern void uart_write_byte(Uart uart, u8 data);
extern void uart_write(Uart uart, const u8 *data, u32 size);
extern u8 uart_read_byte(Uart uart);
extern u32 uart_read(Uart uart, u8 *data, u32 size);
extern bool uart_data_avaliable(Uart uart);

#endif
