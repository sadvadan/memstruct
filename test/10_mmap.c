// create memstruct, mmap onto heap memory, munmap

#include <stdio.h>
#include <sys/mman.h>
#include "../src/mstrct.h"

int main(void) {
  M(int *const,foo,); // declare safe type foo[][1]

  // map foo -> foo[12][1] on-heap; NOTE: MAP_FAILED sanity checked already performed internally 
  M(mmap(NULL, 48, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0) , foo, 12);

  m(foo,5) = 10; // define foo[5][0]

  printf("size of a memstruct should be 4. check: %zu\n", sizeof(foo));
  printf("var[5], with checks: %d\n", m(foo,5)); // fetch memory (with checks)
  printf("var[5], w/o checks: %d\n", m(foo)[5]); // fetch memory (no checks)

  // free the memory; NOTE: sanity check already included
  munmap(foo); // comment this to see leak warning
  return 0;
}

