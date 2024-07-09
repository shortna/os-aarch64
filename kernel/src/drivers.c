#include "drivers.h"
#include "mem.h"

// fit in one block
#define N_DRIVERS (32)

struct Driver {
  u32 int_id;
  enum DRIVER_TYPE type;
  void *d;
};

static struct Driver *drivers;
static u8 drivers_count;

void driver_list_init() { drivers = block_alloc(); }

void driver_add(enum DRIVER_TYPE t, void *d, u32 int_id) {
  drivers[drivers_count].type = t;
  drivers[drivers_count].int_id = int_id;
  drivers[drivers_count].d = d;
  drivers_count++;
}

enum DRIVER_TYPE driver_get(void **d, u32 int_id) {
  for (u8 i = 0; i < drivers_count; i++) {
    if (drivers[i].int_id == int_id) {
      *d = drivers[i].d;
      return drivers[i].type;
    }
  }

  return DT_NONE;
}
