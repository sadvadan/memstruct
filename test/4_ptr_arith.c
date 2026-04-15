// create heap memory, assign value, free

#include <stdio.h>
#include "../src/mstrct.h"

int main(void) {
  $(int * , var, 12, malloc(48)); // declare heap memory
  $(var, 5) = 10; // define heap memory, with checks, make it 15 to see comptime err!
  printf("addr: %p\n", $(var)); // fetch memory (no checks)
  printf("addr: %p\n", &$(var,0)); // fetch memory (no checks)

  $(var)++;
  *$(var) = 20; // define w/o checks
  printf("var[5], w/o checks: %d\n", $(var)[5]); // fetch memory (no checks)
  printf("var[5], with checks: %d\n", $(var,5)); // fetch memory (with checks)
  printf("var[6], w/o checks: %d\n", $(var)[4]); // fetch memory (no checks)
  printf("var[6], with checks: %d\n", $(var,4)); // fetch memory (with checks)
  printf("addr: %p\n", $(var)); // fetch memory (no checks)
  printf("addr: %p\n", &$(var,0)); // fetch memory (no checks)
  free(var); // comment this to see leak warning
  return 0;
}

/*out (typical)
addr: 0x563fce6bc010
addr: 0x563fce6bc010
var[5], w/o checks: 0
var[5], with checks: 0
var[6], w/o checks: 10
var[6], with checks: 10
addr: 0x563fce6bc014
addr: 0x563fce6bc014
*/
