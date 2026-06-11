// check how aliasing a memstruct memory & dereferencing afterwards is illegal

#include <stdio.h>
#include "../src/mstrct.h"

int main(void) {
  M(int * ,var,); // var[][1]
  M(malloc(48),var,12); // var[12][1]
  m(var,5) = 10; // define var[5][0]

  printf("ptr_base_addr: %p\n", m(base var)); // reset index and fetch base addr
  printf("aliased dereference: %d\n", *(int *)m(base var)); // fetch 1st int thru aliased ptr

  M(free, var);
  return 0;
}

/*out (compile time warning)
warning:
array subscript ‘int[0]’ is partly outside array bounds of ‘unsigned char[1]’ [-Warray-bounds=]
12 |   printf("aliased dereference: %d\n", *(int *)m(base var)); // fetch 1st int thru aliased ptr
   |   ^~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/
