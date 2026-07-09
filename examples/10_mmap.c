// create memstruct, mmap onto heap memory, munmap

#include <sys/mman.h>
#include "../src/mstrct.h"

int main(void) {
  M(int, const, foo,); // declare safe type int[][1]

  // map foo -> int[12][1] on-heap; note: MAP_FAILED sanity check included in M() 
  M(mmap(NULL, 48, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0), foo, 12);

  m(foo,5) = 10; // define int[5][0]

  printf("var[5], with checks: %d\n", m(foo,5)); // fetch memory (with checks)

  // free the memory; NOTE: sanity check already included
  //M(munmap, foo); // comment this to see leak warning
  return 0;
}
/*out
var[5], with checks: 10
*/
