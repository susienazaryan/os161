/* Collaborated with Megan and Lindsey */

#include <types.h>
#include <lib.h>
#include <thread.h>
#include <synch.h>
#include <test.h>

static int unsafethreadcounter;
static int safethreadcounter;

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


static void mythread2(void *junk, unsigned long num) {
	int upperBound = (int) num;
	int i;
	
	(void)junk;
	for( i=0; i<upperBound; i++){
	unsafethreadcounter++;
	safethreadcounter++;
	}

	V(tsem);
}

static void threadfun2( int VNAME, int upperBound ) {
	char name[16];
	int i, result;

	for(i = 0; i<VNAME; i++) {
		snprintf(name, sizeof(name), "threadtest%d", upperBound);
		result = thread_fork(name, NULL, mythread2, NULL, upperBound);

	if(result) {
		panic("threadtest: thread_fork failed %s)\n", strerror(result));
		}
	}

	for (i=0; i<VNAME; i++) {
		P(tsem);
	}
}

int threadtest5(int nargs, char **args ) {
	
	(void)nargs;
	int VNAME = atoi(args[1]);
	int upperBound = atoi(args[2]);
	unsafethreadcounter = 0;	
	safethreadcounter = 0;
	init_sem();
	P(tsem);
	kprintf("starting thread test...\n");
	threadfun2(VNAME, upperBound);
	kprintf("\nThread test done.\n");
	V(tsem);
	kprintf("\nThe value of the unsafethreadcounter is %d. \n", unsafethreadcounter);
	kprintf("\nThe value of the unsafethreadcounter SHOULD be %d. \n", VNAME * upperBound );
	kprintf("\nThe value of the safethreadcounter is %d. \n", safethreadcounter);
	return 0;

}

/* static void int threadtest6( int nargs, char **args ) {

	int VNAME = atoi(args[1]);
	int upperBound = atoi(args[2]);

	init_sem();
	sem_lock;
	kprintf("The safe thread counter has counted " );
	
}
*/

