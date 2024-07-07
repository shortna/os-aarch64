ENTRY(_start)

MEMORY {
  FLASH (rx)  : ORIGIN = 0x00000000, LENGTH = 0x4000000
  RAM   (rwx) : ORIGIN = 0x40000000, LENGTH = 0x10000
}
SECTIONS
{
  STACK_FRAME_SIZE = 16;
  .init : { *(.init) } > RAM
  . = ALIGN(0x800);
  .tables : {*(.table*)} > RAM
  .text : { *(.text) } > RAM
  .data : { *(.data) } > RAM
  .bss  : { *(.bss COMMON) } > RAM
  . = ALIGN(STACK_FRAME_SIZE);

  STACK_SIZE = 0x400;
  . = . + STACK_SIZE;
  STACK_TOP = .;
}
