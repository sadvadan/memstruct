// create memstruct, map onto heap memory, free

//#define NMSTRCT       // uncomment to disable memory safety
#include "../src/mstrct.h"

int main(void) {
  m(foo, 1, int);       // declare int[][1]

  M(foo, malloc, 48);   // allocate: int[][1] + int[12][] -> int[12][1], on-heap

  m(foo,5) = 10;        // define int[5][0]; make index e.g. 13 to see bounds error

  printf("size of the memstruct should be 4. check: %zu\n", sizeof(foo));
  printf("foo[5]: %d\n", m(foo,5)); // fetch foo[5]
  M(foo, free);         // comment this to see leak warning
  return 0;
}

/* out
size of the memstruct should be 4. check: 4
foo[5]: 10;
*/
