// create safe ptr, map onto heap memory, free

#include <stdio.h>
#include "../src/mstrct.h"

int main(void) {
  M(int *const,foo,); // declare safe type foo[][1]

  M(malloc(48),foo,12); // map foo -> foo[12][1] on-heap

  m(foo,5) = 10; // define foo[5][0]

  printf("size of safe ptr should be 2. check: %zu\n", sizeof(foo));
  printf("var[5], with checks: %d\n", m(foo,5)); // fetch memory (with checks)
  printf("var[5], w/o checks: %d\n", m(foo)[5]); // fetch memory (no checks)
  free(foo); // comment this to see leak warning
  return 0;
}

/* out
size of safe ptr should be 2. check: 2
var[5], w/o checks: 10
var[5], with checks: 10;
*/


