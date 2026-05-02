// create heap memory, assign value, do ptr arithmetic, free

#include <stdio.h>
#include "../src/mstrct.h"

int main(void) {
  M(int * ,var,); // var[][1]
  M(malloc(48),var,12); // var[12][1]
  m(var,5) = 10; // define var[5][0]

  printf("addr: %p\n", m(var)); // fetch addr
  printf("var[5], before ptr arith: %d\n", m(var,5)); // fetch memory
  m(var)++; // ptr arithmetic

  printf("var[4], after ptr arith: %d\n", m(var,4)); // fetch memory
  free(var);
  return 0;
}

/*out (typical)
addr: 0x558c75e21010
var[5], before ptr arith: 10
var[4], after ptr arith: 10
*/
