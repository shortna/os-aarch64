#ifndef _DRIVERS_H
#define _DRIVERS_H

#include "types.h"

enum DEVICE_TYPE {
  DT_UART,
  DT_RTC,
  DT_TIMER,
  DT_VIRTIO,
  DT_NONE,
};

bool driver_list_init(void);
void driver_add(enum DEVICE_TYPE t, void *d, u32 int_id);
enum DEVICE_TYPE driver_get(void **d, u32 int_id);

#endif

