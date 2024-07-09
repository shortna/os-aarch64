#ifndef _DRIVERS_H
#define _DRIVERS_H

#include "types.h"

enum DRIVER_TYPE {
  DT_UART,
  DT_RTC,
  DT_TIMER,
  DT_NONE,
};

void driver_list_init(void);
void driver_add(enum DRIVER_TYPE t, void *d, u32 int_id);
enum DRIVER_TYPE driver_get(void **d, u32 int_id);

#endif

