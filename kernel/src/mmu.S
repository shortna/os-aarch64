.section .text
.set GRANULARITY_4KB_NOT_SUPPORTED,  0xf
.set GRANULARITY_16KB_NOT_SUPPORTED, 0x0
.set GRANULARITY_64KB_NOT_SUPPORTED, 0xf

mmu_init_err:
ldr lr, [sp, -0x8]
add sp, sp, 0x10
mov x0, 0
ret

.global mmu_init_kernel
// bool mmu_init_kernel(u8 address_space_n_bits, enum GRANULARITY_TYPE granularity);
// args
// x0 = u8 address_space_n_bits
// x1 = u8 granularity, allowed values are 4, 16, 64
.type mmu_init_kernel, @function
mmu_init_kernel:
mov x2, x0 // x2 = x0
           // store return address 
stp xzr, lr, [sp, -0x10]!
// get maximum value of TSZ in x0
bl mmu_maximum_value_of_TSZ

// calculate minimal size of adress space 
mov x3, 64
sub x0, x3, x0

// if address_space_n_bits is less then min than jump to err
cmp x2, x0
b.lt mmu_init_err

// get supported granularity
bl mmu_supported_granule_size_stage1
// if granularity provided by caller supported, register x2 will be greater than zero

// result of mmu_supported_granule_size_stage1 is supported granularities at bits [0] - 4KB [16] - 64KB [32] - 16KB
mov x4, 0 // shift
          // loop
0:
mov x4, x1 // desired granularity value
lsr x0, x0, x3 // get next granularity
and x4, x4, x0 // compare
add x3, x3, 16 // add shift for next iteration
cmp x3, 48 // if shift 48, desired granularity not supported
b.eq mmu_init_err
// compare to zero and branch to 0
cbz x4, 0b

// write common parameters to TCR_EL1
// bit  [22]    - Selects whether TTBR0_EL1 or TTBR1_EL1 defines the ASID.
// bits [34:32] - Intermediate Physical Address Size (mmu_implemented_parange)
// 8bit ASID and TTBR0_EL1 defines the ASID, 32bits per IPA
bl mmu_implemented_parange

mrs x3, TCR_EL1
// clear bits [22] [36] [34:32]
movz x4, 0x0040, lsl 16
movk x4, 0x0017, lsl 32
mvn x4, x4
and x3, x3, x4
// insert result of mmu_implemented_parange at bit 32
orr x3, x3, x0, lsl 32

// check if Hierarchical Permission for tables implemented and if so, enable them
// bit  [41]    - HPD0
// bit  [42]    - HPD1
mrs x5, ID_AA64MMFR1_EL1
ubfx x5, x5, 12, 4
cmp xzr, x5
// branch if not implemented
b.ne 0f

// clear bits [41] [42]
movz x4, 0x0600, lsl 32
mvn x4, x4
and x3, x3, x4

0:
msr TCR_EL1, x3

// calculate TSZ value needed to achieve `address_space_n_bits`
mov x3, 64
sub x2, x3, x2

/*
   TTBR1_EL1 - configuration registers for KERNEL SPACE
   bits in TTBR1_EL1:
   bits to set:
   bits [47:5] - Translation table base address
   bits [2:1]  - This determines the number of levels to be skipped from the regular start level of the stage 1 EL1&0 translation table walks using TTBR0_EL1.

   bits in TCR_EL1 for TTBR1_EL1:
   bits to set:
   bits [21:16] - The size offset of the memory region addressed by TTBR1_EL1.
   bits [31:30] - Granule size for the TTBR1_EL1.

   bits to zero:
   bit [23] - This bit controls whether a translation table walk is performed on a TLB miss, for an address that is translated using TTBR1_EL1. 
   bit [38] - Top Byte ignored. Indicates whether the top byte of an address is used for address match for the TTBR1_EL1 region, or ignored and used for tagged addresses.
 */
// x0 - TCR
// x1 - GRANULARITY
// x2 - TSZ
// x3 - MASK
mrs x0, TCR_EL1
// mask `0x40c0bf0000` to clear relevant bits
movz x3, 0xc0bf, lsl 16
movk x3, 0x0040, lsl 32
mvn x3, x3
// clear relevant bits
and x0, x0, x3
// place TSZ start on bit [16]
lsl x2, x2, 16
// shift granularity(x1) to position and orr it with x2
orr x1, x2, x1, lsl 30
// insert x1 to TCR(x0)
orr x0, x0, x1
msr TCR_EL1, x0

bl mmu_init_translation_tables_kernel

ldr lr, [sp, -0x8]
add sp, sp, 0x10 // restore sp
mov x0, 1
ret

.global mmu_init_user
// bool mmu_init_user(u8 address_space_n_bits, enum GRANULARITY_TYPE granularity);
// args
// x0 = u8 address_space_n_bits
// x1 = u8 granularity, allowed values are 4, 16, 64
.type mmu_init_user, @function
mmu_init_user:
//mov x0, x2 // x2 = x0
//
//// store return address 
//stp xzr, lr, [sp, -0x10]!
//// get maximum value of TSZ in x0
//bl mmu_maximum_value_of_TSZ
//
//// calculate minimal size of adress space 
//mov x3, 64
//sub x0, x3, x0
//
//// if address_space_n_bits is less then min than jump to err
//cmp x2, x0
//b.lt mmu_init_err
//
//// get supported granularity
//bl mmu_supported_granule_size_stage1
//// if granularity provided by caller supported, register x2 will be greater than zero
//
//// result of mmu_supported_granule_size_stage1 is supported granularities at bits [0] - 4KB [16] - 64KB [32] - 16KB
//mov x4, 0 // shift
//          // loop
//0:
//mov x4, x1 // desired granularity value
//lsr x0, x0, x3 // get next granularity
//and x4, x4, x0 // compare
//add x3, x3, 16 // add shift for next iteration
//cmp x3, 48 // if shift 48, desired granularity not supported
//b.eq mmu_init_err
//// compare to zero and branch to 0
//cbz x4, 0b
//
//// write common parameters to TCR_EL1
//// bit  [22]    - Selects whether TTBR0_EL1 or TTBR1_EL1 defines the ASID.
//// bits [34:32] - Intermediate Physical Address Size (mmu_implemented_parange)
//// bit  [36]    - ASID Size
//// 8bit ASID and TTBR0_EL1 defines the ASID, 32bits per IPA
//bl mmu_implemented_parange
//
//mrs x3, TCR_EL1
//// clear bits [22] [36] [34:32]
//movz x4, 0x0040, lsl 16
//movk x4, 0x0017, lsl 32
//mvn x4, x4
//and x3, x3, x4
//// insert result of mmu_implemented_parange at bit 32
//orr x3, x3, x0, lsl 32
//msr TCR_EL1, x3
//
//// calculate TSZ value needed to achieve `address_space_n_bits`
//mov x3, 64
//sub x2, x3, x2
//
///*
//   TTBR0_EL1 - configuration registers for USER SPACE
//   bits in TTBR0_EL1:
//   bits to set:
//   bits [55:48] - An ASID for the translation table base address. 
//   bits [47:5]  - Translation table base address
//   bits [2:1]   - This determines the number of levels to be skipped from the regular start level of the stage 1 EL1&0 translation table walks using TTBR0_EL1.
//
//   bits in TCR_EL1 for TTBR0_EL1:
//   bits to set:
//   bits [5:0] - The size offset of the memory region addressed by TTBR0_EL1. 
//   bits [15:14] - Granule size for the TTBR0_EL1.
//
//   bits to zero:
//   bit [7] - This bit controls whether a translation table walk is performed on a TLB miss, for an address that is translated using TTBR0_EL1. 
//   bit [37] - Top Byte ignored. Indicates whether the top byte of an address is used for address match for the TTBR0_EL1 region, or ignored and used for tagged addresses.
// */
//// x0 - TCR
//// x1 - GRANULARITY
//// x2 - TSZ
//// x3 - MASK
//mrs x0, TCR_EL1
//// mask `0x200000c0bf` to clear relevant bits
//movz x3, 0xc0bf
//movk x3, 0x0020, lsl 32
//// clear relevant bits
//mvn x3, x3
//and x0, x0, x3
//// insert x2(TSZ) to x0(TCR)
//orr x0, x0, x2
//// insert granularity to x0
//orr x0, x0, x1, lsl 14
//msr TCR_EL1, x0
//
//bl mmu_init_translation_tables_user
//
//ldr lr, [sp, -0x8]
//add sp, sp, 0x10 // restore sp
mov x0, 0
ret

.global mmu_enable
// void mmu_enable(void);
.type mmu_enable, @function
mmu_enable:
mov x1, 0x5       // bit mask
orr x0, x0, x1    // enale cacheability and mmu bits [2] and [0]
msr SCTLR_EL1, x0
// apply changes
isb
ret

.global mmu_maximum_value_of_TSZ
// u8 mmu_maximum_value_of_TSZ(void)
.type mmu_maximum_value_of_TSZ, @function
mmu_maximum_value_of_TSZ:
// reads ST field form register
mrs x0, ID_AA64MMFR2_EL1
// extract bits [31:28]
ubfx x0, x0, 28, 4
tbz x0, 0, 0f // test if bit [0] is = 0 and jump forward to mark 0, if true
mov x0, 48
ldr x0, [sp, -0x8]
ret
0:
mov x0, 39
ret

.global mmu_implemented_parange
.type mmu_implemented_parange, @function
mmu_implemented_parange:
mrs x0, ID_AA64MMFR0_EL1
ubfx x0, x0, 0, 4
ret

.global mmu_supported_granule_size_stage1
// u64 mmu_supported_granule_size_stage1(void)
.type mmu_supported_granule_size_stag1, @function
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

.global mmu_write_ttbr1_el1
.type mmu_write_ttbr1_el1, @function
mmu_read_ttbr1_el1:
mrs x0, TTBR1_EL1
ret
