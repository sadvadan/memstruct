// custom arena allocator

#include <string.h>
#include "../src/mstrct.h"
#include <stdint.h>
#include <assert.h>

// define the arena structure
typedef struct {
  m(buffer, 1, char);               // memstruct as struct field
  size_t capacity;
  size_t offset;
} Arena;

// initialize the arena with a fixed capacity
Arena arena_init(long unsigned capacity) {
  Arena arena;
  M(arena.buffer, malloc, capacity);
  arena.capacity = capacity;
  arena.offset = 0;
  return arena;
}

// allocate memory from the arena
void *arena_alloc(Arena *arena, size_t size, size_t alignment) {
  // ensure alignment is a power of 2
  assert(alignment && !(alignment & (alignment - 1)));

  uintptr_t current_addr = (uintptr_t)&m(arena->buffer, arena->offset);
  
  size_t padding = (alignment - (current_addr % alignment)) % alignment;
  size_t total_size = size + padding;
  if (arena->offset + total_size > arena->capacity) {
      return NULL; // out of memory
  }

  void *ptr = (void *)(current_addr + padding);
  arena->offset += total_size;

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
  m(number, 1, int);
  M(number, arena_alloc, &arena, sizeof(int), _Alignof(int));
  m(number,0) = 42;

  // allocate an array of floats
  m(prices, 1, float);
  M(prices, arena_alloc, &arena, 5 * sizeof(float), _Alignof(float));
 
  for (int i = 0; i < m(prices,_); i++) { // i_max = m(prices,) = 5
    m(prices,i) = i * 10.5f;
  }

  // allocate a string
  m(greeting, 1, char);
  M(greeting, arena_alloc, &arena, 13 * sizeof(char), _Alignof(char));

  strncpy(&m(greeting), "Hello Arena!", m(greeting,_));

  // print the values to verify
  printf("Integer: %d\n", m(number,0));
  printf("Float array: %.1f, %.1f\n", m(prices,0), m(prices,1));
  printf("String: %s\n", &m(greeting));
  printf("Arena usage: %zu/%zu bytes\n", arena.offset, arena.capacity);

  // dummy frees (comment the below to see LEAK warning)
  M(number, arena_free);
  M(prices, arena_free);
  M(greeting, arena_free);

  // actual deallocation
  M(arena.buffer, free);

  return 0;
}

/* out:
Integer: 42
Float array: 0.0, 10.5
String: Hello Arena!
Arena usage: 37/1024 bytes
*/
