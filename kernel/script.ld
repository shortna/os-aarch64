ENTRY(_start)

MEMORY {
  FLASH (rx)  : ORIGIN = 0x00000000, LENGTH = 0x4000000
  RAM   (rwx) : ORIGIN = 0x40000000, LENGTH = 0x10000
}
SECTIONS
{
  STACK_SIZE = 0x400;
  . = ORIGIN(FLASH) + STACK_SIZE;
  STACK_TOP = .;
 
  .init : { *(.init) } > RAM
  .text : { *(.text) } > RAM
  .data : { *(.data) } > RAM
  .bss  : { *(.bss COMMON) } > RAM
  . = ALIGN(8);
}
