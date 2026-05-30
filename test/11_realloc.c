// create memstructs, allocate and then re-allocate

#define _GNU_SOURCE
#include <stdio.h>
#include <sys/mman.h>
#include "../src/mstrct.h"

int main(void) {
  M(int *,foo,); // declare int[][1]
  M(int *,bar,); // declare int[][1]

  M(malloc(32), foo, 8); // int[8][1]
  M(mmap(NULL, 48, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0), bar, 12); // int[12][1]

  m(foo,5) = 10; // define int[5][0]
  m(bar,9) = 10; // define int[9][0]

  printf("foo[5], before realloc: %d\n", m(foo,5));   // fetch memory (before realloc)
  printf("bar[9], before realloc: %d\n\n", m(bar,9)); // fetch memory (before realloc)

  // re-allocation; note: M(foo) is implicitly cast to (void *) bas_addr
  M(realloc(M(foo)->addr, 60), foo, 15); // int[15][1]
  M(mremap(M(bar)->addr, 48, 44, MREMAP_MAYMOVE), bar, 12); // int[12][1]

  printf("foo[5], after realloc: %d\n", m(foo,5)); // fetch memory (after realloc)
  printf("bar[9], after realloc: %d\n", m(bar,9)); // fetch memory (after realloc)

  // free the memory; NOTE: sanity check already included
  free(foo);   // comment this to see leak warning
  munmap(bar); // comment this to see leak warning
  return 0;
}
/*out
foo[5], before realloc: 10
bar[9], before realloc: 10

foo[5], after realloc: 10
bar[9], after realloc: 10
*/
