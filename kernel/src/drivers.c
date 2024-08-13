#include "drivers.h"

// fit in one block
#define N_DRIVERS (32)

struct Driver {
  u32 int_id;
  enum DEVICE_TYPE type;
  void *d;
};

__attribute__((section(".data"))) static struct Driver drivers[N_DRIVERS];
__attribute__((section(".data"))) static u8 drivers_count = 0;

void driver_add(enum DEVICE_TYPE t, void *d, u32 int_id) {
  drivers[drivers_count].type = t;
  drivers[drivers_count].int_id = int_id;
  drivers[drivers_count].d = d;
  drivers_count++;
}

enum DEVICE_TYPE driver_get(void **d, u32 int_id) {
  for (u8 i = 0; i < drivers_count; i++) {
    if (drivers[i].int_id == int_id) {
      *d = drivers[i].d;
      return drivers[i].type;
    }
  }

  return DT_NONE;
}
