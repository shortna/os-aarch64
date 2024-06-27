#ifndef _GIC_DEFS_H
#define _GIC_DEFS_H

#include "types.h"

// addresses are inclusive e.g 0x80 - 0xFC, 0xFC is start of the last member
struct GICDv3 {
  volatile u32 CTLR;                                               /* +0x0000 */
  const volatile u32 TYPER;                                        /* +0x0004 */
  const volatile u32 IIDR;                                         /* +0x0008 */

  /* when FEAT_GICv4p1 is implemented, otherwise - RES0 */         
  const volatile u32 TYPER2;                                       /* +0x000C */
  /**/     

  volatile u32 STATUSR;                                            /* +0x0010 */
  const volatile u8 padding_0[12];                                 /* +0x0014 */
  volatile u8 IMPLEMETATION_DEFINED_0[32];                         /* +0x0020 */
  volatile u32 SETSPI_NSR;                                         /* +0x0040 */
  const volatile u32 padding_1;                                    /* +0x0044 */
  volatile u32 CLRSPI_NSR;                                         /* +0x0048 */
  const volatile u32 padding_2;                                    /* +0x004C */
  volatile u32 SETSPI_SR;                                          /* +0x0050 */
  const volatile u32 padding_3;                                    /* +0x0054 */
  volatile u32 CLRSPI_SR;                                          /* +0x0058 */
  const volatile u8 padding_4[36];                                 /* +0x005C */

  /* ARRAYS */
  volatile u32 IGROUPR[32];                                        /* +0x0080 */
  volatile u32 ISENABLER[32];                                      /* +0x0100 */
  volatile u32 ICENABLER[32];                                      /* +0x0180 */
  volatile u32 ISPENDR[32];                                        /* +0x0200 */
  volatile u32 ICPENDR[32];                                        /* +0x0280 */
  volatile u32 ISACTIVER[32];                                      /* +0x0300 */
  volatile u32 ICACTIVER[32];                                      /* +0x0380 */
  volatile u8 IPRIORITYR[1024];                                    /* +0x0400 */
  volatile u8 ITARGETSR[1024];                                     /* +0x0800 */
  volatile u32 ICFGR[64];                                          /* +0x0C00 */
  volatile u32 IGRPMODR[32];                                       /* +0x0D00 */
  const volatile u8 padding_5[128];                                /* +0x0D80 */
  volatile u32 NSACR[64];                                          /* +0x0E00 */
  volatile u32 SGIR;                                               /* +0x0F00 */
  const volatile u8 padding_6[12];                                 /* +0x0F04 */
  volatile u32 CPENDSGIR[4];                                       /* +0x0F10 */
  volatile u32 SPENDSGIR[4];                                       /* +0x0F20 */
  const volatile u8 padding_7[80];                                 /* +0x0F30 */
  volatile u32 INMIR[32];                                          /* +0x0F80 */

  /* only when FEAT_GICv3p1 is implemented, otherwise RES0 */
  volatile u32 IGROUPR_E[128];                                     /* +0x1000 */
  volatile u32 ISENABLER_E[128];                                   /* +0x1200 */
  volatile u32 ICENABLER_E[128];                                   /* +0x1400 */
  volatile u32 ISPENDR_E[128];                                     /* +0x1600 */
  volatile u32 ICPENDR_E[128];                                     /* +0x1800 */
  volatile u32 ISACTIVER_E[128];                                   /* +0x1A00 */
  volatile u32 ICACTIVER_E[128];                                   /* +0x1C00 */
  const volatile u8 padding_8[512];                                /* +0x1E00 */
  volatile u8 IPRIORITYR_E[4096];                                  /* +0x2000 */
  volatile u32 ICFGR_E[256];                                       /* +0x3000 */
  volatile u32 IGRPMODR_E[128];                                    /* +0x3400 */
  volatile u32 NSACR_E[64];                                        /* +0x3600 */
  const volatile u8 padding_9[1024];                               /* +0x3700 */
  volatile u32 INMIR_E[32];                                        /* +0x3B00 */
  /**/
  const volatile u8 padding_10[9344];                              /* +0x3B80 */

  volatile u64 GICD_IROUTER[1024];                                 /* +0x6000 */

  /* only when FEAT_GICv3p1 is implemented, otherwise RES0 */
  volatile u64 GICD_IROUTER_E[1024];                               /* +0x8000 */
  /**/

  const volatile u8 padding_11[8192];                              /* +0xA000 */
  volatile u8 IMPLEMENTATION_DEFINED_1[16336];                     /* +0xC000 */
  const volatile u8 IMPLEMENTATION_DEFINED_2[48];                  /* +0xFFD0 */
};

struct Redistributor_GICv3_lpis {
  volatile u32 CTLR;                                              /* +0x0000 */
  const volatile u32 IIDR;                                        /* +0x0004 */
  const volatile u32 TYPER[2];                                    /* +0x0008 */
  volatile u32 STATUSR;                                           /* +0x0010 */
  volatile u32 WAKER;                                             /* +0x0014 */
  const volatile u32 MPAMIDR;                                     /* +0x0018 */
  volatile u32 PARTIDR;                                           /* +0x001C */
  const volatile u8 padding0[32];                                 /* +0x0020 */
  volatile u64 SETLPIR;                                           /* +0x0040 */
  volatile u64 CLRLPIR;                                           /* +0x0048 */
  const volatile u8 padding1[32];                                 /* +0x0050 */
  volatile u64 PROPBASER;                                         /* +0x0070 */
  volatile u64 PENDBASER;                                         /* +0x0078 */
  const volatile u8 padding2[32];                                 /* +0x0080 */
  volatile u64 INVLPIR;                                           /* +0x00A0 */
  const volatile u8 padding3[8];                                  /* +0x00A8 */
  volatile u64 INVALLR;                                           /* +0x00B0 */
  const volatile u8 padding4[8];                                  /* +0x00B8 */
  volatile u32 SYNCR;                                             /* +0x00C0 */
  const volatile u8 padding5[60];                                 /* +0x00C4 */
  volatile u8 IMPLEMENTATION_DEFINED_0[8];                        /* +0x0100 */
  const volatile u8 padding6[8];                                  /* +0x0108 */
  volatile u8 IMPLEMENTATION_DEFINED_1[8];                        /* +0x0110 */
  const volatile u8 padding7[48872];                              /* +0x0118 */
  volatile u8 IMPLEMENTATION_DEFINED_2[16336];                    /* +0xC000 */
  volatile u8 IMPLEMENTATION_DEFINED_3[48];                       /* +0xFFD0 */
};

struct Redistributor_GICv3_sgis {
  const volatile u8 padding_0[128];                                /* +0x0000 */
  volatile u32 IGROUPR[3];                                         /* +0x0080 */
  const volatile u8 padding_1[116];                                /* +0x008C */
  volatile u32 ISENABLER[3];                                       /* +0x0100 */
  const volatile u8 padding_2[116];                                /* +0x010C */
  volatile u32 ICENABLER[3];                                       /* +0x0180 */
  const volatile u8 padding_3[116];                                /* +0x018C */
  volatile u32 ISPENDR[3];                                         /* +0x0200 */
  const volatile u8 padding_4[116];                                /* +0x020C */
  volatile u32 ICPENDR[3];                                         /* +0x0280 */
  const volatile u8 padding_5[116];                                /* +0x028C */
  volatile u32 ISACTIVER[3];                                       /* +0x0300 */
  const volatile u8 padding_6[116];                                /* +0x030C */
  volatile u32 ICACTIVER[3];                                       /* +0x0380 */
  const volatile u8 padding_7[116];                                /* +0x038C */
  volatile u8 IPRIORITYR[96];                                      /* +0x0400 */
  const volatile u8 padding_8[1952];                               /* +0x0460 */
  volatile u32 ICFGR[6];                                           /* +0x0C00 */
  const volatile u8 padding_9[232];                                /* +0x0C18 */
  volatile u32 IGRPMODR[3];                                        /* +0x0D00 */
  const volatile u8 padding_10[244];                               /* +0x0D0C */
  volatile u32 NSACR;                                              /* +0x0E00 */
  const volatile u8 padding_11[380];                               /* +0x0E04 */
  volatile u32 INMIR[32];                                          /* +0x0F80 */
  const volatile u8 padding_12[45056];                             /* +0x1000 */
  volatile u8 IMPLEMENTATION_DEFINED[16336];                       /* +0xC000 */
  const volatile u8 padding_13[48];                                /* +0xFFD0 */
};

struct GICRv3 {
  /* each structure is 64Kb, so allign accordingly */
  struct Redistributor_GICv3_lpis lpis __attribute__((aligned(0x10000)));
  struct Redistributor_GICv3_sgis sgis __attribute__((aligned(0x10000)));
};

#endif
