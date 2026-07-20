// create stack memory, assign value, try using in outer scope
#include "../src/mstrct.h"

int main(void) {
  m(baz, (2,6), int); // declare simple int[][1]

  {
    m(foo, 12, int, auto);                                        // allocate foo[12][]: net on-stack memory is foo[12][1]
    m(foo,8) = 10;                                                // define foo[8][0], with checks

    printf("foo[8], with checks: %d\n", m(foo,8));                // fetch (with checks)
    m(baz, auto) = m(foo, auto);
    m(baz,(0,1,2)) = 537;
    
    printf("bar[0][1][2], with checks: %d\n", m(baz,(0,1,2)));    // fetch (with checks)
  }

  printf("foo[8], with checks: %d\n", m(baz,8));                  // fetch (with checks): generates UAF report!

  return 0;
}
/* out
foo[8], with checks: 10
bar[0][1][2], with checks: 537
M_BAD/3_UAF.c/5
foo[8], with checks: 0 [garbage, in DEFAULT MODE]
*/


