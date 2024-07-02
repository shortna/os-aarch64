.section .text
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
  bfc x1, 0, 8
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
