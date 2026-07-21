// create memstructs, allocate and then re-allocate

#define _GNU_SOURCE
#include <sys/mman.h>
#include "../src/mstrct.h"

int main(void) {
  m(foo, 1, int); // declare int[][1]
  m(bar, 1, int); // declare int[][1]

  // note: sanity checks (if ptr == NULL etc) are implicit
  M(foo, malloc, 32); // int[8][1]
  M(bar, mmap, NULL, 48, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0); // int[12][1]

  m(foo,5) = 10; // define int[5][0]
  m(bar,9) = 10; // define int[9][0]

  printf("foo[5], before realloc: %d\n", m(foo,5));   // fetch memory (before realloc)
  printf("bar[9], before realloc: %d\n\n", m(bar,9)); // fetch memory (before realloc)

  // re-allocation; note: sanity checks are implicit
  M(foo, realloc, &m(foo), 60); // int[15][1]
  M(bar, mremap, &m(bar), 48, 44, MREMAP_MAYMOVE); // int[11][1]

  printf("foo[5], after realloc: %d\n", m(foo,5)); // fetch memory (after realloc)
  printf("bar[9], after realloc: %d\n", m(bar,9)); // fetch memory (after realloc)

  // free the memory; NOTE: sanity check already included
  M(foo, free);   // comment this to see leak warning
  M(bar, munmap); // comment this to see leak warning
  return 0;
}
/*out
foo[5], before realloc: 10
bar[9], before realloc: 10

foo[5], after realloc: 10
bar[9], after realloc: 10
*/
