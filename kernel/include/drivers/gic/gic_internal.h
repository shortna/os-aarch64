#ifndef _GIC_DEFS_H
#define _GIC_DEFS_H

#include "types.h"

// addresses are inclusive e.g 0x80 - 0xFC, 0xFC is start of the last member
struct GICDv3 {
  volatile uint32_t CTLR;                                               /* +0x0000 */
  const volatile uint32_t TYPER;                                        /* +0x0004 */
  const volatile uint32_t IIDR;                                         /* +0x0008 */

  /* when FEAT_GICv4p1 is implemented, otherwise - RES0 */         
  const volatile uint32_t TYPER2;                                       /* +0x000C */
  /**/     

  volatile uint32_t STATUSR;                                            /* +0x0010 */
  const volatile uint8_t padding_0[12];                                 /* +0x0014 */
  volatile uint8_t IMPLEMETATION_DEFINED_0[32];                         /* +0x0020 */
  volatile uint32_t SETSPI_NSR;                                         /* +0x0040 */
  const volatile uint32_t padding_1;                                    /* +0x0044 */
  volatile uint32_t CLRSPI_NSR;                                         /* +0x0048 */
  const volatile uint32_t padding_2;                                    /* +0x004C */
  volatile uint32_t SETSPI_SR;                                          /* +0x0050 */
  const volatile uint32_t padding_3;                                    /* +0x0054 */
  volatile uint32_t CLRSPI_SR;                                          /* +0x0058 */
  const volatile uint8_t padding_4[36];                                 /* +0x005C */

  /* ARRAYS */
  volatile uint32_t IGROUPR[32];                                        /* +0x0080 */
  volatile uint32_t ISENABLER[32];                                      /* +0x0100 */
  volatile uint32_t ICENABLER[32];                                      /* +0x0180 */
  volatile uint32_t ISPENDR[32];                                        /* +0x0200 */
  volatile uint32_t ICPENDR[32];                                        /* +0x0280 */
  volatile uint32_t ISACTIVER[32];                                      /* +0x0300 */
  volatile uint32_t ICACTIVER[32];                                      /* +0x0380 */
  volatile uint8_t IPRIORITYR[1024];                                    /* +0x0400 */
  volatile uint8_t ITARGETSR[1024];                                     /* +0x0800 */
  volatile uint32_t ICFGR[64];                                          /* +0x0C00 */
  volatile uint32_t IGRPMODR[32];                                       /* +0x0D00 */
  const volatile uint8_t padding_5[128];                                /* +0x0D80 */
  volatile uint32_t NSACR[64];                                          /* +0x0E00 */
  volatile uint32_t SGIR;                                               /* +0x0F00 */
  const volatile uint8_t padding_6[12];                                 /* +0x0F04 */
  volatile uint32_t CPENDSGIR[4];                                       /* +0x0F10 */
  volatile uint32_t SPENDSGIR[4];                                       /* +0x0F20 */
  const volatile uint8_t padding_7[80];                                 /* +0x0F30 */
  volatile uint32_t INMIR[32];                                          /* +0x0F80 */

  /* only when FEAT_GICv3p1 is implemented, otherwise RES0 */
  volatile uint32_t IGROUPR_E[128];                                     /* +0x1000 */
  volatile uint32_t ISENABLER_E[128];                                   /* +0x1200 */
  volatile uint32_t ICENABLER_E[128];                                   /* +0x1400 */
  volatile uint32_t ISPENDR_E[128];                                     /* +0x1600 */
  volatile uint32_t ICPENDR_E[128];                                     /* +0x1800 */
  volatile uint32_t ISACTIVER_E[128];                                   /* +0x1A00 */
  volatile uint32_t ICACTIVER_E[128];                                   /* +0x1C00 */
  const volatile uint8_t padding_8[512];                                /* +0x1E00 */
  volatile uint8_t IPRIORITYR_E[4096];                                  /* +0x2000 */
  volatile uint32_t ICFGR_E[256];                                       /* +0x3000 */
  volatile uint32_t IGRPMODR_E[128];                                    /* +0x3400 */
  volatile uint32_t NSACR_E[64];                                        /* +0x3600 */
  const volatile uint8_t padding_9[1024];                               /* +0x3700 */
  volatile uint32_t INMIR_E[32];                                        /* +0x3B00 */
  /**/
  const volatile uint8_t padding_10[9344];                              /* +0x3B80 */

  volatile uint64_t GICD_IROUTER[1024];                                 /* +0x6000 */

  /* only when FEAT_GICv3p1 is implemented, otherwise RES0 */
  volatile uint64_t GICD_IROUTER_E[1024];                               /* +0x8000 */
  /**/

  const volatile uint8_t padding_11[8192];                              /* +0xA000 */
  volatile uint8_t IMPLEMENTATION_DEFINED_1[16336];                     /* +0xC000 */
  const volatile uint8_t IMPLEMENTATION_DEFINED_2[48];                  /* +0xFFD0 */
};

struct GICv3RedistributorLpis {
  volatile uint32_t CTLR;                                              /* +0x0000 */
  const volatile uint32_t IIDR;                                        /* +0x0004 */
  const volatile uint32_t TYPER[2];                                    /* +0x0008 */
  volatile uint32_t STATUSR;                                           /* +0x0010 */
  volatile uint32_t WAKER;                                             /* +0x0014 */
  const volatile uint32_t MPAMIDR;                                     /* +0x0018 */
  volatile uint32_t PARTIDR;                                           /* +0x001C */
  const volatile uint8_t padding0[32];                                 /* +0x0020 */
  volatile uint64_t SETLPIR;                                           /* +0x0040 */
  volatile uint64_t CLRLPIR;                                           /* +0x0048 */
  const volatile uint8_t padding1[32];                                 /* +0x0050 */
  volatile uint64_t PROPBASER;                                         /* +0x0070 */
  volatile uint64_t PENDBASER;                                         /* +0x0078 */
  const volatile uint8_t padding2[32];                                 /* +0x0080 */
  volatile uint64_t INVLPIR;                                           /* +0x00A0 */
  const volatile uint8_t padding3[8];                                  /* +0x00A8 */
  volatile uint64_t INVALLR;                                           /* +0x00B0 */
  const volatile uint8_t padding4[8];                                  /* +0x00B8 */
  volatile uint32_t SYNCR;                                             /* +0x00C0 */
  const volatile uint8_t padding5[60];                                 /* +0x00C4 */
  volatile uint8_t IMPLEMENTATION_DEFINED_0[8];                        /* +0x0100 */
  const volatile uint8_t padding6[8];                                  /* +0x0108 */
  volatile uint8_t IMPLEMENTATION_DEFINED_1[8];                        /* +0x0110 */
  const volatile uint8_t padding7[48872];                              /* +0x0118 */
  volatile uint8_t IMPLEMENTATION_DEFINED_2[16336];                    /* +0xC000 */
  volatile uint8_t IMPLEMENTATION_DEFINED_3[48];                       /* +0xFFD0 */
};

struct GICv3RedistributorSgis {
  const volatile uint8_t padding_0[128];                                /* +0x0000 */
  volatile uint32_t IGROUPR[3];                                         /* +0x0080 */
  const volatile uint8_t padding_1[116];                                /* +0x008C */
  volatile uint32_t ISENABLER[3];                                       /* +0x0100 */
  const volatile uint8_t padding_2[116];                                /* +0x010C */
  volatile uint32_t ICENABLER[3];                                       /* +0x0180 */
  const volatile uint8_t padding_3[116];                                /* +0x018C */
  volatile uint32_t ISPENDR[3];                                         /* +0x0200 */
  const volatile uint8_t padding_4[116];                                /* +0x020C */
  volatile uint32_t ICPENDR[3];                                         /* +0x0280 */
  const volatile uint8_t padding_5[116];                                /* +0x028C */
  volatile uint32_t ISACTIVER[3];                                       /* +0x0300 */
  const volatile uint8_t padding_6[116];                                /* +0x030C */
  volatile uint32_t ICACTIVER[3];                                       /* +0x0380 */
  const volatile uint8_t padding_7[116];                                /* +0x038C */
  volatile uint8_t IPRIORITYR[96];                                      /* +0x0400 */
  const volatile uint8_t padding_8[1952];                               /* +0x0460 */
  volatile uint32_t ICFGR[6];                                           /* +0x0C00 */
  const volatile uint8_t padding_9[232];                                /* +0x0C18 */
  volatile uint32_t IGRPMODR[3];                                        /* +0x0D00 */
  const volatile uint8_t padding_10[244];                               /* +0x0D0C */
  volatile uint32_t NSACR;                                              /* +0x0E00 */
  const volatile uint8_t padding_11[380];                               /* +0x0E04 */
  volatile uint32_t INMIR[32];                                          /* +0x0F80 */
  const volatile uint8_t padding_12[45056];                             /* +0x1000 */
  volatile uint8_t IMPLEMENTATION_DEFINED[16336];                       /* +0xC000 */
  const volatile uint8_t padding_13[48];                                /* +0xFFD0 */
};

struct GICRv3 {
  /* each structure is 64Kb, so allign accordingly */
  struct GICv3RedistributorLpis lpis __attribute__((aligned(0x10000)));
  struct GICv3RedistributorSgis sgis __attribute__((aligned(0x10000)));
};

#endif
