#ifndef _UART_H
#define _UART_H

#include "types.h"
#define PRINT(x) uart_write((const u8 *)x, sizeof(x))

struct pl011 {
  u64 base_address;
  u64 clock;
  u64 baud_rate;
};

extern void uart_setup(struct pl011 *p);
extern void uart_write_byte(u8 data);
extern void uart_write(const u8 *data, u32 size);
extern u8 uart_read_byte(void);
extern u32 uart_read(u8 *data, u32 size);
extern bool uart_data_avaliable(void);

#endif
