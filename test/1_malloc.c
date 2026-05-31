// create safe ptr, map onto heap memory, free

#include <stdio.h>
#include "../src/mstrct.h"

int main(void) {
  M(int *const,foo,); // declare safe type int[][1]

  M(malloc(48),foo,12); // map foo -> int[12][1] on-heap

  m(foo,5) = 10; // define int[5][0]

  printf("size of a memstruct should be 8. check: %zu\n", sizeof(foo));
  printf("var[5], with checks: %d\n", m(foo,5)); // fetch memory (with checks)
  free(foo); // comment this to see leak warning
  return 0;
}

/* out
size of safe ptr should be 8. check: 8
var[5], with checks: 10;
*/

/* try this const array variant too:
  int main(void) {
  M(int *const,foo,,12); // declare safe type int[][12]

  M(malloc(48),foo,1); // map foo -> int[1][12] on-heap

  m(foo,,5) = 10; // define int[5][0]

  printf("size of safe ptr should be 8. check: %zu\n", sizeof(foo));
  printf("var[5], with checks: %d\n", m(foo,,5)); // fetch memory (with checks)
  free(foo); // comment this to see leak warning
  return 0;
}
*/
