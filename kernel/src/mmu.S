.section .text
.set GRANULARITY_4KB_NOT_SUPPORTED,  0xf
.set GRANULARITY_16KB_NOT_SUPPORTED, 0x0
.set GRANULARITY_64KB_NOT_SUPPORTED, 0xf

mmu_init_err:
ldr lr, [sp, -0x8]
add sp, sp, 0x10
mov x0, 0
ret

.global mmu_init
// bool mmu_init(enum GRANULARITY_TYPE granularity);
// u8 granularity, allowed values are 4, 16, 64
// granularity passed in x0
.type mmu_init, @function
mmu_init:
// store granularity and return address 
mov x1, x0
mov x6, lr

// get supported granularity
// result of mmu_supported_granule_size_stage1 is supported granularities at bits [0] - 4KB [16] - 64KB [32] - 16KB
bl mmu_supported_granule_size_stage1

// shift
mov x3, 0 
// loop
// x0 - return from mmu_supported_granule_size_stage1
// x2 - copy of granularity
// x3 - shift
0:
mov x2, x1     // desired granularity value
lsr x0, x0, x3 // get next granularity
and x2, x2, x0 // check if supported
add x3, x3, 16 // add shift for next iteration
cmp x3, 48     // if shift 48, desired granularity not supported
b.eq mmu_init_err
// compare to zero and branch to 0
cbz x2, 0b

/*{{{
// bits [21:16] - T1SZ                                = x2
// bit  [22]    - TTBR1_EL1 defines ASID              = 1
// bit  [23]    - enable translation table walk       = 0
// bits [25:24] - Normal memory, Inner Non-cacheable. = 0
// bits [27:26] - Normal memory, Outer Non-cacheable. = 0
// bits [29:28] - Shareability                        = 0
// bits [31:30] - granularity                         = x1
// bits [34:32] - Intermediate Physical Address Size  = x0
// bit  [36]    - ASID size, 8bit.                    = 0
// bit  [38]    - Top Byte ignored.                   = 0
// When FEAT_HPDS is implemented, explicitly set 0
// bit  [42]    - HPD1                                = 0

bl mmu_implemented_parange
mov x2, 32

mrs x3, TCR_EL1
// set bits described above, except [42]
movz x4, 0xFFFF, lsl 16
movk x4, 0x0057, lsl 32

// check if Hierarchical Permission for tables implemented
mrs x5, ID_AA64MMFR1_EL1
ubfx x5, x5, 12, 4
cmp xzr, x5
// branch if not implemented
b.ne 0f

// clear bit [42] 
mov x5, 0
movn x5, 0x0400, lsl 32
and x3, x3, x5

0:
// negate to get mask that clears all set bits
mvn x4, x4
and x3, x3, x4

// set relevant bits
orr x3, x3, x2, lsl 16 // set tsz
orr x3, x3, 0x400000   // set bits [22]
lsl x1, x1, 30         // shift granularity to positon
orr x1, x1, x0, lsl 32 // insert address size to position
orr x3, x3, x1         // insert granularity and address size

msr TCR_EL1, x3
*///}}}

// bits [5:0]                                      - T0SZ                               = x2{{{
// bit  [7]                                        - EPD0                               = 0
// bits [9:8]                                      - Normal memory, Inner Non cacheable = 0
// bits [11:10]                                    - Normal memory, Outer Non cacheable = 0
// bits [13:12]                                    - Shareability                       = 0
// bits [15:14]                                    - TG0                                = x1
// bit  [22]                                       - TTBR0_EL1 defines ASID             = 0
// bits [34:32]                                    - Intermediate Physical Address Size = x0
// bit  [36]                                       - ASID size, 8bit.                   = 0
// bit  [38]                                       - Top Byte ignored.                  = 0
// When FEAT_HPDS is implemented, explicitly set 0
// bit  [41]                                       - HPD1                               = 0

bl mmu_implemented_parange
// switch
adr x3, . + 12        // load address of IPS_4GB
add x3, x3, x0, lsl 3 // add offset based on x0 value
br x3

IPS_4GB:
mov x3, 32
b IPS_END 
IPS_64GB:
mov x3, 36
b IPS_END 
IPS_1TB:
mov x3, 40
b IPS_END 
IPS_4TB:
mov x3, 42
b IPS_END 
IPS_16TB:
mov x3, 44
b IPS_END 
IPS_256TB:
mov x3, 48
b IPS_END 
IPS_4PB:
mov x3, 52
b IPS_END 
IPS_64PB:
mov x3, 56

IPS_END:
mov x2, 64
sub x2, x2, x3

mrs x3, TCR_EL1

movz x4, 0xffbf
movk x4, 0x0400, lsl 16
movk x4, 0x0057, lsl 32

// check if Hierarchical Permission for tables implemented
mrs x5, ID_AA64MMFR1_EL1
ubfx x5, x5, 12, 4
cmp xzr, x5
// branch if not implemented
b.ne 0f

// clear bit [41] 
mov x5, 0
movn x5, 0x0200, lsl 32
and x3, x3, x5

0:
// negate to get mask that clears all set bits
mvn x4, x4
and x3, x3, x4

// T0SZ
orr x3, x3, x2
// TG0
orr x3, x3, x1, lsl 14
// IPA
orr x3, x3, x0, lsl 32
msr TCR_EL1, x3//}}}

// pass granularity and implemented PA, PA - x0, GRANULARITY - x1
bl mmu_init_translation_tables

mov lr, x6
mov x0, 1
ret

// .set OFFSET, 0XFFFFFFFF00000000 // start of TTBR1_EL1 address space
.global mmu_enable
// void mmu_enable(void);
.type mmu_enable, @function
mmu_enable:
// save return
mov x0, lr
bl mmu_write_memory_attribute
mov lr, x0

// clear all tlb caches
tlbi VMALLE1
dsb SY
isb

//ldr x0, =OFFSET
//add lr, lr, x0
//add sp, sp, x0
//
//mrs x1, VBAR_EL1
//add x1, x1, x0
//msr VBAR_EL1, x1

mrs x0, SCTLR_EL1
mov x1, 0x1       // bit mask
orr x0, x0, x1    // enale cacheability and mmu bits [2] and [0]
msr SCTLR_EL1, x0
// apply changes
isb

ret

mmu_maximum_value_of_TSZ:
// reads ST field form register
mrs x0, ID_AA64MMFR2_EL1
// extract bits [31:28]
ubfx x0, x0, 28, 4
tbz x0, 0, 0f // test if bit [0] is = 0 and jump forward to mark 0, if true
mov x0, 48
ret
0:
mov x0, 39
ret

mmu_implemented_parange:
mrs x0, ID_AA64MMFR0_EL1
ubfx x0, x0, 0, 4
ret

mmu_supported_granule_size_stage1:
// save registers
stp xzr, x1, [sp, -0x10]!
stp x2, x3, [sp, -0x10]!

mov x0, 0 // initialize x0 to 0
mrs x2, ID_AA64MMFR0_EL1 // read system register

ubfx x1, x2, 28, 4 // extract bits [31:28]
cmp x1, GRANULARITY_4KB_NOT_SUPPORTED // check if granularity supported
b.eq 0f // if not, jump forward to mark 0
orr x0, x0, 0x2 // else, set bit [1]

0:
ubfx x1, x2, 24, 4 // extract bits [27:24]
cmp x1, GRANULARITY_64KB_NOT_SUPPORTED // check if granularity supported
b.eq 0f // if not, jump forward to mark 0
mov x3, 0x3
orr x0, x0, x3, lsl 16 // else, set bits [17:16]

0:
ubfx x1, x2, 20, 4 // extract bits [23:20]
cmp x1, GRANULARITY_16KB_NOT_SUPPORTED // check if granularity supported
b.eq 0f // if not, jump forward to mark 0
mov x3, 0x1
orr x0, x0, x3, lsl 32 // else, set bit [32]

0:
// restore registers
ldp x2, x3, [sp], +0x10
ldp xzr, x1, [sp], +0x10
ret

mmu_write_memory_attribute:
stp x0, x1, [sp, -0x10]!
mrs x0, MAIR_EL1

movn x1, 0xffff
and x0, x0, x1
orr x0, x0, 0xff00

msr MAIR_EL1, x0
ldp x0, x1, [sp], +0x10
ret

// void mmu_read_ttbrN_el1(void)
// void mmu_write_ttbrN_el1(u64 v)
.global mmu_write_ttbr0_el1
.type mmu_write_ttbr0_el1, @function
mmu_write_ttbr0_el1:
msr TTBR0_EL1, x0
ret

.global mmu_write_ttbr1_el1
.type mmu_write_ttbr1_el1, @function
mmu_write_ttbr1_el1:
msr TTBR1_EL1, x0
ret

.global mmu_read_ttbr0_el1
.type mmu_read_ttbr0_el1, @function
mmu_read_ttbr0_el1:
mrs x0, TTBR0_EL1
ret

.global mmu_read_ttbr1_el1
.type mmu_read_ttbr1_el1, @function
mmu_read_ttbr1_el1:
mrs x0, TTBR1_EL1
ret
