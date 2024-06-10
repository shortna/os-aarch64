#include "fdt.h"
#include "string.h"

#define FDT_MAGIC U32(0xd00dfeed)
#define FDT_VERSION U8(0x11)
#define FDT_LOWEST_COMPATIBLE_VERSION U8(0x10)

#define FDT_TOKEN_BEGIN_NODE U32(0x00000001)
#define FDT_TOKEN_END_NODE U32(0x00000002)
#define FDT_TOKEN_PROP U32(0x00000003)
#define FDT_TOKEN_NOP U32(0x00000004)
#define FDT_TOKEN_END U32(0x00000009)

struct FDTHeader {
  u32 magic;
  u32 totalsize;
  u32 off_dt_struct;
  u32 off_dt_strings;
  u32 off_mem_rsvmap;
  u32 version;
  u32 last_comp_version;
  u32 boot_cpuid_phys;
  u32 size_dt_strings;
  u32 size_dt_struct;
};

static u32 STRUCTURE_BLOCK_CURRENT_OFFSET = 0;
bool fdt_read_node(const void *fdt, struct FDTNode *node) {
  const struct FDTHeader *fdt_header = fdt;
  const u32 *structure_block = fdt + REVERSE_U32(fdt_header->off_dt_struct) +
                               STRUCTURE_BLOCK_CURRENT_OFFSET;

  // find the begin of the node
  do {
    if (REVERSE_U32(*structure_block) == FDT_TOKEN_BEGIN_NODE) {
      structure_block++;
      break;
    }
    if (REVERSE_U32(*structure_block) == FDT_TOKEN_END) {
      return false;
    }
    structure_block++;
  } while (1);

  // skip pading and emtpy tokens
  // 0 is 0 no matter the endianess
  while (REVERSE_U32(*structure_block) == FDT_TOKEN_NOP ||
         *structure_block == 0) {
    structure_block++;
  }

  node->name = (u8 *)structure_block;
  STRUCTURE_BLOCK_CURRENT_OFFSET =
      (u8 *)structure_block - (u8 *)fdt + sizeof(*structure_block);
  return true;
}

bool fdt_get_node_address(struct FDTNode *node, u64 *res) {
  u32 at;
  if (!memfind(node->name, strlen(node->name), '@', &at)) {
    return false;
  }
  *res = str_to_num(node->name + at + 1);
  return true;
}

bool fdt_read_property(const void *fdt, struct FDTNode *node,
                       struct FDTProperty *property) {
  return true;
}

bool fdt_read_reserve_entry(const void *fdt, struct FDTReserveEntry *entry) {
  (void)fdt;
  (void)entry;
  return true;
}

bool fdt_valid_header(const void *fdt) {
  const struct FDTHeader *fdt_header = fdt;

  if (fdt_header->magic != REVERSE_U32(FDT_MAGIC)) {
    return false;
  }

  if (fdt_header->version > REVERSE_U32(FDT_VERSION) ||
      fdt_header->version < REVERSE_U32(FDT_LOWEST_COMPATIBLE_VERSION)) {
    return false;
  }

  if (fdt_header->totalsize == 0) {
    return false;
  }

  return true;
}
