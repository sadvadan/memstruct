// create non-array type memstruct; use case: when the address needs to be taken safely

#include <stdio.h>
#include "../src/mstrct.h"

int main(void) {
  /* example-1: a single int */
  m(foo, 1, int);                   // declare simple int[][1]
  M(foo, malloc, 4);                   // map foo -> int[1][1] on-heap
  m(foo) = 10;                            // define int[0][0]
  printf("size of a non-array memstruct foo should be 4. check: %zu\n", sizeof(foo));
  printf("value of foo = %d, addr of foo: %p\n\n", m(foo), &m(foo));  // fetch memory
  M(foo, free);                           // comment this to see leak warning

  /* example-2: a single struct */
  struct sample {int a; int b;};
  static m(bar, 1, struct sample, auto);   
  m(bar).a = 5; m(bar).b = 111;   // define bar fields
  printf("size of a non-array memstruct bar should be 4 (memstruct) + 2*4 (struct) = 12. check: %zu\n", sizeof(bar));
  printf("value of bar = {.a = %d, .b = %d} , addr of bar = %p\n", m(bar).a, m(bar).b, &m(bar));  // fetch memory
 
  return 0;
} /* out
size of a non-array memstruct foo should be 4. check: 4
value of foo = 10, addr of foo: 0x55d140c80010

size of a non-array memstruct bar should be 4 (memstruct) + 2*4 (struct) = 12. check: 12
value of bar = {.a = 5, .b = 0x6f} , addr of bar = 0x55d12b9be070
*/
