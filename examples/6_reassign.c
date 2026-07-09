// reassign memory to an already assigned memstruct

#include "../src/mstrct.h"

int main(void) {
  M(int,, var,);                                   // int[][1]
  M(malloc(48), var, 12);                          // int[12][1]
  
  M(int, volatile, foo, 12) = {};                  // declare foo[][12], initialize id & i as 0.

  m(foo,enum) = m(var,enum);                       // share memory: foo -> foo[1][12]

  M(auto, var, 16);                                // remap var -> var[16][1]

  m(var,10) = 10;                                  // define var[1][10] as 10

  printf("var[10], with checks: %d\n", m(var,10)); // fetch (with checks)
  M(free, foo);
  return 0;
}

/*out
var[5], w/o checks: 10
*/
