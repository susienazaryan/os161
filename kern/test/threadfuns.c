/* Collaborated with Megan and Lindsey */

#include <types.h>
#include <lib.h>
#include <thread.h>
#include <synch.h>
#include <test.h>

static int safethreadcounter;

static struct semaphore *newSem = NULL;
static struct semaphore *tsem = NULL;

static void init_sem(void)
{

	if (newSem==NULL) {
		newSem = sem_create("newSem", 1);
		if (newSem == NULL) {
				panic("threadtest: sem_create failed\n");
		}
	}

	if (tsem == NULL) {
		tsem = sem_create("tsem", 0);
		if(tsem == NULL) {
				panic("threadtest: sem_create failed\n");
			}
		}

}


static void mythread3(void *junk, unsigned long num) {
	(void)junk;
	int upperBound = (int) num;
	int i;
	
	for( i=0; i<upperBound; i++){
	P(newSem);
	safethreadcounter++;
	V(newSem);
	}

	V(tsem);
}

static void threadfun3( int VNAME, int upperBound ) {
	char name[16];
	int i, result;

	for(i = 0; i<VNAME; i++) {
		snprintf(name, sizeof(name), "threadtest%d", upperBound);
		result = thread_fork(name, NULL, mythread3, NULL, upperBound);

	if(result) {
		panic("threadtest: thread_fork failed %s)\n", strerror(result));
		}
	}

	for (i=0; i<VNAME; i++) {
		P(tsem);
	}
}

int threadtest6(int nargs, char **args ) {
	
	(void)nargs;
	int VNAME = atoi(args[1]);
	int upperBound = atoi(args[2]);
	safethreadcounter = 0;	
	init_sem();
	kprintf("starting thread test...\n");
	threadfun3(VNAME, upperBound);
	kprintf("\nThread test done.\n");
	kprintf("\nThe value of the safethreadcounter is %d. \n", safethreadcounter);
	kprintf("\nThe value of the safethreadcounter SHOULD be %d. \n", VNAME * upperBound );
	return 0;

}


