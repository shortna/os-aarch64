#include "drivers/gic/gic.h"
#include "drivers/gic/gic_internal.h"
#include "bits.h"

// system support two security states
// access not secure
uint8_t gic_init(GICD dist) {
  if (dist == 0) {
    return 0;
  }

  uint32_t c = dist->CTLR;
  c |= BIT(4); // set ARE_NS
  dist->CTLR = c;

  c |= BIT(1); // Enable group1NS
  dist->CTLR = c;

  // enable SRE bits
  configure_icc();
  return 1;
}

uint32_t get_redistributor_id(uint64_t base_address, uint32_t affinity) {
  uint32_t ind = 0;
  struct GICRv3 *redists = (void *)base_address;

  do {
    if (redists[ind].lpis.TYPER[1] == affinity) {
      return ind;
    }
    ind++;
  } while ((redists[ind].lpis.TYPER[0] & BIT(4)) == 0);

  return UINT32_MAX;
}

void wake_redistributor(GICR redist) {
  uint32_t tmp = redist->lpis.WAKER;
  tmp &= ~BIT(1);
  redist->lpis.WAKER = tmp;

  do {
    tmp = redist->lpis.WAKER;
  } while ((tmp & BIT(2)) != 0);
}

void int_set_priority(GICD dist, GICR redist, uint32_t int_id, uint8_t priority) {
  uint32_t bank, offset; // offset within register
  bank = int_id / 4;
  offset = int_id & 0x3;
  if (int_id < 31) {
    redist->sgis.IPRIORITYR[bank + offset] = priority;
  } else if (int_id < 1020) {
    dist->IPRIORITYR[bank + offset] = priority;
  }
};

void int_set_group(GICD dist, GICR redist, uint32_t int_id, uint8_t security) {
  uint32_t bank, bit, group, mod;
  bank = int_id / 32;
  bit = int_id & 0x1f;
  bit = 1 << bit;

  if (int_id < 31) {
    group = redist->sgis.IGROUPR[bank];
    mod = redist->sgis.IGRPMODR[bank];
  } else if (int_id < 1020) {
    group = dist->IGROUPR[bank];
    mod = dist->IGRPMODR[bank];
  } else {
    return;
  }

  switch (security) {
  case GICV3_GROUP0:
    group &= ~bit;
    mod &= ~bit;
    break;
  case GICV3_GROUP1_SECURE:
    group &= ~bit;
    mod |= bit;
    break;
  case GICV3_GROUP1_NON_SECURE:
    group |= bit;
    mod &= ~bit;
    break;
  default:
    return;
  }

  if (int_id < 31) {
    redist->sgis.IGROUPR[bank] = group;
    redist->sgis.IGRPMODR[bank] = mod;
  } else {
    dist->IGROUPR[bank] = group;
    dist->IGRPMODR[bank] = mod;
  }
}

void int_set_triger(GICD dist, GICR redist, uint32_t int_id, uint8_t triger) {
  uint32_t bank, bit, tr;
  bank = int_id / 16;
  bit = int_id & 0xf;
  bit *= 2;

  if (int_id < 31) {
    tr = redist->sgis.ICFGR[bank];
  } else if (int_id < 1020) {
    tr = dist->ICFGR[bank];
  } else {
    return;
  }

  tr = bits_clear(tr, bit, 2);
  tr |= triger << bit;

  if (int_id < 31) {
    redist->sgis.ICFGR[bank] = tr;
  } else if (int_id < 1020) {
    dist->ICFGR[bank] = tr;
  }
}

void int_set_route(GICD dist, uint32_t int_id, uint32_t affinity) {
  uint64_t int_route = dist->GICD_IROUTER[int_id];

  int_route &= ~BIT(31); // enable routing mode
  int_route = bits_clear(int_route, 32, 8);
  int_route = bits_clear(int_route, 0, 24);
  int_route |= ((uint64_t)(affinity & 0xff000000) << 32) | (uint64_t)(affinity & 0x00ffffff);

  dist->GICD_IROUTER[int_id] = int_route;
}

void int_enable(GICD dist, GICR redist, uint32_t int_id) {
  uint32_t bank, bit;
  bank = int_id / 32;
  bit = int_id & 0x1f;
  bit = 1 << bit;

  if (int_id < 31) {
    redist->sgis.ISENABLER[bank] |= bit;
  } else if (int_id < 1020) {
    dist->ISENABLER[bank] |= bit;
  }
}

GICD get_distributor(uint64_t gicd_base) { return (GICD)gicd_base; }

GICR get_redistributor(uint64_t gicr_base, uint32_t redist_id) {
  return ((GICR)gicr_base) + redist_id;
}

void register_interrupt(GICD dist, GICR redist, uint32_t int_id,
                        struct InterruptParameters p) {
  int_set_priority(dist, redist, int_id, p.priority);
  int_set_group(dist, redist, int_id, p.security);
  int_set_triger(dist, redist, int_id, p.triger);
  if (p.route) {
    int_set_route(dist, int_id, p.affinity);
  }
  int_enable(dist, redist, int_id);
}
