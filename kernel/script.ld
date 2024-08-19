ENTRY(_start)

/* FLASH0 from 0x0 to 0x100000 reserved for fdt */
MEMORY {
  FLASH0 (rx)  : ORIGIN = 0x00000000, LENGTH = 0x04000000
  FLASH1 (rx)  : ORIGIN = 0x04000000, LENGTH = 0x04000000
  RAM    (rwx) : ORIGIN = 0x40000000, LENGTH = 0x40000000 /* 1GB */
}
SECTIONS
{
  .rodata : { *(.rodata) } > FLASH1

  .init ALIGN(0x8) : { *(.init) } > FLASH1
  .text ALIGN(0x8): { *(.text) } > FLASH1
  .exception_table_el1 ALIGN(0x800) : { *(.exception_table_el1) } > FLASH1

  /* FLASH STUFF */
  .data ALIGN(0x8): { *(.data) } > KERNEL_DATA;

  . = ALIGN(0x10);
  STACK_TOP = . + 0x400;
  . = STACK_TOP;
  FREE_MEMORY = .;
  MEMORY_SIZE = 0x0;
  MEMORY_START = 0x0;
}
