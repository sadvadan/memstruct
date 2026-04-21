// create stack memory, assign value, try using in outer scope
#include <stdio.h>
#include "../src/mstrct.h"

int main(void) {
  $(int *, foo,); // declare safe ptr

  {
    $(int * const, var, 12, (int [12]){0}); // declare safe stack memory
    $(var, 5) = 10; // define stack memory, with checks
    printf("var[5], w/o checks: %d\n", $(var).addr[5]); // fetch (no checks)
    printf("var[5], with checks: %d\n", $(var,5)); // fetch (with checks)
    foo.id = var.id;
    printf("foo[5], w/o checks: %d\n", $(foo).addr[5]); // fetch (no checks)
    printf("foo[5], with checks: %d\n", $(foo,5)); // fetch (with checks)
    printf("foo[5], with checks: %d\n", $(foo,5)); // fetch (with checks)
  }
  // printf("foo[5], w/o checks: %d\n", $(foo).addr[5]); // fetch (no checks): this will segfault!
  printf("foo[5], with checks: %d\n", $(foo,5)); // fetch (with checks): generates UAF report!

  return 0;
}
/* out
var[5], w/o checks: 10
var[5], with checks: 10
foo[5], w/o checks: 10
foo[5], with checks: 10
MSTRCT ERR: USE_AFTER_FREE; originated at line: 9, file: sample_1.c; err status: 1711
foo[5], with checks: (garbage with settings MSTRCT_L = 0)
*/


