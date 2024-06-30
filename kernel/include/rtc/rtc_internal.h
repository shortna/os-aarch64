#ifndef _RTC_DEFS_H
#define _RTC_DEFS_H

#include "types.h"

#define RTC_IM_SET_MASK (1)
#define RTC_IM_CLR_MASK (0)
#define RTC_INT_CLR     (1)

struct RTC {
  const volatile u32 DR;                                            /* +0x000 */
  volatile u32 MR;                                                  /* +0x004 */
  volatile u32 LR;                                                  /* +0x008 */
  volatile u32 CR;                                                  /* +0x00C */
  volatile u32 IMSC;                                                /* +0x010 */
  const volatile u32 RIS;                                           /* +0x014 */
  const volatile u32 MIS;                                           /* +0x018 */
  volatile u32 ICR;                                                 /* +0x01C */
  const volatile u8 padding_0[96];                                  /* +0x020 */
  volatile u32 ITCR;                                                /* +0x080 */
  volatile u32 ITIP;                                                /* +0x084 */
  volatile u32 ITOP;                                                /* +0x088 */
  volatile u32 TOFFSET;                                             /* +0x08C */
  volatile u32 TCOUNT;                                              /* +0x090 */
  const volatile u8 padding_1[3916];                                /* +0x094 */
  const volatile u32 PeriphID0;                                     /* +0xFE0 */
  const volatile u32 PeriphID1;                                     /* +0xFE4 */
  const volatile u32 PeriphID2;                                     /* +0xFE8 */
  const volatile u32 PeriphID3;                                     /* +0xFEC */
  const volatile u32 CellID0;                                       /* +0xFF0 */
  const volatile u32 CellID1;                                       /* +0xFF4 */
  const volatile u32 CellID2;                                       /* +0xFF8 */
  const volatile u32 CellID3;                                       /* +0xFFC */
};

#endif
