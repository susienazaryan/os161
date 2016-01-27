#include <types.h>
#include <lib.h>
#include <thread.h>
#include <synch.h>
#include <test.h>

#define VNAME 10

static struct semaphore *tsem = NULL;

static void init_sem(void)
{

	if (tsem==NULL) {
		tsem = sem_create("tsem", 0);
		if (tsem == NULL) {
				panic("threadtest: sem_create failed\n");
		}
	}

}

static void mythread(void *junk, unsigned long num) {
	int ch = '0' + num;
	int i;

	(void)junk;

	for( i=0; i<100; i++) {
		putch(ch);
	}

	V(tsem);
}

static void threadfun( ) {
	char name[16];
	int i, result;

	for(i = 0; i<VNAME; i++) {
		snprintf(name, sizeof(name), "threadtest%d", i);
		result = thread_fork(name, NULL, mythread, NULL, i);

	if(result) {
		panic("threadtest: thread_fork failed %s)\n", strerror(result));
		}
	}

	for (i=0; i<VNAME; i++) {
		P(tsem);
	}
}

int threadtest4( ) {

	init_sem();
	kprintf("starting thread test...\n");
	threadfun();
	kprintf("\nThread test done.\n");

	return 0;

}

