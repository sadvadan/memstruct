// custom arena allocator

#include <string.h>
#include "../src/mstrct.h"

// define the arena structure
typedef struct {
  M(char,, buffer,);               // memstruct as struct field
  size_t capacity;
  size_t offset;
} Arena;

// initialize the arena with a fixed capacity
Arena arena_init(long unsigned capacity) {
  Arena arena = {};
  M(malloc(capacity), arena.buffer, capacity);
  arena.capacity = capacity;
  arena.offset = 0;
  return arena;
}

// allocate memory from the arena
void *arena_alloc(Arena *arena, size_t size) {
  // basic alignment (align to 8 bytes for safety on most architectures)
  size_t aligned_size = (size + 7) & ~7;

  // check if we have enough room left
  if (arena->offset + aligned_size > arena->capacity) {
    fprintf(stderr, "Arena out of memory!\n");
    return NULL; 
  }

  // get the pointer to the allocated chunk
  void *ptr = &m(arena->buffer, arena->offset);
  
  // Move the offset forward
  arena->offset += aligned_size;

  return ptr;
}

// dummy free for sub arrays
void arena_free(__attribute__((unused)) void *ptr) {
  // not using it will give LEAK error!
}

// reset the arena (O(1) clearing!)
void arena_reset(Arena *arena) {
  arena->offset = 0;
}

int main() {
  // create an arena of 1 KB
  Arena arena = arena_init(1024);

  // allocate an integer
  M(int, const, number,);
  M(arena_alloc(&arena, sizeof(int)), number, 1);
  m(number,0) = 42;

  // allocate an array of floats
  M(float, const, prices,);
  M(arena_alloc(&arena, 5 * sizeof(float)), prices, 5);
 
  for (int i = 0; i < m(prices,_); i++) { // i_max = m(prices,) = 5
    m(prices,i) = i * 10.5f;
  }

  // allocate a string
  M(char, const, greeting,);
  M(arena_alloc(&arena, 13 * sizeof(char)), greeting, 13);

  strncpy(m(greeting,void), "Hello Arena!", m(greeting,sizeof));

  // print the values to verify
  printf("Integer: %d\n", m(number,0));
  printf("Float array: %.1f, %.1f\n", m(prices,0), m(prices,1));
  printf("String: %s\n", m(greeting,void));
  printf("Arena usage: %zu/%zu bytes\n", arena.offset, arena.capacity);

  // dummy frees (comment the below to see LEAK warning)
  M(arena_free, number);
  M(arena_free, prices);
  M(arena_free, greeting);

  // actual deallocation
  M(free, arena.buffer);

  return 0;
}

/* out:
Integer: 42
Float array: 0.0, 10.5
String: Hello Arena!
Arena usage: 48/1024 bytes
*/
