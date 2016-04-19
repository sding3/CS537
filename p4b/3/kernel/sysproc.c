#include "types.h"
#include "x86.h"
#include "defs.h"
#include "param.h"
#include "mmu.h"
#include "proc.h"
#include "sysfunc.h"

int
sys_fork(void)
{
  return fork();
}

int
sys_exit(void)
{
  exit();
  return 0;  // not reached
}

int
sys_wait(void)
{
  return wait();
}

int
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

int
sys_getpid(void)
{
  return proc->pid;
}

int
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  addr = proc->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

int
sys_sleep(void)
{
  int n;
  uint ticks0;
  
  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(proc->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

// return how many clock tick interrupts have occurred
// since boot.
int
sys_uptime(void)
{
  uint xticks;
  
  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

int
sys_clone(void){
  //int clone(void(*fcn)(void*), void *arg, void *stack)
  void (*func)(void*);
  void *arg;
  void *stack;

  if(argptr(0,(void*)&func, sizeof(*func))<0 || 
     argptr(1,(void*)&arg,sizeof(*arg))<0 ||
     argptr(2,(void*)&stack,sizeof(*stack))<0){
	 return -1;
  }

  //TODO: size of page passed?
  
  if((uint)stack%4096 || ((uint)proc->sz-(uint)stack)<4096)
    return -1;
  //cprintf("sys_clone: arg is %p\n",arg);
  return clone(func,arg,stack);
}

int
sys_join(void){
  void **stack;

  if(argptr(0,(void*)&stack, sizeof(**stack))<0){
    return -1;
  }
  if((uint)stack%4!=0){
    return -1;
  }
  //cprintf("stack is %d\n",(int)stack);
  //*stack = joinstack;


  return join(stack); //tmp
}


