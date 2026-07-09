// create safe ptr, map onto stack with initializer list

#include "../src/mstrct.h"

int main(void) {
  M(int, const, foo, ); // declare fixed size type int[][1]

  M(auto, foo, (1,2,3,4,10)); // foo -> int[5][1] on-stack, with initializer list

  m(foo,4) = 5; // define int[0][4]
  printf("byte size of foo: %llu\n", m(foo,sizeof));

  // note: fetches are completely compile time checked (fixed type memstruct), even with checks disabled
  printf("var[0], with checks: %d\n", m(foo,0)); // fetch memory (with checks)
  printf("var[1], with checks: %d\n", m(foo,1)); // fetch memory (with checks)
  printf("var[2], with checks: %d\n", m(foo,2)); // fetch memory (with checks)
  printf("var[3], with checks: %d\n", m(foo,3)); // fetch memory (with checks)
  printf("var[4], with checks: %d\n", m(foo,4)); // fetch memory (with checks)

  return 0;
}
/* out
byte size of foo: 20

var[0], with checks: 1
var[1], with checks: 2
var[2], with checks: 3
var[3], with checks: 4
var[4], with checks: 5
*/
