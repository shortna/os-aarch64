#ifndef _RTC_DEFS_H
#define _RTC_DEFS_H

#include "types.h"

#define RTC_IM_SET_MASK (1)
#define RTC_IM_CLR_MASK (0)
#define RTC_INT_CLR     (1)

struct RTC {
  const volatile uint32_t DR;                                            /* +0x000 */
  volatile uint32_t MR;                                                  /* +0x004 */
  volatile uint32_t LR;                                                  /* +0x008 */
  volatile uint32_t CR;                                                  /* +0x00C */
  volatile uint32_t IMSC;                                                /* +0x010 */
  const volatile uint32_t RIS;                                           /* +0x014 */
  const volatile uint32_t MIS;                                           /* +0x018 */
  volatile uint32_t ICR;                                                 /* +0x01C */
  const volatile uint8_t padding_0[96];                                  /* +0x020 */
  volatile uint32_t ITCR;                                                /* +0x080 */
  volatile uint32_t ITIP;                                                /* +0x084 */
  volatile uint32_t ITOP;                                                /* +0x088 */
  volatile uint32_t TOFFSET;                                             /* +0x08C */
  volatile uint32_t TCOUNT;                                              /* +0x090 */
  const volatile uint8_t padding_1[3916];                                /* +0x094 */
  const volatile uint32_t PeriphID0;                                     /* +0xFE0 */
  const volatile uint32_t PeriphID1;                                     /* +0xFE4 */
  const volatile uint32_t PeriphID2;                                     /* +0xFE8 */
  const volatile uint32_t PeriphID3;                                     /* +0xFEC */
  const volatile uint32_t CellID0;                                       /* +0xFF0 */
  const volatile uint32_t CellID1;                                       /* +0xFF4 */
  const volatile uint32_t CellID2;                                       /* +0xFF8 */
  const volatile uint32_t CellID3;                                       /* +0xFFC */
};

#endif
