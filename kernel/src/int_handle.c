#include "types.h"

extern u32 get_int_id(void);

void handle(void) {
  u32 id = get_int_id();
  while(1);
}

