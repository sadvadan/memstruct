// demonstrate how aliasing a memstruct memory through a
// raw ptr and then modifying an OOB memory is illegal!

#include <stdio.h>
#include "../src/mstrct.h"

int main(void) {
  M(int * ,var,); // var[][1]
  M(malloc(48),var,12); // var[12][1]
  //m(var,5) = 10; // define var[5][0]

  // uncomment below: compile-time err as OOB memory is modified
  // (&m(var,0))[5] = 9; printf("printf to make use of modified memory: %d\n", m(var,5));

  // uncomment below: compile-time err as OOB memory is modified
  // *(&m(var,2) + 5) = 9; printf("printf to make use of modified memory: %d\n", m(var,5));

  // below: no comptime err as OOB memory isn't modified (only read)
  // int ser = (&m(var,0))[5]; printf("printf to make use of read memory: %d\n", ser);

  // below: should have warning but doesn't
  // int *tem = (&m(var,2) + 5); *tem = 13; printf("printf to make use of modified memory: %d\n", *tem);

  M(free, var);
  printf("test_12 complete.\n");
  return 0;
}

/*out (typical compile-time warning at >O1):
warning: writing 4 bytes into a region of size 0 [-Wstringop-overflow=]
      |   *(&m(var,0) + 5) = 9; printf("printf to make use of modified memory: %d", m(var,5));
      |   ~~~~~~~~~~~~~~~~~^~~
*/
