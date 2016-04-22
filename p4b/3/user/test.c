#include "types.h"
#include "stat.h"
#include "user.h"

int x = 10;

void
dumbfunc123(void *arg){
  printf(1, "arg modulo 5!\n");
}

void
dumbfunc(void* inc){
  printf(1,"dumbfunc: arg = %x\n",inc);
  printf(1,"dumbfunc: arg is @ mem addr %d\n",&inc);
  //int *tmp = inc;
  printf(1,"dumbfunc: x = %d\n",x);
  //x+= *tmp ;
  sleep(50);
  exit();
}

int
main(int argc, char *argv[])
{
  printf(1,"x = %d\n",x);
  int inc = 22;
  /*
  void *stack = malloc(4096);
  if((uint)stack % 4096)
    stack = stack + (4096 - (uint)stack % 4096);
  */
  printf(1,"in main: arg  = %p\n",(void*)&inc);
  //printf(1, "pointer is %d bytes\n",sizeof(page));
  printf(1, "int is %d bytes\n",sizeof(int));
  //int pid = clone(dumbfunc,(void*) &inc,stack);
  int pid = thread_create(dumbfunc,(void*) &inc);
  printf(1,"in main: PID = %d\n",pid);


  thread_join();





  //sleep(200);
  
  //printf(1,"x = %d\n",x);

  exit();
}
