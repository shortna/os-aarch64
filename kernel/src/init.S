.section .init
.global _start

_start:
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
  mrs x0, SPSel
  bfc x0, 0, 1 // clears bit [0]
  msr SPSel, x0

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

  // branch to kernel
  bl kmain
  b .

.type getAffinity, @function
.global getAffinity
getAffinity:
  mrs      x0, mpidr_el1
  ubfx     x1, x0, #32, #8
  bfi      w0, w1, #24, #8
  ret

.type setPriorityMask, @function
.global setPriorityMask
setPriorityMask:
  mrs x1, icc_pmr_el1
  orr x1, x1, x0
  msr icc_pmr_el1, x1
  ret

.type enableGroup0Ints, @function
.global enableGroup0Ints
enableGroup0Ints:
  mrs x0, icc_igrpen0_el1
  orr x0, x0, 1
  msr icc_igrpen0_el1, x0
  ret

.type enableGroup1Ints, @function
.global enableGroup1Ints
enableGroup1Ints:
  mrs x0, icc_igrpen1_el1
  orr x0, x0, 1
  msr icc_igrpen1_el1, x0
  ret

.type configureICC, @function
.global configureICC 
configureICC:
  // enable GIC system register interface for EL1
  mrs x0, icc_sre_el1
  bfc x0, 0, 3   // clears bits [0:2]
  orr x0, x0, 1  // set bit [0] to 1
  msr icc_sre_el1, x0

  // bit [0]
  // ICC_BPR0_EL1 determines the preemption group for both Group 0 and Group 1 interrupts.
  // bit [1] 
  // ICC_EOIR0_EL1 and ICC_EOIR1_EL1 provide priority drop functionality only.
  // ICC_DIR_EL1 provides interrupt deactivation functionality.
  // bit [6]
  // disable Priority Mask Hint
  mrs x0, icc_ctlr_el1
  bfc x0, 6, 1  // clears bit [6]
  orr x0, x0, 3 // enables bits [0:1]
  msr icc_ctlr_el1, x0
  ret 
