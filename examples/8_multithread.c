// multithreading

#define MSTRCT_TNO 8 // IMPORTANT: define it above #include mstrct.h
#include "../src/mstrct.h"
#include <pthread.h>

#define ARRAY_SIZE  10000


// worker (writes its own local/global memory, reads global: no locks, atomics etc
void* native_thread(void *arg) {

  M(arg); // IMPORTANT: write this here to set thread ID for the thread:

  // thread's own global:
  m(view, 1, int);
  M(view, malloc, 1000*sizeof(int));

  for (int i = 0; i < 1000; i++) {
  // access (for read) 1000 elems of (global) shared
    m(view,i) = m(arg, i, int[], _);   // note the usage to get cross-thread read-only data
  }

  M(view, free);
  printf("Thread(I) %d done\n", mstrct_tid); // thread ID
  return 0;
}

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
#define MSTRCTM   // switching this flag to enable read-write shared memory in memstruct

// worker (writes shared global memory): mutex used
void* library_thread(void *arg) {
  M(arg);

  for (int i = 0; i < 1000; i++) {
  // protect write!
    pthread_mutex_lock(&mutex);
    m(arg, i, int[], do) = 2*i;   // note the usage to get cross-thread read-write data
    pthread_mutex_unlock(&mutex);
  }

  printf("Thread(II) %d done\n", mstrct_tid);
  return 0;
}

int main(void) {

  // simple memstruct to hold (shared) memory
  m(shared, 1, int);
  M(shared, malloc, ARRAY_SIZE * sizeof(int));

  // initialize shared
  for (int i = 0; i < m(shared,_); i++)
    m(shared,i) = i;

  // allocate pthreads on stack
  m(threads, MSTRCT_TNO, pthread_t, auto);

  printf("=== memstruct multithreading-I starts ===\n"); ///////////////////////////////////////////////////////////

  for (int i = 0; i < m(threads,_); i++) {
  // create threads, share shared
    pthread_create(&m(threads,i), NULL, native_thread, m(shared, auto, i));
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
    pthread_create(&m(threads,i), NULL, library_thread, m(shared, auto, i));
  }

  for (int i = 0; i < m(threads,_); i++) {
      pthread_join(m(threads,i), NULL);
  }

  count = 0;
  for (int i = 0; i < m(shared,_); i++) {
      if (m(shared,i) != 0) count++;
  }

  printf("multi-threading-II finished. Modified elements: %d / %d\n", count+1, ARRAY_SIZE);

  M(shared, free);
  pthread_mutex_destroy(&mutex);
  return 0;
}
/* out (typical):
=== memstruct multithreading-I starts ===
Thread(I) 2 done
Thread(I) 1 done
Thread(I) 3 done
Thread(I) 8 done
Thread(I) 6 done
Thread(I) 7 done
Thread(I) 4 done
Thread(I) 5 done
multi-threading-I finished. Modified elements: 10000 / 10000
=== memstruct multithreading-II starts ===
Thread(II) 1 done
Thread(II) 2 done
Thread(II) 4 done
Thread(II) 3 done
Thread(II) 8 done
Thread(II) 7 done
Thread(II) 5 done
Thread(II) 6 done
multi-threading-II finished. Modified elements: 10000 / 10000
*/
