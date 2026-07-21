// create different types of globals

#include "../src/mstrct.h"

m(baz, 100, float, static);
m(bin, 12, int, static __thread);
m(bar, (2,3), char, );

int main(void) {
  static m(foo, 32, int, auto);

  m(foo,4) = 5;
  m(bar,(1,2)) = 15;
  m(baz,92) = 'a';
  m(bin,10) = 4;

  printf("span of foo: %lu\n",   m(foo,_));
  printf("span of bar: %lu\n",   m(bar,_));
  printf("span of baz: %lu\n",   m(baz,_));
  printf("span of bin: %lu\n\n", m(bin,_));

  // note: fetches are completely compile time checked (fixed type memstruct), even with checks disabled
  printf("foo[4], with checks: %d\n",     m(foo,4)); // fetch memory (with checks)
  printf("bar[1][2], with checks: %d\n",  m(bar,(1,2))); // fetch memory (with checks)
  printf("baz[92], with checks: %f\n",    m(baz,92)); // fetch memory (with checks)
  printf("bin[10], with checks: %d\n",    m(bin,10)); // fetch memory (with checks)

  return 0;
}
/* out
span of foo: 32
span of bar: 6
span of baz: 100
span of bin: 12

foo[4], with checks: 5
bar[1][2], with checks: 15
baz[92], with checks: 97.000000
bin[10], with checks: 4
*/
