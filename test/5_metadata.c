// create heap memory, assign value, access metadata, free

#include <stdio.h>
#include "../src/mstrct.h"

int main(void) {
  M(int * ,var,); // var[][1]
  M(malloc(48),var,12); // var[12][1]
  m(var,5) = 10; // define var[5][0]

  printf("ptr_addr: %p\n", m(var)); // fetch addr
  printf("arr_byte_size: %zu\n", M(var.id)->size); // fetch memory size in bytes
  printf("base_addr: %p\n", M(var.id)->addr); // fetch memory base addr

  free(var);
  return 0;
}

/*out (typical)
ptr_addr: 0x555693274010
arr_range: 48
base_addr: 0x555693274010
*/
