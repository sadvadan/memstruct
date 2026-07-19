// reassign memory to an already assigned memstruct

#include "../src/mstrct.h"

int main(void) {
  m(var, 1, int, do);                                   // int[][1]
  M(var, malloc, 48);                          // int[12][1]
  
  m(foo, 12, int, do) = {};                  // declare foo[][12], initialize id & i as 0.

  m(foo,auto) = m(var,auto);                       // share memory: foo -> foo[1][12]

  M(var, malloc, 100);                                // remap var -> var[16][1]

  m(var,10) = 10;                                  // define var[1][10] as 10

  printf("var[10], with checks: %d\n", m(var,10)); // fetch (with checks)
  M(foo, free);
  return 0;
}

/*out
var[5], w/o checks: 10
*/
