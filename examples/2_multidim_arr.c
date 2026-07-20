// map multidim memstruct onto heap

#include "../src/mstrct.h"

int main(void) {
  m(foo, (2,3,2), int); // declare type int[][2][3][2]
  M(foo, malloc, 480); // int[][2][3][2] + int[10][][][] -> int[10][2][3][2] on heap
  
  m(foo,(7,1,2,1)) = 5; // define foo[7][1][2], with checks

  printf("size of this memstruct should be 8. check: %zu\n", sizeof(foo));
  printf("foo[7][1][2][1], with checks: %d\n", m(foo,(7,1,2,1))); // fetch (with checks)
  M(foo, free);
  return 0;
}

/* out
 * size of this memstruct should be 8. check: 8
 * foo[7][1][2][1], with checks: 5
*/


