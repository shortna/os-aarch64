#ifndef _GIC_H
#define _GIC_H

#include "types.h"

#define GICV3_GROUP0 (0)
#define GICV3_GROUP1_SECURE (1)
#define GICV3_GROUP1_NON_SECURE (2)

#define GICV3_LEVEL_SENSITIVE (0)
#define GICV3_EDGE_TRIGGERED (2)

struct GICRv3;
struct GICDv3;
typedef struct GICRv3 *GICR;
typedef struct GICDv3 *GICD;

// get current PE
extern u32 getAffinity(void);

// returns pointer to distributor
extern GICD gicd(u64 gicd_base);
// returns pointer to redistributors (including sgis and lpis)
extern GICR gicr(u64 gicr_base);

// enable interrupt controller
extern u8 enableGIC(GICD dist);

// find redistributor that belongs to the specified affinity
extern u32 getRedistributorId(GICR redists, u32 affinity);
// wake up redistributor
extern void wakeUpRedistributor(GICR redists, u32 redist_id);

/* INTERRUPT CONTROL */

extern void int_set_priority(GICD dist, GICR redists, u32 int_id, u32 redist_id,
                             u8 priority);
extern void int_set_group(GICD dist, GICR redists, u32 int_id, u32 redist_id,
                          u8 security);
extern void int_set_triger(GICD dist, GICR redists, u32 int_id, u32 redist_id,
                           u8 triger);
extern void int_set_route(GICD dist, u32 int_id, u32 affinity);
extern void int_enable(GICD dist, GICR redists, u32 int_id, u32 redist_id);

/* GIC CPU INTERFACE */

// set interrupt priority mask
extern void setPriorityMask(u8 mask);

// enable support for interrupts
extern void enableGroup0Ints(void);
extern void enableGroup1Ints(void);

// configure current PE interface for interrupts e.g. enable SRE
extern void configureICC(void);

#endif
