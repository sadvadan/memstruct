// create heap memory, assign value, access metadata, free

#include <stdio.h>
#include "../src/mstrct.h"

int main(void) {
  $(int * , var, 12, malloc(48)); // declare heap memory
  $(var, 5) = 10; // define heap memory, with checks, make it 15 to see comptime err!

  printf("ptr_addr: %zu\n", $(var,)->addr); // fetch addr (uint64_t)
  printf("arr_range: %zu\n", $(var,)->size); // fetch range (uint64_t)
  printf("base_addr: %zu\n", $(var,)->base); // fetch base (uint64_t)

  free(var); // comment this to see leak warning
  return 0;
}

/*out (typical)
ptr_addr: 94235992948752
arr_range: 48
base_addr: 94235992948752
*/
