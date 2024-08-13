.section .init,"ax"
.global _start

_start:
  // save fdt base
  mov x2, x0

  // read sctlr_el1
  mrs x0, SCTLR_EL1
  // bit [0] - when 0 mmu disabled = 0
  // bits [1] [3] [4] - alignment checks = 1
  // bit [12] - Stage 1 instruction access Cacheability control, for accesses at EL0 and EL1 = 1
  // bit [24] - Endianness of data accesses at EL0 = 0 (liitle endian)
  // bit [25] - Endianness of data accesses at EL1, and stage 1 translation table walks in the EL1&0 translation regime = 0(little endian)

  movn x1, 0x1001         // move mask 0xeffe to first  16 bits
  movn x1, 0x0300, lsl 16 // move mask 0xfcff to second 16 bits

  // fill upper 32 bits with 1`s
  movn x1, 0, lsl 32
  movn x1, 0, lsl 48

 // disable bits [0] [12] [24] [25]
  and x0, x0, x1 

  // enable bits [1] [3] [4]
  mov x1, 0x001A
  orr x0, x0, x1

  // write sctlr_el1
  msr SCTLR_EL1, x0

  // check CurrentEL
  mrs x0, CurrentEL
  ubfx x1, x0, 2, 2 
  // if not EL1, hang
  cmp x1, 1
  b.ne .

  // set base address of EVT
  adr x0, vector_table_el1
  msr VBAR_EL1, x0

  // Use SP_EL0 at all Exception levels.
  msr SPSel, 0

  // intialize stack
  // STACK_TOP defined in linker script
  ldr x0, =STACK_TOP  // x1 = stack_top
  mov sp, x0          // sp = x1

  // enable floating points
  mrs x0, cpacr_el1
  orr x0, x0, (3 << 20)
  msr CPACR_EL1, x0

  // Ensure changes to system register are visible
  isb

  // unmask IRQ and FIQ in DAIF register
  msr DAIFClr, 0x3

  // restore fdt base, and pass as first argument to kmain
  mov x0, x2
  // branch to kernel
  bl kmain
  b .
