// reassign memory to an already allocated variable

#include <stdio.h>
#include "../src/mstrct.h"

int main(void) {
  $(int * , var, 12, malloc(48)); // declare heap memory
  $(int * volatile, foo,); // define heap memory, with checks, make it 15 to see comptime err!

  foo.id = var.id;
  $(, var, 16, (int [16]){0}); // reassign
  $(var, 5) = 10; // define stack memory, with checks
  printf("var[5], w/o checks: %d\n", $(var)[5]); // fetch (no checks)
  printf("var[5], with checks: %d\n", $(var,5)); // fetch (with checks)
  printf("&var: %zu\n", $(var,)->addr); // fetch (with checks)
  free(foo); // comment this to see leak warning
  return 0;
}

/*out
var[5], w/o checks: 10
var[5], with checks: 10
&var: 140733110432368
*/
