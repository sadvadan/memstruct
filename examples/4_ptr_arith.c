// create heap memory, assign value, do ptr arithmetic, free
// note: to force index feature on a memstruct create the same
// using under-parentheses index like (index):

#include "../src/mstrct.h"

int main(void) {
  m(var, (1), int);                                   // int[][1], with index featured
  M(var, malloc, 48);                                 // int[][1] + int[12][] -> int[12][1], on-heap
  m(var,5) = 10;                                      // define var[5][0]

  printf("size of this memstruct should be 8. check: %zu\n", sizeof(var));
  printf("var[5], before ptr arith: %d\n", m(var,5)); // fetch memory
  var.i++;                                            // ptr arithmetic

  printf("var[4], after ptr arith: %d\n", m(var,4));  // fetch memory
  M(var, free);
  return 0;
}

/*out
size of this memstruct should be 8. check: 8
var[5], before ptr arith: 10
var[4], after ptr arith: 10
*/
