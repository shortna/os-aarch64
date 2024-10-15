ENTRY(_start)

MEMORY {
  /* FLASH from 0x0 to 0x100000 reserved for fdt */
  FLASH  (rx)  : ORIGIN = 0x00100000, LENGTH = 0x07F00000
  RAM    (rwx) : ORIGIN = 0x40000000, LENGTH = 0x40000000 /* 1GB */
}
SECTIONS
{
  .rodata              ALIGN(0x8)    : { *(.rodata)              } > FLASH
  .init                ALIGN(0x8)    : { *(.init)                } > FLASH
  .text                ALIGN(0x8)    : { *(.text)                } > FLASH
  .exception_table_el1 ALIGN(0x800)  : { *(.exception_table_el1) } > FLASH
  .data                ALIGN(0x8)    : { *(.data)                } > RAM
  .bss                 ALIGN(0x8)    : { *(.bss)                 } > RAM
  . = ALIGN(0x10);
  STACK_TOP = . + 0x400;
  . = STACK_TOP;
  FREE_SPACE = .;
}

