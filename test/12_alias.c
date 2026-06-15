// at >O1 memstruct warns at compile time if
// memstruct syntax is used non-idiomatically,
// or if puns and aliased dereferences to
// memstruct held memories are made.

#include <stdio.h>
#include "../src/mstrct.h"

int main(void) {
  M(int * ,var,);
  M(malloc(48),var,12);
  m(var,5) = 10;

  // uncomment below: compile-time err!
  // (&m(var,0))[5] = 9; printf("printf to make use of modified memory: %d\n", m(var,5));

  // uncomment below: compile-time err!
  // *(&m(var,2) + 5) = 9; printf("printf to make use of modified memory: %d\n", m(var,5));

  // uncomment below: compile-time err!
  int *tem = (&m(var,2) + 5); *tem = 13; printf("printf to make use of modified memory: %d\n", *tem);

  M(free, var);
  printf("test_12 complete.\n");
  return 0;
}

/*out (typical compile-time warning at >O1):
warning: array subscript [5, 7] is outside array bounds of ‘char[4]’ [-Warray-bounds=]
      |   int *tem = (&m(var,2) + 5); *tem = 13; printf("printf to make use of modified memory: %d\n", *tem);
      |                               ^~~~
*/
