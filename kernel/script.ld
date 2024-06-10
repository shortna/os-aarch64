ENTRY(_start)
SECTIONS
{
  STACK_SIZE = 0x1000; /* 4Kb */
  RAM_START = 0x40000000;
  . = RAM_START + STACK_SIZE;
  STACK_TOP = .;

  .init : {objects/init.S.o(.text)}
  .text : { *(.text) }
  .data : { *(.data) }
  .bss : { *(.bss COMMON) }
  . = ALIGN(8);   /* in case previous data ended on not aligned address */
  FREE_MEMORY = .;
}
