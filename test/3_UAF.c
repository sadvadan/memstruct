// create stack memory, assign value, try using in outer scope
#include <stdio.h>
#include "../src/mstrct.h"

int main(void) {
  M(int *,foo,); // declare type foo[][1]

  {
    M(auto,foo,12);                                         // map foo -> foo[12][]: the net on-stack memory is foo[12][1]

    m(foo,8) = 10;                                          // define foo[8][0], with checks

    printf("foo[8], with checks: %d\n", m(foo,8));          // fetch (with checks)

    M(int *const,bar,,2,6) = {0};                           // declare multidm type bar[][2][6] with default id,i = 0 values

    bar.id = foo.id;                                        // share memory with foo
    
    printf("bar[0][1][2], with checks: %d\n", m(bar,0,1,2));// fetch (with checks)
  }

  printf("foo[8], with checks: %d\n", m(foo,8));            // fetch (with checks): generates UAF report!

  return 0;
}
/* out
foo[8], with checks: 10
bar[0][1][2], with checks: 10
M_BAD/3_UAF.c/6
foo[8], with checks: 0 (default: "safe" garbage from reserved area)
*/


