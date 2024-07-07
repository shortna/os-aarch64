#include "fdt.h"
#include "bits.h"

#define FDT_VERSION_16 (16)
#define FDT_VERSION_17 (17)

// those are rotated from BIG to LITTLE
#define FDT_MAGIC U32(0xedfe0dd0)
#define FDT_BEGIN_NODE U32(0x01000000)
#define FDT_END_NODE U32(0x02000000)
#define FDT_PROP U32(0x03000000)
#define FDT_NOP U32(0x04000000)
#define FDT_END U32(0x09000000)

struct FdtHeader {
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

bool fdt_walk(u64 address) {
  if (*(u32 *)address != (FDT_MAGIC)) {
    return false;
  }

  struct FdtHeader *h = (void *)address;
  if (rotate_u32(h->version) != FDT_VERSION_16 ||
      rotate_u32(h->version) != FDT_VERSION_17) {
    return false;
  }
}

