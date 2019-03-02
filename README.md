# Thread_Safe_Malloc
implement two different thread-safe versions (i.e. safe for concurrent access by different threads of a process) of the malloc() and free() functions.

In version 1 of the thread-safe malloc/free functions, you may use lock-based synchronization to
prevent race conditions that would lead to incorrect results. These functions are to be named:

//Thread Safe malloc/free: locking version
void *ts_malloc_lock(size_t size);
void ts_free_lock(void *ptr);

In version 2 of the thread-safe malloc/free functions, you may not use locks, with one exception.
Because the sbrk function is not thread-safe, you may acquire a lock immediately before
calling sbrk and release a lock immediately after calling sbrk. These functions are to be
named:

//Thread Safe malloc/free: non-locking version
void *ts_malloc_nolock(size_t size);
void ts_free_nolock(void *ptr);

To provide necessary synchronization for the locking version, you may use support from the
pthread library and needed synchronization primitives (e.g. pthread_mutex_t, etc.). Also, don’t
forget about Thread-Local Storage -- you may find that useful.

In order to exercise and test the thread-safe malloc routines, pthread-based multi-threaded
sample programs will be provided. These programs will create threads which will make
concurrent calls to the thread-safe malloc and free functions. You are also strongly encouraged
to create your own multi-threaded test programs to test your library code. Recall that concurrent
execution means that multiple threads will call the malloc and free functions, and thus may be
concurrently reading and updating any shared data structures used by the malloc routines (e.g.
free list information).
