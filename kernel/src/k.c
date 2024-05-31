#include "types.h"
// deduced from dump of device tree blob (dtb)
volatile u64 *UART0 = (u64 *)0x9000000;

u8 print(const char *format) {
  while (*format) {
    *UART0 = *format;
    format++;
  }
  return 1;
}

void kmain() {
  while (1) {
    print("Hello, World!\n");
  };
}
