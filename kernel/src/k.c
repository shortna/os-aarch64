#include "uart.h"

u64 RAM_START;

void kmain() {
  uart_setup();
  u8 *b = (void*)0x40000F00;
  u8 c = 'A';
  for (u32 i = 0; i < b - (u8*)RAM_START; i++) {
    b[i] = c;
    if (i % 0x10) {
      c++;
    }
  }

  while (1) {
    uart_write(b, sizeof(b));
  };
}
