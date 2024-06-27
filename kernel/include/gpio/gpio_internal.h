#ifndef _GPIO_DEFS_H
#define _GPIO_DEFS_H

#include "types.h"

#define GPIO_INTERRUPTS_CLEAR (0xff)

struct GPIO {
  volatile u8 DATA;                                                 /* +0x000 */
  const volatile u8 padding_0[1023];                                /* +0x001 */
  volatile u8 DIR;                                                  /* +0x400 */
  const volatile u8 padding_1[3];                                   /* +0x401 */
  volatile u8 IS;                                                   /* +0x404 */
  const volatile u8 padding_2[3];                                   /* +0x405 */
  volatile u8 IBE;                                                  /* +0x408 */
  const volatile u8 padding_3[3];                                   /* +0x409 */
  volatile u8 IEV;                                                  /* +0x40C */
  const volatile u8 padding_4[3];                                   /* +0x40D */
  volatile u8 IE;                                                   /* +0x410 */
  const volatile u8 padding_5[3];                                   /* +0x411 */
  const volatile u8 RIS;                                            /* +0x414 */
  const volatile u8 padding_6[3];                                   /* +0x415 */
  const volatile u8 MIS;                                            /* +0x418 */
  const volatile u8 padding_7[3];                                   /* +0x419 */
  volatile u8 IC;                                                   /* +0x41C */
  const volatile u8 padding_8[3];                                   /* +0x41D */
  volatile u8 AFSEL;                                                /* +0x420 */
  const volatile u8 padding_9[3007];                                /* +0x424 */
  const volatile u8 PeriphID0;                                      /* +0xFE0 */
  const volatile u8 padding_10[3];                                  /* +0xFE1 */
  const volatile u8 PeriphID1;                                      /* +0xFE4 */
  const volatile u8 padding_11[3];                                  /* +0xFE5 */
  const volatile u8 PeriphID2;                                      /* +0xFE8 */
  const volatile u8 padding_12[3];                                  /* +0xFE9 */
  const volatile u8 PeriphID3;                                      /* +0xFEC */
  const volatile u8 padding_13[3];                                  /* +0xFED */
  const volatile u8 CellID0;                                        /* +0xFF0 */
  const volatile u8 padding_14[3];                                  /* +0xFF1 */
  const volatile u8 CellID1;                                        /* +0xFF4 */
  const volatile u8 padding_15[3];                                  /* +0xFF5 */
  const volatile u8 CellID2;                                        /* +0xFF8 */
  const volatile u8 padding_16[3];                                  /* +0xFF9 */
  const volatile u8 CellID3;                                        /* +0xFFC */
  const volatile u8 padding_17[3];                                  /* +0xFFD */
};

#endif
