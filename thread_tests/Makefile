CC=gcc
CFLAGS=-O3
#MALLOC_VERSION=LOCK_VERSION
MALLOC_VERSION=NOLOCK_VERSION
WDIR=../

all: thread_test thread_test_malloc_free thread_test_malloc_free_change_thread thread_test_measurement

thread_test: thread_test.c
	$(CC) -g $(CFLAGS) -I$(WDIR) -D$(MALLOC_VERSION) -L$(WDIR) -Wl,-rpath=$(WDIR) -o $@ thread_test.c -lmymalloc -lrt -lpthread

thread_test_malloc_free: thread_test_malloc_free.c
	$(CC) -g $(CFLAGS) -I$(WDIR) -D$(MALLOC_VERSION) -L$(WDIR) -Wl,-rpath=$(WDIR) -o $@ thread_test_malloc_free.c -lmymalloc -lrt -lpthread

thread_test_malloc_free_change_thread: thread_test_malloc_free_change_thread.c
	$(CC) -g $(CFLAGS) -I$(WDIR) -D$(MALLOC_VERSION) -L$(WDIR) -Wl,-rpath=$(WDIR) -o $@ thread_test_malloc_free_change_thread.c -lmymalloc -lrt -lpthread

thread_test_measurement: thread_test_measurement.c
	$(CC) -g $(CFLAGS) -I$(WDIR) -D$(MALLOC_VERSION) -L$(WDIR) -Wl,-rpath=$(WDIR) -o $@ thread_test_measurement.c -lmymalloc -lrt -lpthread

clean:
	rm -f *~ *.o thread_test thread_test_malloc_free thread_test_malloc_free_change_thread thread_test_measurement

clobber:
	rm -f *~ *.o
