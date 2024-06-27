#ifndef _GPIO_H
#define _GPIO_H

#include "types.h"

struct GPIO;
typedef struct GPIO *GPIO;

enum GPIO_LINE {
  GPIO_LINE_0 = BIT(0),
  GPIO_LINE_1 = BIT(1),
  GPIO_LINE_2 = BIT(2),
  GPIO_LINE_3 = BIT(3),
  GPIO_LINE_4 = BIT(4),
  GPIO_LINE_5 = BIT(5),
  GPIO_LINE_6 = BIT(6),
  GPIO_LINE_7 = BIT(7),
};

#define GPIO_LINE_TYPE_IN  (0)
#define GPIO_LINE_TYPE_OUT (1)

// clear gpio interrupt and return interface for gpio
extern GPIO gpio(u64 gpio_base);

// fully resets gpio
extern void gpio_reset(GPIO g);

// write data
extern void gpio_write(GPIO g, enum GPIO_LINE line, u32 data);

// read data
extern u32 gpio_read(GPIO g, enum GPIO_LINE line);

// returns type of `line` `0 - in`, `1 - out`
extern u8 gpio_line_type_get(GPIO g, enum GPIO_LINE line);

/* 
 * sets `line_type` to `line` 
 * `GPIO_LINE_TYPE_IN (0) - in`, `GPIO_LINE_TYPE_OUT (1) - out`
 */
extern void gpio_line_type_set(GPIO g, enum GPIO_LINE line, u8 line_type);

#endif
