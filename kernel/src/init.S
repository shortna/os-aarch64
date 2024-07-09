.section .init,"ax"
.global _start

_start:
  // save fdt base
  mov x2, x0
  // check CurrentEL
  mrs x0, CurrentEL
  ubfx x1, x0, 2, 2 
  // if not EL1, hang
  cmp x1, 1
  b.ne .

  // set base address of EVT
  adr x0, vector_table_el1
  msr vbar_el1, x0

  // Use SP_EL0 at all Exception levels.
  msr SPSel, 0

  // intialize stack
  // STACK_TOP defined in linker script
  ldr x0, =STACK_TOP  // x1 = stack_top
  mov sp, x0          // sp = x1

  // enable floating points
  mrs x0, cpacr_el1
  orr x0, x0, (3 << 20)
  msr cpacr_el1, x0

  // Ensure changes to system register are visible
  isb

  // unmask IRQ and FIQ in DAIF register
  msr DAIFClr, 0x3

  // restore fdt base, and pass as first argument to kmain
  mov x0, x2
  // branch to kernel
  bl kmain
  b .
