#include "types.h"
#include "x86.h"
#include "defs.h"
#include "param.h"
#include "mmu.h"
#include "proc.h"
#include "sysfunc.h"
#include "pstat.h"

//struct pstat pstat;

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
sys_setpri(void)
{
	int pri;
  	if(argint(0, &pri) < 0)
  		return -1;
  	if(pri!=1 && pri!=2)
  		return -1;
	setpri(pri);
	return 0;
}

int
sys_getpstat(void)
{
	cprintf("gotpstat: %p\n",getpstat());
	return 0;
}

int
sys_getpinfo(void)
{
	//cprintf("in sys_getpinfo: pstat is at %p\n",&pstat);
	struct pstat **p=0;
	if(argptr(0,(char **) p,sizeof(struct pstat))==-1)
		return -1;
	if(*p == NULL)
		return -1;
	//if(argptr(0,p,sizeof(struct pstat *)) < 0);
	//	return -1;
	//cprintf("in sys_getpinfo: after argptr %p \n",*p);
	//*p = getpstat();
	//cprintf("in sys_getpinfo: after getpstat %p \n",*p);
	 **p = pstat;
	
	return 0;
	//return getpinfo((struct pstat *) *p);
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
sys_getprocs(void)
{
  int n = getopenprocs();
  return n;
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
