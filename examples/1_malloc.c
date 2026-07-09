// create memstruct, map onto heap memory, free

//#define NMSTRCT       // uncomment to disable spatial safety
//#define NMSTRCTH      // uncomment to disable heap temporal safety
#include "../src/mstrct.h"

int main(void) {
  M(int, const, foo,);  // declare safe type int[][1]

  M(malloc(48),foo,12); // int[][1] + int[12][] -> int[12][1], on-heap

  m(foo,5) = 10;        // define int[5][0]

  printf("size of the memstruct should be 4. check: %zu\n", sizeof(foo));
  printf("var[5], with checks: %d\n", m(foo,5)); // fetch memory
  M(free, foo);         // comment this to see leak warning
  return 0;
}

/* out
size of the memstruct should be 4. check: 4
var[5], with checks: 10;
*/
