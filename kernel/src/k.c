#include "uart/uart.h"
#define UART_BAUD_RATE U64(115200)
#define UART_BASE_ADDRESS U64(0x9000000)

#define GICD_BASE_ADDRESS U64(0x8000000)
#define GICR_BASE_ADDRESS U64(0x80a0000)

#define GICV3_GROUP0             (0)
#define GICV3_GROUP1_SECURE      (1)
#define GICV3_GROUP1_NON_SECURE  (2)

#define GICV3_LEVEL_SENSITIVE (0)
#define GICV3_EDGE_TRIGGERED  (2)

/* GICD offset table */
#define GICD_CTLR       (0x0000)
#define GICD_IPRIORITYR (0x0400)
#define GICD_IGROUPR    (0x0080)
#define GICD_IGRPMOD    (0x00D0)
#define GICD_ICFGR      (0x00C0)
#define GICD_ISENABLER  (0x0100)
#define GICD_ICENABLER  (0x0180)
#define GICD_IROUTER    (0x6000)

/* GICR offset table */
#define GICR_WAKER (0x0014)
#define GICR_TYPER (0x0008)
#define GICR_FRAME_SIZE (0x20000) // 128kb

extern u32 getAffinity(void);
extern void setPriorityMask(u8 mask);
extern void enableGroup0Ints();
extern void enableGroup1Ints();
extern void configureICC();

u32 getRedistributorId(u32 affinity) {
  u32 current;
  u32 reg;
  u64 offset = GICR_TYPER;

  do {
    current = mmio_register_read((void *)GICR_BASE_ADDRESS, offset);
    reg = mmio_register_read((void *)GICR_BASE_ADDRESS, offset + 4);
    if (reg == affinity) {
      return reg;
    }
    offset += GICR_FRAME_SIZE;
  } while ((current & BIT(4)) == 0);

  return UINT32_MAX;
}

void wakeUpRedistributor(u32 redistributor_id) {
  u32 offset = GICR_WAKER + redistributor_id * GICR_FRAME_SIZE;
  u32 reg = mmio_register_read((void *)GICR_BASE_ADDRESS, offset);
  reg &= ~BIT(1);
  mmio_register_write((void *)GICR_BASE_ADDRESS, offset, reg);

  do {
    reg = mmio_register_read((void *)GICR_BASE_ADDRESS, offset);
  } while ((reg & BIT(2)) != 0);
}

// system support two security states
// access not secure
void enableGIC(void) {
  u32 reg = mmio_register_read((void *)GICD_BASE_ADDRESS, GICD_CTLR);
  reg |= BIT(4) | BIT(1);
  mmio_register_write((void *)GICD_BASE_ADDRESS, GICD_CTLR, reg);
}

// one byte is priority for one interrupt e.g priority of INTID 32 - byte 32
void spi_set_priority(u32 id, u8 priority) {
  u32 offset = (id / 4) * 4;
  id = id & 0x3;
  id = id * 8;

  u32 p = mmio_register_read((void *)GICD_BASE_ADDRESS, GICD_IPRIORITYR + offset);
  p = bits_clear(p, id, 8);
  p |= priority << id;
  mmio_register_write((void *)GICD_BASE_ADDRESS, GICD_IPRIORITYR + offset, p);
};

// one bit per interrupt e.g group of INTID 32 - bit 32
void spi_set_group(u32 id, u8 security) {
  u32 offset = (id / 32) * 4;
  u32 group, mod;

  group = mmio_register_read((void *)GICD_BASE_ADDRESS, GICD_IGROUPR + offset);
  mod = mmio_register_read((void *)GICD_BASE_ADDRESS, GICD_IGRPMOD + offset);

  // get positon of bit that responsible for interrupt (same as id % 32)
  id = id & 0x1f; 
  id = 1 << id;   // set bit at that position

  switch (security) {
  case GICV3_GROUP0:
    group = (group & ~id);
    mod = (mod & ~id);
    break;
  case GICV3_GROUP1_SECURE:
    group = (group & ~id);
    mod = (mod | id);
    break;
  case GICV3_GROUP1_NON_SECURE:
    group = (group | id);
    mod = (mod & ~id);
    break;
  default:
    return;
  }

  mmio_register_write((void *)GICD_BASE_ADDRESS, GICD_IGROUPR + offset, group);
  mmio_register_write((void *)GICD_BASE_ADDRESS, GICD_IGRPMOD + offset, mod);
}

// two bits per interrupt e.g INTID 32 - bits [65:66] (first 2 bits of a 3 byte)
void spi_set_triger(u32 id, u8 trigger) {
  u32 offset = (id / 16) * 4;
  u32 t;

  t = mmio_register_read((void *)GICD_BASE_ADDRESS, GICD_ICFGR + offset);

  // get positon of bit that responsible for interrupt (same as id % 16)
  id &= 0xF; 
  id *= 2;   // get bit offset
  t = bits_clear(t, id, 2); // clear bit
  t |= trigger;

  mmio_register_write((void *)GICD_BASE_ADDRESS, GICD_ICFGR + offset, t);
}

void spi_set_route(u32 id, u32 affinity) {
  u32 offset = (id - 32) * 8;
  u32 irouter_h, irouter_l;
  
  irouter_h = mmio_register_read((void *)GICD_BASE_ADDRESS, GICD_ICFGR + offset);
  irouter_l = mmio_register_read((void *)GICD_BASE_ADDRESS, GICD_ICFGR + offset + 4);

  irouter_h = bits_clear(irouter_h, 32, 8);
  irouter_h |= affinity & (0xff << 24); // extracn affinity 3

  irouter_l = bits_clear(irouter_l, 0, 24);
  irouter_l = bits_clear(irouter_l, 31, 1);

  irouter_l |= affinity & ~(0xff << 24);

  mmio_register_write((void *)GICD_BASE_ADDRESS, GICD_ICFGR + offset, irouter_h);
  mmio_register_write((void *)GICD_BASE_ADDRESS, GICD_ICFGR + offset + 4, irouter_l);
}

void spi_enable(u32 id) {
  u32 offset = (id / 32) * 4;
  u32 e;

  e = mmio_register_read((void *)GICD_BASE_ADDRESS, GICD_ISENABLER + offset);

  // get positon of bit that responsible for interrupt (same as id % 32)
  id = id & 0x1f; 
  id = 1 << id;   // set bit at that position

  e |= id;
    
  mmio_register_write((void *)GICD_BASE_ADDRESS, GICD_ISENABLER + offset, e);
}

void kmain() {
  enableGIC();

  u32 redistributor_id = getRedistributorId(getAffinity());
  if (redistributor_id == UINT32_MAX) {
    while (1);
  }

  wakeUpRedistributor(redistributor_id);

  // enable SRE bits 
  configureICC();
  // Configure the CPU interface
  // This assumes that the SRE bits are already set
  setPriorityMask(0xFF);
  enableGroup0Ints();
  enableGroup1Ints();

  uart_setup((void *)UART_BASE_ADDRESS, UART_BAUD_RATE);
  u8 c = 'A';
  while (c != 'z') {
    uart_write_byte(c);
    c++;
  }

  while (1);
}
