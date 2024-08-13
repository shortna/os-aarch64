ENTRY(_start)

/* ORIGIN(FLASH) offsetted by 0x100000 cause addresses from 0x0 to 0x100000 reserved for dtb */
MEMORY {
  FLASH (rx)  : ORIGIN = 0x00100000, LENGTH = 0x3f00000
  RAM   (rwx) : ORIGIN = 0x40000000, LENGTH = 0x40000000 /* 1GB */
}
SECTIONS
{
  .rodata : { *(.rodata) } > FLASH

  .init ALIGN(0x8) : { *(.init) } > FLASH
  .text ALIGN(0x8): { *(.text) } > FLASH
  .exception_table_el1 ALIGN(0x800) : { *(.exception_table_el1) } > FLASH

  .data ALIGN(0x8): { *(.data) } > RAM

  . = ALIGN(0x10);
  STACK_TOP = . + 0x400;
  . = STACK_TOP;
  FREE_MEMORY = .;
}
