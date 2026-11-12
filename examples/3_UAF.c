// create stack memory, assign value, try using in outer scope
#include "../src/mstrct.h"

int main(void) {
  m(baz, (2,6), int);                                             // declare int[][2][6]
  m(tor, 1, int);                                                 // declare int[][1]

  {
    m(foo, 12, int, auto);                                        // declare & allocate foo[12][1] on-stack
    m(foo,8) = 10;                                                // define foo[8][0]

    m(bar, 12, int, auto);                                        // declare & allocate bar[12][1] on-stack
    m(bar,8) = 12;                                                // define bar[8][0]


    m(baz, auto) = m(foo, auto);                                  // share memory of foo with baz
    m(baz,(0,1,2)) = 537;
    printf("baz[0][1][2], inner block: %d\n", m(baz,(0,1,2)));    // fetch
    
    m(tor, auto) = m(bar, auto);                                  // share memory of bar with tor
    printf("tor[8], inner block: %d\n", m(bar,8));                // fetch
  }

  printf("baz[0][1][2], outer block: %d\n", m(baz,(0,1,2)));      // fetch : generates UAF report!
  printf("tor[8], outer block: %d\n", m(tor,8));                  // fetch : generates UAF report!

  return 0;
}
/* out
baz[0][1][2], inner block: 537
tor[8], inner block: 12
M_BAD/3_UAF.c/5
baz[0][1][2], outer block: 0  [garbage, in DEFAULT MODE]
M_BAD/3_UAF.c/6
tor[8], outer block: 0        [garbage, in DEFAULT MODE]
*/


