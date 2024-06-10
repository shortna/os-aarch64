#ifndef _FDT_H
#define _FDT_H

#include "types.h"

struct FDTProperty {
  u32 len;
  u32 nameoff;
  u8 *value;
};

struct FDTNode{
  u8 *name;
};

struct FDTReserveEntry {
  u64 address;
  u64 size;
};

/*
 * Reads only nodes.
 * `node` will point to entry.
 * returns `false` when no more `node` 's found.
 */
extern bool fdt_read_node(const void *fdt, struct FDTNode *node);

/*
 * Reads `propety` for node_entry.
 * `property` will point to entry.
 * returns `false` when no more `propertie` 's found.
 */
extern bool fdt_read_property(const void *fdt, struct FDTNode *node,
                              struct FDTProperty *property);

/*
 * Reads reserved entries.
 * `entry` will point to entry.
 * returns `false` when no more `entry` 's found.
 */
extern bool fdt_read_reserve_entry(const void *fdt,
                                   struct FDTReserveEntry *entry);

/*
 * Check if `fdt` has valid magic and version.
 */
extern bool fdt_valid_header(const void *fdt);


/*
 * Extract `node` address from `node.name`.
 * returns `true` if address specified.
 */
extern bool fdt_get_node_address(struct FDTNode *node, u64 *res);
#endif
