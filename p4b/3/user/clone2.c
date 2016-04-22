/* clone and play with the argument */
#include "types.h"
#include "user.h"

#undef NULL
#define NULL ((void*)0)

#define PGSIZE (4096)

int ppid;
volatile int arg = 55;
volatile int global = 1;

#define assert(x) if (x) {} else { \
   printf(1, "%s: %d ", __FILE__, __LINE__); \
   printf(1, "assert failed (%s)\n", # x); \
   printf(1, "TEST FAILED\n"); \
   kill(ppid); \
   exit(); \
}

void worker(void *arg_ptr);

int
main(int argc, char *argv[])
{
   ppid = getpid();
   void *stack = malloc(PGSIZE*2);
   assert(stack != NULL);
   if((uint)stack % PGSIZE)
     stack = stack + (4096 - (uint)stack % PGSIZE);
   printf(1,"in main: arg_ptr = %p\n",(void*)&arg);
   int clone_pid = clone(worker, (void*)&arg, stack);
   assert(clone_pid > 0);
   printf(1,"main called clone\n");
   sleep(200);
   printf(1,"global = %d\n",global);
   while(global != 55);
   assert(arg == 1);
   printf(1, "TEST PASSED\n");
   exit();
}

void
worker(void *arg_ptr) {
   printf(1,"in worker: arg_ptr = %p\n",arg_ptr);
   printf(1,"in worker: arg_ptr @ mem addr %d\n",&arg_ptr);
   int tmp = *(int*)arg_ptr;
   printf(1,"in worked: dereferenced arg_ptr\n");
   *(int*)arg_ptr = 1;
   assert(global == 1);
   printf(1,"in worked: tmp = %d\n",tmp);
   global = tmp;
   printf(1,"in worked: gloabl = %d\n",global);
   exit();
}
