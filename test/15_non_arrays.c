// create non-array type memstruct; use case: when the address needs to be taken safely
// here, m(base foo) is the safe address of the singlet
// note: in example-2, the singlet memstruct has size 4B only (preferred method for singlets)

#include <stdio.h>
#include "../src/mstrct.h"

int main(void) {
  /* example-1: a single int */
  M(int *const, foo,);                    // declare singlet int[][1]
  M(malloc(4), foo, 1);                   // map foo -> int[1][1] on-heap
  m(foo) = 10;                            // define int[0][0]
  printf("size of a non-array memstruct foo should be 8. check: %zu\n", sizeof(foo));
  printf("value of foo = %d, addr of foo: %p\n\n", m(foo), m(base foo));  // fetch memory
  M(free, foo);                           // comment this to see leak warning

  /* example-2: a single struct */
  struct sample {int a; void *b;};
  M(struct sample *, bar,, 0);            // declare singlet struct[][0] with size 4B only
  M(static, bar, 1);                      // allocate the struct on static storage
  m(bar).a = 5; m(bar).b = (void *)111;   // define bar fields
  printf("size of a non-array memstruct bar should be 4. check: %zu\n", sizeof(bar));
  printf("value of bar = {.a = %d, .b = %p} , addr of bar = %p\n", m(bar).a, m(bar).b, m(base bar));  // fetch memory
 
  return 0;
} /* out
size of a non-array memstruct foo should be 8. check: 8
value of foo = 10, addr of foo: 0x55d140c80010

size of a non-array memstruct bar should be 4. check: 4
value of bar = {.a = 5, .b = 0x6f} , addr of bar = 0x55d12b9be070
*/
