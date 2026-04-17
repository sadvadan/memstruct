// create heap memory, assign value, free

#include <stdio.h>
#include "../src/mstrct.h"

int main(void) {
  $(int * const, var, 12, malloc(48)); // declare heap memory
  $(var, 5) = 10; // define heap memory, with checks, make it 15 to see comptime err!
  printf("size of safe ptr should be 4. check: %zu\n", sizeof(var));
  printf("var[5], w/o checks: %d\n", $(var).addr[5]); // fetch memory (no checks)
  printf("var[5], with checks: %d\n", $(var,5)); // fetch memory (with checks)
  free(var); // comment this to see leak warning
  return 0;
}

/* out
size of safe ptr should be 4. check: 4
var[5], w/o checks: 10
var[5], with checks: 10;
*/


