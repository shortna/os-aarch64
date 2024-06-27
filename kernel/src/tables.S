.global vector_table_el1
.section .table_el1,"ax"
/********************/
/* EL1 VECTOR TABLE */
/********************/
.balign 0x800
vector_table_el1:
el1_curr_el_sp0_sync:        
  mov x0, #0xdead
  b .                        
                             
.balign 0x80                 
el1_curr_el_sp0_irq:         
  mov x0, #0xdead
  b .                        
                             
.balign 0x80                 
el1_curr_el_sp0_fiq:         
  mov x0, #0xdead
  b .                        
                             
.balign 0x80                 
el1_curr_el_sp0_serror:      
  mov x0, #0xdead
  b .                        
                             
.balign 0x80                 
el1_curr_el_spx_sync:        
  mov x0, #0xdead
  b .                        
                             
.balign 0x80                 
el1_curr_el_spx_irq:         
  mov x0, #0xdead
  b .                        
                             
.balign 0x80                 
el1_curr_el_spx_fiq:             
  mov x0, #0xdead
  b .                        
                             
.balign 0x80                 
el1_curr_el_spx_serror:      
  mov x0, #0xdead
  b .                        
                             
 .balign 0x80                
el1_lower_el_aarch64_sync:   
  mov x0, #0xdead
  b .                        
                             
.balign 0x80                 
el1_lower_el_aarch64_irq:    
  mov x0, #0xdead
  b .                        
                             
.balign 0x80                 
el1_lower_el_aarch64_fiq:    
  mov x0, #0xdead
  b .                        
                             
.balign 0x80                 
el1_lower_el_aarch64_serror: 
  mov x0, #0xdead
  b .                        
                             
.balign 0x80                 
el1_lower_el_aarch32_sync:   
  mov x0, #0xdead
  b .                        
                             
.balign 0x80                 
el1_lower_el_aarch32_irq:    
  mov x0, #0xdead
  b .                        
                             
.balign 0x80                 
el1_lower_el_aarch32_fiq:    
  mov x0, #0xdead
  b .                        
                             
.balign 0x80                 
el1_lower_el_aarch32_serror: 
  mov x0, #0xdead
  b .                        
