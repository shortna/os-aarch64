#include "drivers/mmu/mmu.h"
#include "bits.h"

enum ADDRESSABLE_IPS {/*{{{*/
  IPS_4GB   = 0,
  IPS_64GB  = 1,
  IPS_1TB   = 2,
  IPS_4TB   = 3,
  IPS_16TB  = 4,
  IPS_256TB = 5,
  IPS_4PB   = 6,
  IPS_64PB  = 7,
};/*}}}*/

enum PAGE_ATTRIBUTES : uint64_t {/*{{{*/
  PAGE_ATTRIBUTE_UNPRIVILEGED_EXECUTE_NEVER = BIT(54),
  PAGE_ATTRIBUTE_READ_WRITE                 = 0x040,
  PAGE_ATTRIBUTE_READ_ONLY                  = 0x0C0,

  PAGE_ATTRIBUTE_PRIVILEGED_EXECUTE_NEVER   = BIT(53),
  PAGE_ATTRIBUTE_PRIVILEGED_READ_WRITE      = 0x000,
  PAGE_ATTRIBUTE_PRIVILEGED_READ_ONLY       = 0x080,

  PAGE_ATTRIBUTE_CONTIGUOUS                 = BIT(52),
  PAGE_ATTRIBUTE_NON_GLOBAL                 = 0x800,
  PAGE_ATTRIBUTE_ACCESS_FLAG                = 0x400,
  PAGE_ATTRIBUTE_NON_SHAREABLE              = 0x000,
  PAGE_ATTRIBUTE_INNER_SHAREABLE            = 0x300,
  PAGE_ATTRIBUTE_OUTER_SHAREABLE            = 0x200,
};/*}}}*/

enum BLOCK_ATTRIBUTES : uint64_t {/*{{{*/
  BLOCK_ATTRIBUTE_UNPRIVILEGED_EXECUTE_NEVER = BIT(54),
  BLOCK_ATTRIBUTE_READ_WRITE                 = 0x040,
  BLOCK_ATTRIBUTE_READ_ONLY                  = 0x0C0,

  BLOCK_ATTRIBUTE_PRIVILEGED_EXECUTE_NEVER   = BIT(53),
  BLOCK_ATTRIBUTE_PRIVILEGED_READ_WRITE      = 0x000,
  BLOCK_ATTRIBUTE_PRIVILEGED_READ_ONLY       = 0x080,

  BLOCK_ATTRIBUTE_CONTIGUOUS                 = BIT(52),
  BLOCK_ATTRIBUTE_NON_GLOBAL                 = 0x800,
  BLOCK_ATTRIBUTE_ACCESS_FLAG                = 0x400,
  BLOCK_ATTRIBUTE_NON_SHAREABLE              = 0x000,
  BLOCK_ATTRIBUTE_INNER_SHAREABLE            = 0x300,
  BLOCK_ATTRIBUTE_OUTER_SHAREABLE            = 0x200,
};/*}}}*/

enum TABLE_ATTRIBUTES : uint64_t {/*{{{*/
  TABLE_ATTRIBUTE_PRIVILEGED_EXECUTE_NEVER   = BIT(59),
  TABLE_ATTRIBUTE_UNPRIVILEGED_EXECUTE_NEVER = BIT(60),
  TABLE_ATTRIBUTE_PRIVILEGED_READ_WRITE      = BIT(61),
  TABLE_ATTRIBUTE_READ_ONLY                  = BIT(62),
  TABLE_ATTRIBUTE_PRIVILEGED_READ_ONLY       = BIT(62) | BIT(61),
};/*}}}*/

enum MEMORY_ATTRIBUTES {/*{{{*/
  MEMORY_ATTRIBUTE_nGnRnE = 0x0,
  MEMORY_ATTRIBUTE_NORMAL = 0x4, // first index
};/*}}}*/

/*{{{*/
#define PAGE_ENTRY           0x3
#define TABLE_ENTRY          0x3
#define BLOCK_ENTRY          0x1

// ideally this info need to be passed from fdt
#define FDT_END              0x00100000
#define FLASH_END            0x08000000
#define MMIO_END             _1GB
#define RAM_END              (_1GB * 2)

#define FDT_L3_ENTRY_ATTRIBUTES  ((uint64_t)PAGE_ENTRY | MEMORY_ATTRIBUTE_nGnRnE | PAGE_ATTRIBUTE_CONTIGUOUS \
    | PAGE_ATTRIBUTE_NON_SHAREABLE | PAGE_ATTRIBUTE_ACCESS_FLAG | PAGE_ATTRIBUTE_PRIVILEGED_READ_ONLY \
    | PAGE_ATTRIBUTE_UNPRIVILEGED_EXECUTE_NEVER | PAGE_ATTRIBUTE_PRIVILEGED_EXECUTE_NEVER)

#define FLASH_L3_ENTRY_ATTRIBUTES  ((uint64_t)PAGE_ENTRY | MEMORY_ATTRIBUTE_NORMAL | PAGE_ATTRIBUTE_CONTIGUOUS \
    | PAGE_ATTRIBUTE_PRIVILEGED_READ_ONLY | PAGE_ATTRIBUTE_UNPRIVILEGED_EXECUTE_NEVER | PAGE_ATTRIBUTE_INNER_SHAREABLE \
    | PAGE_ATTRIBUTE_ACCESS_FLAG)

#define MMIO_L3_ENTRY_ATTRIBUTES  ((uint64_t)PAGE_ENTRY | MEMORY_ATTRIBUTE_nGnRnE | PAGE_ATTRIBUTE_CONTIGUOUS \
    | PAGE_ATTRIBUTE_NON_SHAREABLE | PAGE_ATTRIBUTE_ACCESS_FLAG | PAGE_ATTRIBUTE_PRIVILEGED_READ_WRITE \
    | PAGE_ATTRIBUTE_PRIVILEGED_EXECUTE_NEVER | PAGE_ATTRIBUTE_UNPRIVILEGED_EXECUTE_NEVER)

#define RAM_L3_ENTRY_ATTRIBUTES  ((uint64_t)PAGE_ENTRY | MEMORY_ATTRIBUTE_NORMAL | PAGE_ATTRIBUTE_CONTIGUOUS \
    | PAGE_ATTRIBUTE_INNER_SHAREABLE | PAGE_ATTRIBUTE_READ_WRITE | PAGE_ATTRIBUTE_ACCESS_FLAG)

#define _1GB                 ((uint64_t)1024 * 1024 * 1024)
#define _1TB                 ((uint64_t)1024 * 1024 * 1024 * 1024)
#define _1PB                 ((uint64_t)1024 * 1024 * 1024 * 1024 * 1024)/*}}}*/

extern uint64_t mmu_read_ttbr1_el1(void);/*{{{*/
extern void mmu_write_ttbr1_el1(uint64_t value);
extern uint64_t mmu_read_ttbr0_el1(void);
extern void mmu_write_ttbr0_el1(uint64_t value);/*}}}*/

struct Block {
  uint16_t next;
  bool occupied;
  uint8_t __pad;
};

struct Blocks {
  struct Block *b;
  uint64_t n;
};

struct Blocks BLOCKS;

void *malloc(uint16_t n_blocks) {
  for (uint64_t block = 0; block < BLOCKS.n; block++) {
    uint16_t i = 0;
    while (!BLOCKS.b[block + i].occupied && i < n_blocks) {
      if (block + i == BLOCKS.n)
        return NULL;
      i++;
    }

    if (i == n_blocks) {
      BLOCKS.b[block].next = i - 1;
      BLOCKS.b[block].occupied = true;
      while (--i) 
        BLOCKS.b[block + i].occupied = true;

      return (void*)ALIGN((uint64_t)(BLOCKS.b + BLOCKS.n), PAGE_SIZE) + block * PAGE_SIZE;
    }
    block += i + BLOCKS.b[block + i].next;
  }

  return NULL;
}

void free(void *addr) {
  uint64_t block_ind = (uint64_t)(addr - (void*)(BLOCKS.b + BLOCKS.n)) / PAGE_SIZE;
  for (uint16_t block = 0; block < BLOCKS.b[block_ind].next + 1; block++) {
    BLOCKS.b[block_ind + block].occupied = false;
  }
}

uint64_t PAGE_SIZE = 4096;
void mmu_init_translation_tables(enum ADDRESSABLE_IPS ips_size, enum GRANULARITY_TYPE type) {
  uint64_t TABLE_ENTRY_SIZE = 8;
  uint64_t TABLE_ENTRIES_MAX; 
  uint64_t TABLE_SIZE;        

  switch (type) {
    case GRANULARITY_16KB:
      PAGE_SIZE = 16384;
      break;
    case GRANULARITY_64KB:
      PAGE_SIZE = 65536;
      break;
    case GRANULARITY_4KB:
    default:
      break;
  }

  TABLE_ENTRIES_MAX = (PAGE_SIZE / TABLE_ENTRY_SIZE); 
  TABLE_SIZE = TABLE_ENTRIES_MAX * TABLE_ENTRY_SIZE;

  uint64_t n_entries = 0;
  switch(ips_size) {
    case IPS_4GB:
      n_entries = _1GB * 4 / PAGE_SIZE;
      break;
    case IPS_64GB:
      n_entries = _1GB * 64 / PAGE_SIZE;
      break;
    case IPS_1TB:
      n_entries = _1TB / PAGE_SIZE;
      break;
    case IPS_4TB:
      n_entries = _1TB * 4 / PAGE_SIZE;
      break;
    case IPS_16TB:
      n_entries = _1TB * 16 / PAGE_SIZE;
      break;
    case IPS_256TB:
      n_entries = _1TB * 256 / PAGE_SIZE;
      break;
    case IPS_4PB:
      n_entries = _1PB * 4 / PAGE_SIZE;
      break;
    case IPS_64PB:
      n_entries = _1PB * 64 / PAGE_SIZE;
      break;
  }

  uint64_t *base = (void *)ALIGN((uint64_t)FREE, TABLE_SIZE);
  uint64_t *cur_level = base;
  uint64_t address = 0x0;

  // address < 0x00100000 - nGnRnE, r,   non_shareable    // fdt
  // address < 0x08000000 - normal, rx,  shareable        // flash with code
  // address < _1GB       - nGnRnE, rw,  non_shareable    // mmio
  // address >= _1GB      - normal, rwx, 25% - 2MB BLOCKS // ram

  // fill pages
  uint64_t attrs = FDT_L3_ENTRY_ATTRIBUTES;
  uint64_t n_written_pages = 0;
  for (; address < RAM_END; address += PAGE_SIZE, cur_level++, n_written_pages++) {
    switch (address) {
      case FDT_END:
        attrs = FLASH_L3_ENTRY_ATTRIBUTES;
        break;
      case FLASH_END:
        attrs = MMIO_L3_ENTRY_ATTRIBUTES;
        break;
      case MMIO_END:
        attrs = RAM_L3_ENTRY_ATTRIBUTES;
        break;
    }
    *cur_level = address | attrs;
  }

  uint64_t *previous_level = base;
  n_entries = n_written_pages;
  for (uint8_t i = 3; i > 0; i--) {
    cur_level = (void*)ALIGN((uint64_t)(cur_level + n_entries), TABLE_SIZE);
    n_entries = n_entries / TABLE_ENTRIES_MAX + (bool)(n_entries % TABLE_ENTRIES_MAX); // n_tables + 1 if last not full
    for (uint64_t entry = 0; entry < n_entries; entry++)
      cur_level[entry] = (uint64_t)previous_level + entry * TABLE_SIZE | TABLE_ENTRY;

    previous_level = cur_level;
  }

  uint64_t ttbr0_el1 = mmu_read_ttbr0_el1();
  ttbr0_el1 |= (uint64_t)cur_level; // write address of ttbr
  mmu_write_ttbr0_el1(ttbr0_el1);

  FREE = (void*)ALIGN((uint64_t)(cur_level + TABLE_ENTRIES_MAX), PAGE_SIZE);
  n_entries = (RAM_END - (uint64_t)FREE) / PAGE_SIZE;
  n_entries -= n_entries * sizeof(struct Block) / PAGE_SIZE + (bool)((n_entries * sizeof(struct Block)) % PAGE_SIZE);

  BLOCKS.b = FREE;
  BLOCKS.n = n_entries;
  for (uint64_t i = 0; i < n_entries; i++) {
    BLOCKS.b[i] = (struct Block){0};
  }
}
