// create stack memory, assign value, try using in outer scope
#include <stdio.h>
#include "../src/mstrct.h"

int main(void) {
  M(int *,foo,); // declare type foo[][1]

  {
    M(auto,foo,12); // map foo -> foo[12][]: the net on-stack memory is foo[12][1]

    m(foo,8) = 10; // define foo[8][0], with checks

    printf("foo[8], w/o checks: %d\n", m(foo)[8]); // fetch (no checks)
    printf("foo[8], with checks: %d\n", m(foo,8)); // fetch (with checks)

    M(int *const,bar,,2,6); // declare multidm type bar[][2][6]
    M(foo.id,bar); // share memory with foo
    
    printf("bar[0][1][2], w/o checks: %d\n", m(bar)[8]); // fetch (no checks)
    printf("bar[0][1][2], with checks: %d\n", m(bar,0,1,2)); // fetch (with checks)
  }
  printf("foo[8], w/o checks: %d\n", m(foo)[8]); // fetch (no checks): this will segfault!
  printf("foo[8], with checks: %d\n", m(foo,8)); // fetch (with checks): generates UAF report!

  return 0;
}
/* out
foo[8], w/o checks: 10
foo[8], with checks: 10
bar[0][1][2], w/o checks: 10
bar[0][1][2], with checks: 10
MSTRCT ERR: USE_AFTER_FREE; originated at line: 23, file: 3_UAF.c; err status: 1711
foo[5], with checks: (garbage at foo[0] with settings MSTRCT_L = 0)
*/


