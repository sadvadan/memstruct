// create safe ptr, map onto heap memory, free

#include <stdio.h>
#include "../src/mstrct.h"

int main(void) {
  M(int *const,foo,); // declare safe type int[][1]

  M(alloca(48),foo,12); // map foo -> int[12][1] on-heap

  m(foo,5) = 10; // define int[5][0]

  printf("var[5], with checks: %d\n", m(foo,5)); // fetch memory (with checks)
  return 0;
}

/* out
var[5], with checks: 10;
*/
