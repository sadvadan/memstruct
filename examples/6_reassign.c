// reassign memory to an already assigned memstruct

#include "../src/mstrct.h"

int main(void) {
  m(var, 1, int);                                     // declare var[][1]
  M(var, malloc, 48);                                 // allocate var[12][1]
  
  m(foo, 12, int);                                    // declare foo[][12]

  m(foo,auto) = m(var,auto);                          // share var's memory with foo

  M(var, malloc, 100);                                // reassign var -> var[16][1]

  m(var,10) = 10;                                     // define var[10] as 10

  printf("var[10]: %d\n", m(var,10));                 // fetch
  M(foo, free);
  M(var, free);
  return 0;
}

/*out
var[10]: 10
*/
