#include "drivers/mmu/mmu.h"
#include "bits.h"

/*
   There are several independent virtual address spaces in AArch64.
   I am only interested in:
   1) Non-secure EL0 and EL1 virtual
   2) Non-secure PAS0 which is physical

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

enum DESCRIPTOR_TYPE {
  DESCRIPTOR_INVALID  = 0x0, // bit [1] doesnt matter bit [0] always 0
  DESCRIPTOR_BLOCK    = 0x2, // if table level != 3, otherwise RESERVED
  DESCRIPTOR_RESERVED = 0x2, // if table level == 3
  DESCRIPTOR_TABLE    = 0x3, // if table level != 3, otherwise PAGE
  DESCRIPTOR_PAGE     = 0x3, // if table level == 3,
};

//enum ATTRIBUTES_TABLE {
//};

enum ATTRIBUTES_PAGE  {
  UNPRIVILEGED_EXECUTE_NEVER = 1;
  PRIVILEGED_EXECUTE_NEVER = 1;
};

// 48bit OA
struct InvalidEntry4KB {
  uint64_t ignored_0             : 62;
  enum DESCRIPTOR_TYPE type : 2;
};

// 48bit OA
struct TableEntry4KB {
  uint8_t attributes             : 5;
  uint8_t ignored_0              : 8;
  uint8_t res0                   : 3;
  uint64_t next_level            : 36;
  uint16_t ignored_1             : 10;
  enum DESCRIPTOR_TYPE type : 2;
};

// 48bit OA
struct BlockEntry4KB {
  uint16_t upper_attributes      : 14;
  uint8_t res0_0                 : 2;
  uint32_t output_address        : 27; // for level 1 only upper 17bits valid, for level 2 all 27 are valid
  uint8_t res0_1                 : 4;
  uint8_t nt                     : 1;
  uint8_t res0_2                 : 4;
  uint16_t lower_attributes      : 10;
  enum DESCRIPTOR_TYPE type : 2;
};

// 48bit OA
struct PageEntry4KB {
  uint16_t upper_attributes      : 14;
  uint8_t res0                   : 2;
  uint64_t output_address        : 36;
  uint16_t lower_attributes      : 10;
  enum DESCRIPTOR_TYPE type : 2;
};

extern void mmu_read_ttbr1_el1(void);
extern void mmu_write_ttbr1_el1(uint64_t value);
extern void mmu_read_ttbr0_el1(void);
extern void mmu_write_ttbr0_el1(uint64_t value);

// 1GB
#define ADDRESS_SPACE (1024 * 1024 * 1024)

#define PAGE_SIZE                     (4096)
#define TABLE_ENTRY_SIZE              (8)
#define TABLE_ENTRIES_MAX             (PAGE_SIZE / TABLE_ENTRY_SIZE)
#define TABLE_SIZE                    ((uint64_t)TABLE_ENTRIES_MAX * TABLE_ENTRY_SIZE)

#define FLASH_SIZE ((uint64_t)0x08000000) // 
#define MMIO_SIZE  ((uint64_t)0x38000000) // nGnRnE
#define RAM_SIZE   ((uint64_t)0x40000000) // GRE

static uint8_t mmu_get_n_entries_per_level(uint64_t address_space_size, uint64_t table_entries[4]) {
  uint8_t table_entries_max_bit = ctz(TABLE_ENTRIES_MAX);
  uint64_t n_pages = address_space_size / PAGE_SIZE;
  uint8_t start_level = 0;

  for (int8_t level = 3; level >= 0; level--) {
    // ceil(address_space_size / pow(TABLE_ENTRIES_MAX, (3 - level)))
    n_pages = 1 + ((n_pages - 1) / (1 << ((3 - level) * table_entries_max_bit))); 
    table_entries[level] = n_pages;
    if (address_space_size / ((uint64_t)PAGE_SIZE << ((3 - level) * table_entries_max_bit)) > 0)
      start_level = level;
  }

  return start_level;
}

static void* create_tables(void *base, uint64_t address_space_start, uint8_t start_level, const uint64_t table_entries[4]) {
  base = (void *)ALIGN((uint64_t)base, TABLE_SIZE);

  void *current_level = base;
  void *next_level = base;
  // fill tables
  for (uint8_t level = start_level; level < 3; level++) {
    uint16_t n_incomplete_entries = table_entries[level] % TABLE_ENTRIES_MAX;
    next_level += (table_entries[level] + n_incomplete_entries) * TABLE_ENTRY_SIZE;

    struct TableEntry4KB *t = current_level;
    for (uint64_t entry = 0; entry < table_entries[level]; entry++) {
      t->type = DESCRIPTOR_TABLE;
      t->next_level = (uint64_t)next_level + entry * TABLE_SIZE;
      t += sizeof(*t);
    }

    while (n_incomplete_entries--) {
      t->type = DESCRIPTOR_INVALID;
      t += sizeof(*t);
    }

    current_level = (void*)next_level;
  }

  // fill pages
  struct PageEntry4KB *p = current_level;
  for (uint64_t page = 0; page < table_entries[3]; page++) {
    p->output_address = address_space_start + page * PAGE_SIZE;
    p->type = DESCRIPTOR_PAGE;
    p += sizeof(*p);
  }

  return base;
} 

void mmu_init_translation_tables_kernel(void) {
  uint64_t table_entries[4] = {0};
  uint8_t start_level = mmu_get_n_entries_per_level(ADDRESS_SPACE_KERNEL_SIZE, table_entries);

  // not possible
  // 1GB of address_space always present
  if (start_level == 3 || start_level == 2)
    BREAKPOINT(BREAKPOINT_IND++);

  uint64_t start_table = (uint64_t)create_tables(&FREE_MEMORY, 0x0, start_level, table_entries);
  uint64_t ttbr1_el1 = mmu_read_ttbr1_el1();
  ttbr1_el1 |= start_table;      // write address of ttbr
  ttbr1_el1 |= start_level << 1; // write start level
  mmu_write_ttbr1_el1(ttbr1_el1);
}

void *mmu_get_page(void *addr, uint16_t upper_attrs, uint16_t lower_attrs) {}
void *mmu_get_block(void *addr, uint16_t upper_attrs, uint16_t lower_attrs) {}
void *mmu_get_table(void *addr, uint16_t upper_attrs, uint16_t lower_attrs) {}

void mmu_init_translation_tables_user(void) {
  BREAKPOINT(BREAKPOINT_IND++);
};
