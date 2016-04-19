#include "types.h"
#include "stat.h"
#include "fcntl.h"
#include "user.h"
#include "x86.h"


// calls the underlying join() system call, frees the user stack, and then returns.
int
thread_join(void)
{
  void *join_stack;
  int pid = join(&join_stack);
  if (pid < 0)
      return -1;
  //void **tmp =(void **) (join_stack+4096-12);

  free(join_stack);

  return pid;
}

int
thread_create(void (*start_routine)(void*),void *arg){
  void *stack = malloc(4096*2);
  if (stack == NULL){
    return -1;
  }
  void *temp;
  // to make stack page-aligned
  if((uint)stack % 4096){// if page isn't aligned
    void *new_stack = stack + (4096 - (uint) stack % 4096);
    temp = stack;
    memmove((new_stack+4096-12),(void*) &temp,4);
    stack = new_stack;
  }else{// page is aligned
  	temp = stack;
	memmove((stack+4096-12),(void*) &temp,4);
  }

  return clone(start_routine, arg, stack);
}

// Initializes a spinlock
void
lock_init(lock_t * lock){
  xchg(&lock->flag, 0);
}

void
lock_acquire(lock_t * lock)
{
  while(xchg(&lock->flag, 1) == 1)
    ;//spins
}

// Releases a spinlock
void
lock_release(lock_t * lock)
{
  xchg(&lock->flag, 0);
}
