#include "drivers/mmu/mmu.h"
#include "bits.h"

/*
   I am gonna split virtual address space in two parts (kernel and user spaces)
   theres 2G of ddress space in VM so be it 1280MB for kernel and 768MB for user space
   (cause kernel have peripherals and it will take 256MB of RAM)

   for 1280MB block size of address space will be 31 bit
   which means translation starts at level 1 bits [30:0]

   for 768MB block size of address space will be 30 bit
   which means translation starts at level 2 bits [29:0]

   Chosen configuration:
   granularity = 4KB
*/

// DESCRIPTOR_INVALID  = 0x0, // bit [1] doesnt matter bit [0] always 0
// DESCRIPTOR_RESERVED = 0x2, // if entry level == 3
// DESCRIPTOR_TABLE    = 0x3, // if entry level != 3, otherwise PAGE
// DESCRIPTOR_BLOCK    = 0x1, // if entry level != 3, otherwise RESERVED
// DESCRIPTOR_PAGE     = 0x3, // if entry level == 3,

#define PAGE_UPPER_ATTRIBUTE_EXECUTE_NEVER         0x010 // priviliged or unpriviliged depends on security state
#define PAGE_UPPER_ATTRIBUTE_CONTIGUOUS            0x004
#define PAGE_LOWER_ATTRIBUTE_NON_GLOBAL            0x200
#define PAGE_LOWER_ATTRIBUTE_ACCESS_FLAG           0x100
#define PAGE_LOWER_ATTRIBUTE_NON_SHAREABLE         0x000
#define PAGE_LOWER_ATTRIBUTE_OUTER_SHAREABLE       0x080
#define PAGE_LOWER_ATTRIBUTE_INNER_SHAREABLE       0x0C0
#define PAGE_LOWER_ATTRIBUTE_PRIVILEGED_READ_WRITE 0x000
#define PAGE_LOWER_ATTRIBUTE_READ_WRITE            0x010
#define PAGE_LOWER_ATTRIBUTE_PRIVILEGED_READ_ONLY  0x020
#define PAGE_LOWER_ATTRIBUTE_READ_ONLY             0x030

#define TABLE_ATTRIBUTE_EXECUTE_NEVER              0x02 // priviliged or unpriviliged depends on security state
#define TABLE_ATTRIBUTE_PRIVILIGED                 0x04
#define TABLE_ATTRIBUTE_READ_ONLY                  0x08
#define TABLE_ATTRIBUTE_PRIVILIGED_READ_ONLY       0x0C

extern uint64_t mmu_read_ttbr1_el1(void);
extern void mmu_write_ttbr1_el1(uint64_t value);
extern uint64_t mmu_read_ttbr0_el1(void);
extern void mmu_write_ttbr0_el1(uint64_t value);

#define PAGE_SIZE         (4096)
#define TABLE_ENTRY_SIZE  (8)
#define TABLE_ENTRIES_MAX (PAGE_SIZE / TABLE_ENTRY_SIZE)
#define TABLE_SIZE        ((uint64_t)TABLE_ENTRIES_MAX * TABLE_ENTRY_SIZE)

#define ADDRESS_SPACE ((uint64_t)1024 * 1024 * 1024 + 1024 * 1024 * 256)
extern uint8_t MEMORY_NORMAL_IND;
extern uint8_t MEMORY_nGnRnE_IND;

void *kmalloc(uint64_t size) {}
void *mmap(void *addr) {}

#define MEMORY_ATTRIBUTE_nGnRnE (0x0)
#define MEMORY_ATTRIBUTE_NORMAL (0x1 << 2)

#define BLOCK_ENTRY 0x00000000000000401
#define TABLE_ENTRY 0x00000000000000403
#define PAGE_ENTRY  0x00000000000000403

void mmu_init_translation_tables_kernel(void) {
  uint64_t *tables = (void *)ALIGN((uint64_t)KERNEL_DATA, TABLE_SIZE);
  uint64_t *tables_level2 = tables + TABLE_ENTRIES_MAX;
  uint64_t *pages = tables_level2 + TABLE_ENTRIES_MAX * 2;

  for (uint8_t entry = 0; entry < 2; entry++) {
    // orr address to entry
    tables[entry] = (uint64_t)tables_level2 + entry * TABLE_SIZE | TABLE_ENTRY;
  }

  uint64_t n_pages = ADDRESS_SPACE / PAGE_SIZE;
  for (uint16_t entry = 0; entry < n_pages / TABLE_ENTRIES_MAX; entry++) {
    tables_level2[entry] = (uint64_t)pages + entry * TABLE_SIZE | TABLE_ENTRY;
  }

  uint64_t address = 0;
  for (uint32_t page = 0; page < n_pages; page++, address += PAGE_SIZE) {
    pages[page] = address | MEMORY_ATTRIBUTE_nGnRnE | PAGE_ENTRY;
  }

  uint64_t ttbr0_el1 = mmu_read_ttbr0_el1();
  ttbr0_el1 |= (uint64_t)tables; // write address of ttbr
  mmu_write_ttbr0_el1(ttbr0_el1);
}

void mmu_init_translation_tables_user(void) {
  BREAKPOINT(1);
};
