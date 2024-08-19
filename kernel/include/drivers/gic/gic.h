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
  uint8_t priority;
  uint8_t security;
  uint8_t triger;
  bool route;
  uint32_t affinity;
};

extern void register_interrupt(GICD dist, GICR redists, uint32_t int_id,
                               struct InterruptParameters p);

// get current PE
extern uint32_t get_affinity(void);

// returns pointer to distributor
extern GICD get_distributor(uint64_t gicd_base);
// returns pointer to redistributors (including sgis and lpis)
extern GICR get_redistributor(uint64_t gicr_base, uint32_t redist_id);

// enable interrupt controller
extern uint8_t gic_init(GICD d);

// find redistributor that belongs to the specified affinity
extern uint32_t get_redistributor_id(uint64_t gicr_base, uint32_t affinity);
// wake up redistributor
extern void wake_redistributor(GICR redist);

/* INTERRUPT CONTROL */

extern void int_set_priority(GICD dist, GICR redist, uint32_t int_id, uint8_t priority);
extern void int_set_group(GICD dist, GICR redist, uint32_t int_id, uint8_t security);
extern void int_set_triger(GICD dist, GICR redist, uint32_t int_id, uint8_t triger);
extern void int_set_route(GICD dist, uint32_t int_id, uint32_t affinity);
extern void int_enable(GICD dist, GICR redist, uint32_t int_id);

/* GIC CPU INTERFACE */

// set interrupt priority mask
extern void set_priority_mask(uint8_t mask);

// enable support for interrupts
extern void enable_group0_ints(void);
extern void enable_group1_ints(void);

// configure current PE interface for interrupts e.g. enable SRE
extern void configure_icc(void);

#endif
