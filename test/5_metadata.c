// create heap memory, assign value, access metadata, free

#include <stdio.h>
#include "../src/mstrct.h"

int main(void) {
  M(int * ,var,); // var[][1]
  M(malloc(48),var,12); // var[12][1]
  m(var,5) = 10; // define var[5][0]

  printf("ptr_base_addr: %p\n", (var.i = 0, &m(var,0))); // reset index and fetch base addr
  printf("arr_byte_size: %llu\n", M(var)); // fetch memory size in bytes

  M(free, var);
  return 0;
}

/*out (typical)
ptr_addr: 0x555693274010
arr_range: 48
*/
