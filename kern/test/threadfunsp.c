#include <types.h>
#include <lib.h>
#include <thread.h>
#include <synch.h>
#include <test.h>

static int lockcounter;

static struct lock *splock = NULL;
static struct semaphore *tsem = NULL;

static void init_lock(void){
  if(splock==NULL){
	splock = lock_create("splock");
	if(splock == NULL){
			panic("threadtest: lock_create failed\n");
		}
	}
}

static void init_sem(void){
  if (tsem==NULL){
    tsem = sem_create("tsem",0);
    if (tsem == NULL){
      panic("threadtest: sem_create failed\n");  
    }
  }
}

static void mythread5(void *junk, unsigned long num){
 
  int i;
  int loopBound = (int) num; 

  (void)junk; 

  for(i=0; i<loopBound;i++){
    spinlock_acquire(&splock->lk_spinlock);	
    lockcounter++;
    spinlock_release(&splock->lk_spinlock);
 	 }
    V(tsem);
}

static void threadfun5(int NTHREADS, int loopBound){
  char name[16];
  int i, result;

  for(i=0; i<NTHREADS; i++){
    snprintf(name, sizeof(name), "threadtest%d", loopBound);
    result = thread_fork(name, NULL, mythread5, NULL, loopBound);
    if (result) {
      panic("threadtest: thread_fork failed %s)\n",
	    strerror(result));
    }
  }

  for(i=0; i< NTHREADS; i++){
    P(tsem);
  }

}

int threadtest8(int nargs, char **args){

  (void)nargs;
  int NTHREADS = atoi(args[1]);
  int loopBound = atoi(args[2]);

  lockcounter = 0;
  init_sem();
  init_lock();
  kprintf("Starting thread test..\n");
  threadfun5(NTHREADS, loopBound);
  kprintf("\nThread test done.\n");

  kprintf("\nThe value of the spinlock counter is %d. \n", lockcounter);
  kprintf("\nThe value of the spinlock counter should be: %d \n", NTHREADS * loopBound);
  return 0; 
}


