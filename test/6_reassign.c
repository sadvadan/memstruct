// reassign memory to an already allocated variable

#include <stdio.h>
#include "../src/mstrct.h"

int main(void) {
  M(int * ,var,); // var[][1]
  M(malloc(48),var,12); // var[12][1]
  
  M(int * volatile, foo,,12); // foo[][12]

  M(var.id,foo); // share memory: foo -> foo[1][12]

  M(auto,var,16); // remap var -> var[16][1]

  m(var,10) = 10; // define var[1][10] as 10

  printf("var[10], w/o checks: %d\n", m(var)[10]); // fetch (no checks)
  printf("var[10], with checks: %d\n", m(var,10)); // fetch (with checks)
  free(foo);
  return 0;
}

/*out
var[5], w/o checks: 10
var[5], with checks: 10
*/
