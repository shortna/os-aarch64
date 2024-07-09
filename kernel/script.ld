ENTRY(_start)

MEMORY {
  FLASH (rx)  : ORIGIN = 0x00000000, LENGTH = 0x4000000
  RAM   (rwx) : ORIGIN = 0x40000000, LENGTH = 0x10000
}
SECTIONS
{
  . = ORIGIN(RAM);
  .init : { *(.init) } > RAM
  . = ALIGN(0x800);
  .tables : {*(.table*)} > RAM
  .text : { *(.text) } > RAM
  .data : { *(.data) } > RAM
  .bss  : { *(.bss COMMON) } > RAM
  . = ALIGN(16);

  STACK_SIZE = 0x400;
  . = . + STACK_SIZE;
  STACK_TOP = .;

  FREE_MEMORY = .;
  FREE_MEMORY_SIZE = (ORIGIN(RAM) + LENGTH(RAM)) - FREE_MEMORY;
}
