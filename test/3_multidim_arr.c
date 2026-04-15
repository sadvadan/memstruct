// create on stack memory, assign multidim name

#include <stdio.h>
#include "../src/mstrct.h"

int main(void) {

  $(int * const, var[2][3], 10, (int [60]){0}); // declare memory
  $(var[1][2], 5) = 10; // define memory, with checks
  printf("var[5], w/o checks: %d\n", $(var[1][2])[5]); // fetch (no checks)
  printf("var[5], with checks: %d\n", $(var[1][2],5)); // fetch (with checks)

  return 0;
}

/* out
* var[5], w/o checks: 10
* var[5], with checks: 10
*/


