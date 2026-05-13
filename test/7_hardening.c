// verify err handling behavior based on MSTRCT_L

#include <stdio.h>
//#define MSTRCT_STRICT /* case 1: keep commented | case 2: uncomment | case 3: #define MSTRCT_HARD */
#include "../src/mstrct.h"

// Test helper
#define TEST_CASE(desc) do { \
    printf("\n=== TEST: %s (MSTRCT_L=%d) ===\n", desc, MSTRCT_L); \
} while(0)

int main(void) {
  M(int *,arr,); // declare arr as memstruct with no static index
  M(malloc(40),arr,10);
  m(arr,0) = 47; // assign arr[0]

  // === Level 0: Continue after error (default behavior) ===
  #if MSTRCT_L == 0
    TEST_CASE("Bounds check fail - should print error and continue");
    int x = m(arr,15);    // OOB
    printf("Continued after OOB: x = %d (expected : base value)\n", x);

    // Use-after-free simulation
    free(arr);
    x = m(arr,0);         // UAF -> print error, continue
    printf("Continued after UAF\n");
  #endif

    // === Level 1: Strict (segfault/halt at error site) ===
  #if MSTRCT_L == 1
    TEST_CASE("Strict mode - expect halt/segfault on error");
    int x = m(arr,15);    // OOB
    printf("This line should NOT be reached in STRICT mode: x = %d\n", x);
    free(arr); // program killed already, so no memory leak error print
  #endif

    // === Level 2: Hard (exit with status code) ===
  #if MSTRCT_L == 2
    TEST_CASE("Hard mode - expect clean exit with status code");
    int x = m(arr,15);    // OOB
    printf("This line should NOT be reached in HARD mode: x = %d\n", x);
    free(arr); // this is not reached, so memory leak error print
  #endif

  return 0;
}

