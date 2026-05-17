// Simple multithreading .id test
// in makefile change (may not be reqd on your compiler) value of USE_THREAD to -pthread

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdint.h>
#include "../src/mstrct.h"

#define NUM_THREADS 8
#define ARRAY_SIZE  10000

M(int *, shared,);

// simple mutex for write synchronization
// library is thread-safe for metadata but user must protect data writes including dealloc/realloc
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void* thread_func(void* arg) {
  int tid = (int)((uintptr_t)pthread_self() % 1000);

  M(int *, view,);
  M(arg, view);                              // share memory

  for (int i = 0; i < 1000; i++) {
    int idx = (tid + i * 13) % ARRAY_SIZE;

    pthread_mutex_lock(&mutex);           // protect write
    m(view, idx) = tid;
    pthread_mutex_unlock(&mutex);
  }

  printf("Thread %d done\n", tid);
  return NULL;
}

int main(void) {
  pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

  printf("=== memstruct .id multithreading test ===\n");

  M(malloc(ARRAY_SIZE * sizeof(int)), shared, ARRAY_SIZE);

  // initialize
  for (int i = 0; i < ARRAY_SIZE; i++)
    m(shared, i) = 0;

  pthread_t threads[NUM_THREADS];

  for (int i = 0; i < NUM_THREADS; i++) {
    pthread_create(&threads[i], NULL, thread_func, M(shared));   // pass shared
  }

  for (int i = 0; i < NUM_THREADS; i++) {
      pthread_join(threads[i], NULL);
  }

  int count = 0;
  for (int i = 0; i < ARRAY_SIZE; i++) {
      if (m(shared, i) != 0) count++;
  }

  printf("Test finished. Modified elements: %d / %d\n", count, ARRAY_SIZE);

  free(shared);
  pthread_mutex_destroy(&mutex);
  return 0;
}

