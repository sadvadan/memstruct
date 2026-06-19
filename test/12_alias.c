// at >O1 memstruct warns at compile time if memstruct syntax is used non-idiomatically,
// or if puns and aliased dereferences to memstruct held memories are made.
// note: 
// a) similar static level safety extends to m(base foo) also.
// b) when ptr arithmetic is purely run-time/ obfuscated, the static only check fails (case 4)

#include <stdio.h>
#include "../src/mstrct.h"

int main(void) {
  M(int * ,var,);
  M(malloc(48),var,12);
  m(var,5) = 10;

  // case 1: uncomment below: compile-time err!
  // (&m(var,0))[4] = 9; printf("printf to make use of modified memory: %d\n", m(var,5));

  // case 2: uncomment below: compile-time err!
  // *(&m(var,2) + 5) = 9; printf("printf to make use of modified memory: %d\n", m(var,5));

  // case 3: uncomment below: compile-time err!
  // int *tem = (&m(var,2) + 6); *tem = 13; printf("printf to make use of modified memory: %d\n", *tem);

  // case 4: uncomment below (2 lines): no warning as index a is purely runtime => our static-only analyzer fails!
  int a = 7; asm volatile (" " : "+r" (a) : : ); // deliberately make 'a' runtime
  int *tem = (&m(var,2) + a); *tem = 13; printf("printf to make use of modified memory: %d\n", *tem);

  M(free, var);
  printf("test_12 complete.\n");
  return 0;
}

/*out (typical compile-time warning at >O1; here, for case 3):
warning: array subscript [6, 8] is outside array bounds of ‘char[4]’ [-Warray-bounds=]
      |   int *tem = (&m(var,2) + 5); *tem = 13; printf("printf to make use of modified memory: %d\n", *tem);
      |                               ^~~~
*/
