#include "drivers.h"

// fit in one block
#define N_DRIVERS (32)

struct Driver {
  uint32_t int_id;
  enum DEVICE_TYPE type;
  void *d;
};

static struct Driver drivers[N_DRIVERS];
static uint8_t drivers_count = 0;

void driver_add(enum DEVICE_TYPE t, void *d, uint32_t int_id) {
  drivers[drivers_count].type = t;
  drivers[drivers_count].int_id = int_id;
  drivers[drivers_count].d = d;
  drivers_count++;
}

enum DEVICE_TYPE driver_get(void **d, uint32_t int_id) {
  for (uint8_t i = 0; i < drivers_count; i++) {
    if (drivers[i].int_id == int_id) {
      *d = drivers[i].d;
      return drivers[i].type;
    }
  }

  return DT_NONE;
}
