// custom arena allocator using memstruct safety guarantee

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "../src/mstrct.h"

// Define the Arena structure
typedef struct {
  M(uint8_t *, buffer,);               // memstruct as struct field
  size_t capacity;
  size_t offset;
} Arena;

// Initialize the arena with a fixed capacity
Arena arena_init(size_t capacity) {
  Arena arena = {0};
  M(malloc(capacity), arena.buffer, capacity);
  /* 
  memory sanity check is redundant, as the above line did it internally already
  if (m(arena.buffer) == NULL) {fprintf(stderr, "Failed to allocate arena memory.\n"); exit(1);}
  */
  arena.capacity = capacity;
  arena.offset = 0;
  return arena;
}

// Allocate memory from the arena
void *arena_alloc(Arena *arena, size_t size) {
  // Basic alignment (align to 8 bytes for safety on most architectures)
  size_t aligned_size = (size + 7) & ~7;

  // Check if we have enough room left
  if (arena->offset + aligned_size > arena->capacity) {
    fprintf(stderr, "Arena out of memory!\n");
    return NULL; 
  }

  // Get the pointer to the allocated chunk
  void *ptr = &m(arena->buffer, arena->offset);
  
  // Move the offset forward
  arena->offset += aligned_size;

  return ptr;
}

// Reset the arena (O(1) clearing!)
void arena_reset(Arena *arena) {
  arena->offset = 0;
}

// Free the entire arena back to the OS
void arena_free(Arena *arena) {
  free(arena->buffer);                 // free(memstruct) does NULLing of metadata internally
  arena->capacity = 0;
  arena->offset = 0;
}

// --- Example Usage ---
int main() {
  // 1. Create an arena of 1 KB
  Arena arena = arena_init(1024);

  // 2. Allocate an integer
  M(int *const, number,);
  #define NMSTRCT                      // skip temporal safety 
  M(arena_alloc(&arena, sizeof(int)), number, 1);
  #undef NMSTRCT
  m(number,0) = 42;

  // 3. Allocate an array of floats
  M(float *const, prices,);
  #define NMSTRCT                      // skip temporal safety 
  M(arena_alloc(&arena, 5 * sizeof(float)), prices, 5);
  #undef NMSTRCT
 
  for (int i = 0; i < m(prices,); i++) { // i_max = m(prices,) = 5
    m(prices,i) = i * 10.5f;
  }

  // 4. Allocate a string
  M(char *const, greeting,);
  #define NMSTRCT                      // skip temporal safety 
  M(arena_alloc(&arena, 13 * sizeof(char)), greeting, 13);
  #undef NMSTRCT

  strncpy(m(greeting), "Hello Arena!", M(greeting)->size);        // memory safe (unlike strcpy)!!

  // Print the values to verify
  printf("Integer: %d\n", m(number,0));
  printf("Float array: %.1f, %.1f\n", m(prices,0), m(prices,1));
  printf("String: %s\n", m(greeting));
  printf("Arena usage: %zu/%zu bytes\n", arena.offset, arena.capacity);

  // 5. Clean up everything at once
  arena_free(&arena);

  return 0;
}

/* out:
Integer: 42
Float array: 0.0, 10.5
String: Hello Arena!
Arena usage: 48/1024 bytes
*/
