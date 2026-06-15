// multithreading

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdint.h>
#include "../src/mstrct.h"

#define NUM_THREADS 8
#define ARRAY_SIZE  10000

M(int *, shared,);                        // memstruct to hold (shared) memory

// simple mutex for write synchronization
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

// worker
void* thread_func(void *arg) {
  int tid = (int)((uintptr_t)pthread_self() % 1000);

  M(int *, view,) = {0};
  view.id = (uint64_t)arg;                  // share memory

  for (int i = 0; i < 1000; i++) {
    int idx = (tid + i * 13) % ARRAY_SIZE;

    pthread_mutex_lock(&mutex);           // protect write!
    m(view, idx) = tid;
    pthread_mutex_unlock(&mutex);
  }

  printf("Thread %d done\n", tid);
  return NULL;
}

int main(void) {
  pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

  printf("=== memstruct multithreading test ===\n");

  M(malloc(ARRAY_SIZE * sizeof(int)), shared, ARRAY_SIZE);

  // initialize
  for (int i = 0; i < m(span shared); i++)    // m(span shared) = i_max = ARRAY_SIZE
    m(shared, i) = 0;

  M(pthread_t *, threads,);
  M(auto, threads, NUM_THREADS);          // allocate pthreads on stack

  for (int i = 0; i < m(span threads); i++) { // m(span threads) = i_max = NUM_THREADS
    pthread_create(&m(threads,i), NULL, thread_func, (void *)(uint64_t)shared.id);   // pass shared
  }

  for (int i = 0; i < m(span threads); i++) { // m(span threads) = i_max = NUM_THREADS
      pthread_join(m(threads,i), NULL);
  }

  int count = 0;
  for (int i = 0; i < m(span shared); i++) { // m(span shared) = i_max = ARRAY_SIZE
      if (m(shared, i) != 0) count++;
  }

  printf("Test finished. Modified elements: %d / %d\n", count, ARRAY_SIZE);

  M(free, shared);
  pthread_mutex_destroy(&mutex);
  return 0;
}
/* out (typical):
=== memstruct multithreading test ===
Thread 616 done
Thread 304 done
Thread 680 done
Thread 992 done
Thread 368 done
Thread 432 done
Thread 56 done
Thread 744 done
Test finished. Modified elements: 3120 / 10000
*/
