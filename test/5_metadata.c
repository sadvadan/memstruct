// create heap memory, assign value, access metadata, free

#include <stdio.h>
#include "../src/mstrct.h"

int main(void) {
  M(int * ,var,); // var[][1]
  M(malloc(48),var,12); // var[12][1]
  m(var,5) = 10; // define var[5][0]

  printf("ptr_base_addr: %p\n", m(base var)); // reset index and fetch base addr
  printf("arr_byte_size: %llu\n", m(size var)); // fetch memory size in bytes
  printf("arr_index_size: %llu\n", m(span var)); // fetch memory size in bytes

  M(free, var);
  return 0;
}

/*out (typical)
ptr_base_addr: 0x564754dad010
arr_byte_size: 48
arr_index_size: 12
*/
