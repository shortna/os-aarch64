#define STACK_FRAME (16)
.section .text
.type int_id_size, @function
int_id_size:
  mrs x1, icc_ctlr_el1 
  ubfx x0, x1, 11, 3 // extract size and put to start of x0
  cmp x0, 0          // check if 16 bits
  b.ne 0f            // if  not 24
  mov x0, 16
  b 1f               // jump to return
0:
  mov x0, 24
1:
  ret

.global get_int_id
.type get_int_id, @function
get_int_id:
  // save return address
  str lr, [sp, -STACK_FRAME]!

  // get size of int id in system
  bl int_id_size
  // get register value which contains int id
  mrs x1, ICC_HPPIR1_EL1

  // build mask
  mov x2, 1
  lsl x2, x2, x0 // shift 1 by size of int id
  sub x2, x2, 1  // sub 1 to get mask

  // get int id
  and x0, x1, x2

  // restore return address
  ldr lr, [sp], STACK_FRAME
  // return
  ret

.global vector_table_el1
.section .table_el1,"ax"
/********************/
/* EL1 VECTOR TABLE */
/********************/
.balign 0x800
vector_table_el1:
el1_curr_el_sp0_sync:
  b .

.balign 0x80
el1_curr_el_sp0_irq:
  // for some reason current sp sets to higher EL
  // bring it back
  bl set_sp_sel 
  // save registers
  stp x0, x1,   [sp, -STACK_FRAME]!
  stp x2, x3,   [sp, -STACK_FRAME]!
  stp x4, x5,   [sp, -STACK_FRAME]!
  stp x6, x7,   [sp, -STACK_FRAME]!
  stp x8, x9,   [sp, -STACK_FRAME]!
  stp x10, x11, [sp, -STACK_FRAME]!
  stp x12, x13, [sp, -STACK_FRAME]!
  stp x14, x15, [sp, -STACK_FRAME]!

  bl handle

  // restore registers
  ldp x14, x15, [sp], STACK_FRAME
  ldp x12, x13, [sp], STACK_FRAME
  ldp x10, x11, [sp], STACK_FRAME
  ldp x8, x9,   [sp], STACK_FRAME
  ldp x6, x7,   [sp], STACK_FRAME
  ldp x4, x5,   [sp], STACK_FRAME
  ldp x2, x3,   [sp], STACK_FRAME
  ldp x0, x1,   [sp], STACK_FRAME
  eret

.balign 0x80
el1_curr_el_sp0_fiq:
  b .

.balign 0x80
el1_curr_el_sp0_serror:
  b .

.balign 0x80
el1_curr_el_spx_sync:
  b .

.balign 0x80
el1_curr_el_spx_irq:
  b .

.balign 0x80
el1_curr_el_spx_fiq:
  b .

.balign 0x80
el1_curr_el_spx_serror:
  b .

.balign 0x80
el1_lower_el_aarch64_sync:
  b .

.balign 0x80
el1_lower_el_aarch64_irq:
  b .

.balign 0x80
el1_lower_el_aarch64_fiq:
  b .

.balign 0x80
el1_lower_el_aarch64_serror:
  b .

.balign 0x80
el1_lower_el_aarch32_sync:
  b .

.balign 0x80
el1_lower_el_aarch32_irq:
  b .

.balign 0x80
el1_lower_el_aarch32_fiq:
  b .

.balign 0x80
el1_lower_el_aarch32_serror:
  b .
