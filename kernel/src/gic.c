#include "gic/gic.h"
#include "gic/gic_internal.h"

u32 getRedistributorId(GICR redists, u32 affinity) {
  u32 ind = 0;

  do {
    if (redists[ind].lpis.TYPER[1] == affinity) {
      return ind;
    }
    ind++;
  } while ((redists[ind].lpis.TYPER[0] & BIT(4)) == 0);

  return UINT32_MAX;
}

void wakeUpRedistributor(GICR redists, u32 redist_id) {
  u32 tmp = redists[redist_id].lpis.WAKER;
  tmp &= ~BIT(1);
  redists[redist_id].lpis.WAKER = tmp;

  do {
    tmp = redists[redist_id].lpis.WAKER;
  } while ((tmp & BIT(2)) != 0);
}

// system support two security states
// access not secure
u8 enableGIC(GICD dist) {
  if (dist == 0) {
    return 0;
  }

  u32 c = dist->CTLR;
  c |= BIT(4); // set ARE_NS
  dist->CTLR = c;

  c |= BIT(1); // Enable group1NS
  dist->CTLR = c;
  return 1;
}

void int_set_priority(GICD dist, GICR redists, u32 int_id, u32 redist_id,
                      u8 priority) {
  u32 bank, offset; // offset within register
  bank = int_id / 4;
  offset = int_id & 0x3;
  if (int_id < 31) {
    redists[redist_id].sgis.IPRIORITYR[bank + offset] = priority;
  } else if (int_id < 1020) {
    dist->IPRIORITYR[bank + offset] = priority;
  }
};

void int_set_group(GICD dist, GICR redists, u32 int_id, u32 redist_id,
                   u8 security) {
  u32 bank, bit, group, mod;
  bank = int_id / 32;
  bit = int_id & 0x1f;
  bit = 1 << bit;

  if (int_id < 31) {
    group = redists[redist_id].sgis.IGROUPR[bank];
    mod = redists[redist_id].sgis.IGRPMODR[bank];
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
    redists[redist_id].sgis.IGROUPR[bank] = group;
    redists[redist_id].sgis.IGRPMODR[bank] = mod;
  } else {
    dist->IGROUPR[bank] = group;
    dist->IGRPMODR[bank] = mod;
  }
}

void int_set_triger(GICD dist, GICR redists, u32 int_id, u32 redist_id,
                     u8 triger) {
  u32 bank, bit, tr;
  bank = int_id / 16;
  bit = int_id & 0xf;
  bit *= 2;

  if (int_id < 31) {
    tr = redists[redist_id].sgis.ICFGR[bank];
  } else if (int_id < 1020) {
    tr = dist->ICFGR[bank];
  } else {
    return;
  }

  tr = bits_clear(tr, bit, 2);
  tr |= triger << bit;

  if (int_id < 31) {
    redists[redist_id].sgis.ICFGR[bank] = tr;
  } else if (int_id < 1020) {
    dist->ICFGR[bank] = tr;
  }
}

void int_set_route(GICD dist, u32 int_id, u32 affinity) {
  dist->GICD_IROUTER[int_id] = (U64(affinity & 0xff000000) << 32) |
                               U64(1 << 31) | U64(affinity & 0x00ffffff);
}

void int_enable(GICD dist, GICR redists, u32 int_id, u32 redist_id) {
  u32 bank, bit;
  bank = int_id / 32;
  bit = int_id & 0x1f;
  bit = 1 << bit;

  if (int_id < 31) {
    redists[redist_id].sgis.ISENABLER[bank] |= bit;
  } else if (int_id < 1020) {
    dist->ISENABLER[bank] |= bit;
  }
}

GICD gicd(u64 gicd_base) { return (GICD)gicd_base; }

GICR gicr(u64 gicr_base) { return (GICR)gicr_base; }
