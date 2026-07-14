// create stack memory, assign value, try using in outer scope
#include "../src/mstrct.h"

int main(void) {
  M(int,, foo,); // declare simple int[][1]

  {
    M(auto, foo, 12);                                             // allocate foo[12][]: net on-stack memory is foo[12][1]
    m(foo,8) = 10;                                                // define foo[8][0], with checks

    printf("foo[8], with checks: %d\n", m(foo,8));                // fetch (with checks)

    M(int, const, bar, (2,6)) = {};                               // declare multidm type int[][2][6] with default 0 values
    M(auto, bar, 2);                                              // allocate int[2][2][6] on-stack
    m(bar,(0,1,2)) = 537;
    
    printf("bar[0][1][2], with checks: %d\n", m(bar,(0,1,2)));    // fetch (with checks)
  }

  printf("foo[8], with checks: %d\n", m(foo,8));                  // fetch (with checks): generates UAF report!

  return 0;
}
/* out
foo[8], with checks: 10
bar[0][1][2], with checks: 537
M_BAD/3_UAF.c/5
foo[8], with checks: 0 [garbage, in DEFAULT MODE]
*/


