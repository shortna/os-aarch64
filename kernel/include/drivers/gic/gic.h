#ifndef _GIC_H
#define _GIC_H

#include "types.h"

#define GICV3_GROUP0 (0)
#define GICV3_GROUP1_SECURE (1)
#define GICV3_GROUP1_NON_SECURE (2)

#define GICV3_LEVEL_SENSITIVE (0)
#define GICV3_EDGE_TRIGGERED (2)

struct GICDv3;
struct GICRv3;
typedef struct GICDv3 *GICD;
typedef struct GICRv3 *GICR;

struct InterruptParameters {
  u8 priority;
  u8 security;
  u8 triger;
  bool route;
  u32 affinity;
};

extern void register_interrupt(GICD dist, GICR redists, u32 int_id,
                               struct InterruptParameters p);

// get current PE
extern u32 get_affinity(void);

// returns pointer to distributor
extern GICD get_distributor(u64 gicd_base);
// returns pointer to redistributors (including sgis and lpis)
extern GICR get_redistributor(u64 gicr_base, u32 redist_id);

// enable interrupt controller
extern u8 gic_init(GICD d);

// find redistributor that belongs to the specified affinity
extern u32 get_redistributor_id(u64 gicr_base, u32 affinity);
// wake up redistributor
extern void wake_redistributor(GICR redist);

/* INTERRUPT CONTROL */

extern void int_set_priority(GICD dist, GICR redist, u32 int_id, u8 priority);
extern void int_set_group(GICD dist, GICR redist, u32 int_id, u8 security);
extern void int_set_triger(GICD dist, GICR redist, u32 int_id, u8 triger);
extern void int_set_route(GICD dist, u32 int_id, u32 affinity);
extern void int_enable(GICD dist, GICR redist, u32 int_id);

/* GIC CPU INTERFACE */

// set interrupt priority mask
extern void set_priority_mask(u8 mask);

// enable support for interrupts
extern void enable_group0_ints(void);
extern void enable_group1_ints(void);

// configure current PE interface for interrupts e.g. enable SRE
extern void configure_icc(void);

#endif
