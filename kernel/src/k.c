#include "fdt.h"
#include "memory.h"
#include "string.h"
#include "uart/uart.h"

void kmain() {
  void *fdt;

  // same as u8 *fdt = (u8*)*(u64*)(STACK_TOP - 0x10);
  __asm__("adr x0, STACK_TOP\n\t"
          "ldr %0,[x0,-0x10]\n\t"
          : "=r"(fdt));

  // if fdt is not valid 
  // hang
  if (!fdt_valid_header(fdt)) {
    while (1);
  }

  struct pl011 p;
  struct FDTNode node;
  u8 *keys[] = {(u8 *)"memory", (u8 *)"pl011", (u8 *)"apb-pclk"};
  while (fdt_read_node(fdt, &node)) {
    for (u8 i = 0; i < sizeof(keys) / sizeof(*keys); i++) {
      if (memcmp(node.name, keys[i], strlen(keys[i]))) {
        u64 mem_addr;
        struct FDTProperty property;
        switch (i) {
        case 0:
          fdt_get_node_address(&node, &mem_addr);
          set_memory_address(mem_addr);
          break;
        case 1:
        case 2:
        }
      }
    }
  }

  while (1);
}
