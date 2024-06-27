#include "gpio/gpio.h"
#include "gpio/gpio_internal.h"

GPIO gpio(u64 gpio_base) {
  GPIO g = (void*)gpio_base;
  g->IC |= GPIO_INTERRUPTS_CLEAR;
  g->AFSEL = 0;    // enable software mode control
  g->DIR |= 0xff;  // all lines are GPIO_LINE_TYPE_OUT
  return g;
}

// send PRESETn as LOW
void gpio_reset(GPIO g) {
  if (gpio_line_type_get(g, GPIO_LINE_1) != GPIO_LINE_TYPE_IN) {
    gpio_line_type_set(g, GPIO_LINE_1, GPIO_LINE_TYPE_IN);
  }

  gpio_write(g, GPIO_LINE_1, 0);
}

void gpio_write(GPIO g, enum GPIO_LINE line, u32 data) {}
u32 gpio_read(GPIO g, enum GPIO_LINE line) {}
u8 gpio_line_type_get(GPIO g, enum GPIO_LINE line) {}
void gpio_line_type_set(GPIO g, enum GPIO_LINE line, u8 line_type) {}
