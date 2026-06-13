// demonstrate (>O1) how aliasing a memstruct memory
// and using that to modify a memory is illegal!
// helps user to avoid writing non-idiomatically
// warnings are not limited to illustrated types,
// but any punned, immediate, dereferences.

#include <stdio.h>
#include "../src/mstrct.h"

int main(void) {
  M(int * ,var,); // var[][1]
  M(malloc(48),var,12); // var[12][1]
  //m(var,5) = 10; // define var[5][0]

  // uncomment below: compile-time err as raw memory is modified
  // (&m(var,0))[5] = 9; printf("printf to make use of modified memory: %d\n", m(var,5));

  // uncomment below: compile-time err as raw memory is modified
  // *(&m(var,2) + 5) = 9; printf("printf to make use of modified memory: %d\n", m(var,5));

  // below: not immediate but ideally should have warning (raw memory is modified)
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
