// heap memory, map multidim name

#include <stdio.h>
#include "../src/mstrct.h"

int main(void) {
  M(int *const,foo,,2,3,2); // declare type foo[][2][3]
  M(malloc(480),foo,10); // map foo -> foo[10][2][3] on heap
  
  m(foo,7,1,2,1) = 5; // define foo[7][1][2], with checks

  printf("foo[7][1][2][1], with checks: %d\n", m(foo,7,1,2,1)); // fetch (with checks)
  M(free, foo);
  return 0;
}

/* out
 * foo[7][1][2][1], with checks: 5
*/


