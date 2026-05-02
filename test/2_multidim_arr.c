// heap memory, map multidim name

#include <stdio.h>
#include "../src/mstrct.h"

int main(void) {
  M(int *const,foo,,2,3); // declare type foo[][2][3]
  M(malloc(240),foo,10); // map foo -> foo[10][2][3] on heap
  
  m(foo,7,1,2) = 5; // define foo[7][1][2], with checks

  printf("foo[7][1][2], w/o checks: %d\n", m(foo)[47]);
  printf("foo[7][1][2], with checks: %d\n", m(foo,7,1,2)); // fetch (with checks)
  free(foo);
  return 0;
}

/* out
 * foo[7][1][2], w/o checks: 5
 * foo[7][1][2], with checks: 5
*/


