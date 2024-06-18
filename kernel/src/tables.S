.global vector_table_el1
.section .table_el1
/********************/
/* EL1 VECTOR TABLE */
/********************/
.balign 0x800
vector_table_el1:
el1_curr_el_sp0_sync:        
  b .                        
                             
.balign 0x80                 
el1_curr_el_sp0_irq:         
  b .                        
                             
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
