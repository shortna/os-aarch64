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

// 48bit OA
struct InvalidEntry4KB {
  u64 ignored_0             : 62;
  enum DESCRIPTOR_TYPE type : 2;
};

// 48bit OA
struct TableEntry4KB {
  u8 attributes             : 5;
  u8 ignored_0              : 8;
  u8 res0                   : 3;
  u64 next_level            : 36;
  u16 ignored_1             : 10;
  enum DESCRIPTOR_TYPE type : 2;
};

// 48bit OA
struct BlockEntry4KB {
  u16 upper_attributes      : 14;
  u8 res0_0                 : 2;
  u32 output_address        : 27; // for level 1 only upper 17bits valid, for level 2 all 27 are valid
  u8 res0_1                 : 4;
  u8 nt                     : 1;
  u8 res0_2                 : 4;
  u16 lower_attributes      : 10;
  enum DESCRIPTOR_TYPE type : 2;
};

// 48bit OA
struct PageEntry4KB {
  u16 upper_attributes      : 14;
  u8 res0                   : 2;
  u64 output_address        : 36;
  u16 lower_attributes      : 10;
  enum DESCRIPTOR_TYPE type : 2;
};

extern void mmu_read_ttbr1_el1(void);
extern void mmu_write_ttbr1_el1(u64 value);
extern void mmu_read_ttbr0_el1(void);
extern void mmu_write_ttbr0_el1(u64 value);

#define ADDRESS_SPACE_KERNEL_SIZE (1024 * 1024 * 1024 + 1024 * 1024 * 256)
#define ADDRESS_SPACE_USER_SIZE   (1024 * 1024 * 768)

#define PAGE_SIZE                     ((u64)4096)
#define TABLE_ENTRY_SIZE              (8)
#define TABLE_N_ENTRIES               (PAGE_SIZE / TABLE_ENTRY_SIZE)
#define TABLE_SIZE                    (TABLE_N_ENTRIES * TABLE_ENTRY_SIZE)

#define LEVEL_3_ADDRESSABLE_PER_ENTRY (PAGE_SIZE)
#define LEVEL_2_ADDRESSABLE_PER_ENTRY (LEVEL_3_ADDRESSABLE_PER_ENTRY * TABLE_N_ENTRIES)
#define LEVEL_1_ADDRESSABLE_PER_ENTRY (LEVEL_2_ADDRESSABLE_PER_ENTRY * TABLE_N_ENTRIES)
#define LEVEL_0_ADDRESSABLE_PER_ENTRY (LEVEL_1_ADDRESSABLE_PER_ENTRY * TABLE_N_ENTRIES)

// according to mem_map first 1GB of address sapce is 
#define FDT_SIZE   (0x0000000008000000) // fdt
#define FALSH_SIZE (0x0000000008000000) // flash
#define MMIO_SIZE  (0x0000000038000000) // mmio

static u64 mmu_size_per_entry(u8 level) {
  switch (level) {
    case 0: 
      return LEVEL_0_ADDRESSABLE_PER_ENTRY;
    case 1: 
      return LEVEL_1_ADDRESSABLE_PER_ENTRY;
    case 2: 
      return LEVEL_2_ADDRESSABLE_PER_ENTRY;
    case 3: 
      return LEVEL_3_ADDRESSABLE_PER_ENTRY;
    default:
      return UINT64_MAX;
  }
}

static void mmu_get_n_entries_per_level(u64 address_space_size, u16 table_entries[4]) {
  u8 start_level;
  if (address_space_size >= LEVEL_0_ADDRESSABLE_PER_ENTRY) {
    start_level = 0;
  }
  else if (address_space_size >= LEVEL_1_ADDRESSABLE_PER_ENTRY) {
    start_level = 1;
  }
  else if (address_space_size >= LEVEL_2_ADDRESSABLE_PER_ENTRY) {
    start_level = 2;
  }
  else {
    start_level = 3;
  }

  for (u8 i = start_level; i < 4; i++) {
    u32 n_entries = 0;
    u64 cur_size_per_entry = mmu_size_per_entry(i);

    while (address_space_size >= cur_size_per_entry) {
      address_space_size = address_space_size - cur_size_per_entry;
      n_entries++;
    }

    table_entries[i] = n_entries;
  }

  // if last n_entries in last table != 0(in this case 0 means TABLE_N_ENTRIES), 
  // increment all n_entries before
  for (i8 i = 3; i >= start_level + 1; i--) {
    if (table_entries[i] != 0) {
      table_entries[i - 1]++;
    }
  }
}

u64 create_tables(void *base, u8 level, u16 entries[4], u64 start_of_address_space) {
  if (level > 3) {
    return start_of_address_space;
  }

  if (level == 3) {
    for (u16 i = 0; i < entries[level]; i++) {
      ((struct PageEntry4KB*)base)[i] = (struct PageEntry4KB) {
        output_address = start_of_address_space, 
        type = DESCRIPTOR_PAGE,
      };
      start_of_address_space += PAGE_SIZE;
    }
    return start_of_address_space;
  }

  // next table determined relative to current `base` and alligned to 
  u64 table_addr = (u64)base;
  for (u16 i = 0; i < entries[level]; i++) {
    struct TableEntry4KB *next = (void*)ALLIGN(table_addr, entries[level + 1] * TABLE_ENTRY_SIZE);
    ((struct TableEntry4KB*)base)[i] = (struct TableEntry4KB) {
      .next_level = (u64)next & GENMASK(36, 0),
      .type = DESCRIPTOR_TABLE,
    };
    table_addr += TABLE_SIZE;
    start_of_address_space = create_tables(next, level + 1, entries, start_of_address_space);
  }

  return start_of_address_space;
}

void mmu_init_translation_tables_kernel(void) {
  extern u64 FREE_MEMORY;
  u16 table_entries[4] = {0};
  mmu_get_n_entries_per_level(ADDRESS_SPACE_KERNEL_SIZE, table_entries);

  u8 start_level = 0;
  while (table_entries[start_level] == 0) {
    start_level++;
  }

  // not possible
  // thereis AT LEAST 1GB of address_space
  if (start_level == 3 || start_level == 2) {
    BREAKPOINT(0);
  }

  // always one
  struct TableEntry4KB *base_table = (void *)ALLIGN((u64)&FREE_MEMORY, table_entries[start_level] * TABLE_ENTRY_SIZE);
  u16 entries_per_level[4] = {[start_level] = start_level, TABLE_N_ENTRIES, TABLE_N_ENTRIES};
  for (u16 i = 0; i < table_entries[start_level]; i++) {
    if (i + 1 == table_entries[start_level] && table_entries[start_level + 1] != 0) {
    }
    create_tables(base_table, start_level, entries_per_level, 0);
  }

  BREAKPOINT(1);
}

void mmu_init_translation_tables_user(void) {
  BREAKPOINT(2);
};
