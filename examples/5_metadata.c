// create heap memory, assign value, access metadata, free

#include "../src/mstrct.h"

int main(void) {
  M(int,, var,);                       // var[][1]
  M(malloc(48), var, 12);              // var[12][1]
  m(var,0) = 10;                       // define var[0][0]

  printf("ptr_ID: %d\n",               m(var,enum));
  printf("ptr_base_addr: %p\n",        m(var,void));
  printf("arr_byte_size: %lu\n",      m(var,sizeof));
  printf("arr_index_span: %lu\n",     m(var,_));
  printf("value of 1st element: %d\n", m(var));

  M(free, var);
  return 0;
}

/*out (typical)
ptr_ID: 2
ptr_base_addr: 0x564754dad010
arr_byte_size: 48
arr_index_span: 12
value of 1st element: 10
*/
