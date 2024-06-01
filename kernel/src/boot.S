// KERNEL ENTRY CODE
.section .text
.global _start

// Thanks to qemu -kernel option, we already start at el1
_start:
  /* intialize stack */
  // STACK_TOP and STACK_SIZE defined in linker script
  ldr x1, =STACK_TOP  // x0 = stack_top
  mov sp, x1          // sp = x0

  // set base address of EVT
  adr x1, vector_table_el1
  msr vbar_el1, x1

  bl kmain
  b .

/********************/
/* EL1 VECTOR TABLE */
/********************/
.balign 0x800
vector_table_el1:
el1_curr_el_sp0_sync:        // The exception handler for a synchronous 
                             // exception from the current EL using SP0.
  mov w9, 'Q'
  b print_char

.balign 0x80
el1_curr_el_sp0_irq:         // The exception handler for an IRQ exception
                             // from the current EL using SP0.
  mov w9, 'R'
  b print_char

.balign 0x80
el1_curr_el_sp0_fiq:         // The exception handler for an FIQ exception
                             // from the current EL using SP0.
  mov w9, 'S'
  b print_char

.balign 0x80
el1_curr_el_sp0_serror:      // The exception handler for a System Error 
                             // exception from the current EL using SP0.
  mov w9, 'T'
  b print_char

.balign 0x80
el1_curr_el_spx_sync:        // The exception handler for a synchrous 
                             // exception from the current EL using the
                             // current SP.
  mov w9, 'U'
  b print_char

.balign 0x80
el1_curr_el_spx_irq:         // The exception handler for an IRQ exception from 
                             // the current EL using the current SP.
  mov w9, 'V'
  b print_char

.balign 0x80
curr_el_spx_fiq:             // The exception handler for an FIQ from 
                             // the current EL using the current SP.
  mov w9, 'W'
  b print_char

.balign 0x80
el1_curr_el_spx_serror:      // The exception handler for a System Error 
                             // exception from the current EL using the
                             // current SP.
  mov w9, 'X'
  b print_char

 .balign 0x80
el1_lower_el_aarch64_sync:   // The exception handler for a synchronous 
                             // exception from a lower EL (AArch64).
  mov w9, 'Y'
  b print_char

.balign 0x80
el1_lower_el_aarch64_irq:    // The exception handler for an IRQ from a lower EL
                             // (AArch64).
  mov w9, 'Z'
  b print_char

.balign 0x80
el1_lower_el_aarch64_fiq:    // The exception handler for an FIQ from a lower EL
                             // (AArch64).
  mov w9, '1'
  b print_char

.balign 0x80
el1_lower_el_aarch64_serror: // The exception handler for a System Error 
                             // exception from a lower EL(AArch64).
  mov w9, '2'
  b print_char

.balign 0x80
el1_lower_el_aarch32_sync:   // The exception handler for a synchronous 
                             // exception from a lower EL(AArch32).
  mov w9, '3'
  b print_char

.balign 0x80
el1_lower_el_aarch32_irq:    // The exception handler for an IRQ exception 
                             // from a lower EL (AArch32).
  mov w9, '4'
  b print_char

.balign 0x80
el1_lower_el_aarch32_fiq:    // The exception handler for an FIQ exception from 
                             // a lower EL (AArch32).
  mov w9, '5'
  b print_char

.balign 0x80
el1_lower_el_aarch32_serror: // The exception handler for a System Error
                             // exception from a lower EL(AArch32).
  mov w9, '6'
  b print_char

print_char:
  ldr x1, =0x9000000
  strb w9, [x1]
  b .

