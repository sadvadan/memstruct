// multithreading

#define MSTRCT_TNO 8 // IMPORTANT: define it above #include mstrct.h
#include "../src/mstrct.h"
#include <pthread.h>

#define ARRAY_SIZE  10000


// worker (writes its own local/global memory, reads global: no locks, atomics etc
void* native_thread(void *arg) {

  // IMPORTANT: write this at top of scope to set thread ID for the thread:
  M(arg);

  // thread's own global:
  m(view, 1, int, do);
  M(view, malloc, 1000*sizeof(int));

  for (int i = 0; i < 1000; i++) {
  // access (for read) 1000 elems of (global) shared
    m(view,i) = m(arg,i,int);
  }

  M(view, free);
  printf("Thread(I) %d done\n", mstrct_tid); // thread ID
  return 0;
}

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
// worker (writes shared global memory): mutex used

void* library_thread(void *arg) {
  M(arg);

  m(view, 1, int, do);
  // alias (for write) 1000 elems of (global) shared
  M(m(arg,int,void), view, 1000);

  for (int i = 0; i < 1000; i++) {
  // protect write!
    pthread_mutex_lock(&mutex);
    m(view,i) = 2*i;
    pthread_mutex_unlock(&mutex);
  }

  printf("Thread(II) %d done\n", mstrct_tid);
  return 0;
}

int main(void) {

  // simple memstruct to hold (shared) memory
  M(int,, shared,);
  M(malloc(ARRAY_SIZE * sizeof(int)), shared, ARRAY_SIZE);

  // initialize shared
  for (int i = 0; i < m(shared,_); i++)
    m(shared,i) = i;

  // allocate pthreads on stack
  M(pthread_t,, threads,);
  M(auto, threads, MSTRCT_TNO);

  printf("=== memstruct multithreading-I starts ===\n"); ///////////////////////////////////////////////////////////

  for (int i = 0; i < m(threads,_); i++) {
  // pass shared
    pthread_create(&m(threads,i), NULL, native_thread, (void *)M(void, shared, i));
  }

  for (int i = 0; i < m(threads,_); i++) {
    pthread_join(m(threads,i), NULL);
  }

  int count = 0;
  for (int i = 0; i < m(shared,_); i++) {
    if (m(shared,i) != 0) count++;
  }

  printf("multi-threading-I finished. Modified elements: %d / %d\n", count+1, ARRAY_SIZE);
 
  printf("=== memstruct multithreading-II starts ===\n"); ///////////////////////////////////////////////////////////

  for (int i = 0; i < m(threads,_); i++) {
    pthread_create(&m(threads,i), NULL, library_thread, (void *)M(void, shared, i));
  }

  for (int i = 0; i < m(threads,_); i++) {
      pthread_join(m(threads,i), NULL);
  }

  count = 0;
  for (int i = 0; i < m(shared,_); i++) {
      if (m(shared,i) != 0) count++;
  }

  printf("multi-threading-II finished. Modified elements: %d / %d\n", count+1, ARRAY_SIZE);

  M(free, shared);
  pthread_mutex_destroy(&mutex);
  return 0;
}
/* out (typical):
=== memstruct multithreading-I starts ===
Thread(I) 0 done
Thread(I) 1 done
Thread(I) 4 done
Thread(I) 3 done
Thread(I) 7 done
Thread(I) 5 done
Thread(I) 2 done
Thread(I) 6 done
multi-threading-I finished. Modified elements: 10000 / 10000
=== memstruct multithreading-II starts ===
Thread(II) 1 done
Thread(II) 2 done
Thread(II) 0 done
Thread(II) 3 done
Thread(II) 4 done
Thread(II) 5 done
Thread(II) 7 done
Thread(II) 6 done
multi-threading-II finished. Modified elements: 10000 / 10000
*/
