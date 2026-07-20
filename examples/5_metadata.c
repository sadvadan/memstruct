// create heap memory, assign value, access metadata, free

#include "../src/mstrct.h"

int main(void) {
  m(var, 1, int);                      // var[][1]
  M(var, malloc, 48);                  // var[12][1]
  m(var,0) = 10;                       // define var[0][0]

  printf("ptr_ID: %d\n",               m(var,auto));
  printf("ptr_base_addr: %p\n",        &m(var));
  printf("arr_index_span: %lu\n",      m(var,_));
  printf("value of 1st element: %d\n", m(var));

  M(var, free);
  return 0;
}

/*out (typical)
ptr_ID: 2
ptr_base_addr: 0x564754dad010
arr_index_span: 12
value of 1st element: 10
*/
