// verify err handling behavior based on hardening flag
// one by one uncomment MSTRCT_SOFT and MSTRCT_HARD

#include <stdio.h>
// #define MSTRCT_SOFT
// #define MSTRCT_HARD
#include "../src/mstrct.h"

int main(void) {
  M(int *,arr,); // declare arr as memstruct with no static index
  M(malloc(40),arr,10);
  m(arr,0) = 47; // assign arr[0]

  // === Level 0: continue after error (default behavior) ===
  #if !defined(MSTRCT_SOFT) && !defined(MSTRCT_HARD)
    printf("default mode: OOB prints line no of memstruct genesis.\n");
    int x = m(arr,15);    // OOB
    printf("Continued after OOB: x = %d (expected : base value)\n", x);

    // Use-after-free simulation
    M(free, arr);
    x = m(arr,0);         // UAF -> print error, continue
    printf("default behavior: Continued after UAF\n");
  #endif

    // === Level 1: Strict (segfault/halt at error site) ===
  #if defined(MSTRCT_SOFT)
    printf("soft mode - OOB prints line no of err site.\n");
    int x = m(arr,15);    // OOB
    M(free, arr);
    printf("continued after OOB. x = %d\n", x);
  #endif

    // === Level 2: Hard (exit with status code) ===
  #if defined(MSTRCT_HARD)
    printf("hard mode - segfault at OOB site; line no = 0.\n");
    int x = m(arr,15);    // OOB
    printf("This line should NOT be reached in HARD mode: x = %d\n", x);
    M(free, arr); // this is not reached
  #endif

  return 0;
}
/*out (one by one case)

default mode: OOB prints line no of memstruct genesis.
M_OOB/7_hardening.c/10
Continued after OOB: x = 47 (expected : base value)
M_BAD/7_hardening.c/10
default behavior: Continued after UAF

soft mode - OOB prints line no of err site.
M_OOB/7_hardening.c/29
continued after OOB. x = 47

hard mode - segfault at OOB site; line no of memstruct genesis.
M_BAD/7_hardening.c/0
Segmentation fault         (core dumped) ./out

*/
