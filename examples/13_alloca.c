// create safe ptr, map onto heap memory, free

#include "../src/mstrct.h"

int main(void) {
  m(foo, 1, int); // declare safe type int[][1]

#define NMSTRCTH  // suppress leak warning (alloca memories end at function scope automatically)
  M(foo, alloca, 48); // map foo -> int[12][1] on-heap
#undef NMSTRCTH

  m(foo,5) = 10; // define int[5][0]

  printf("var[5], with checks: %d\n", m(foo,5)); // fetch memory (with checks)
  return 0;
}

/* out
var[5], with checks: 10;
*/
