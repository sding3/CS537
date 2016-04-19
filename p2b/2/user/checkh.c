#include "types.h"
#include "stat.h"
#include "user.h"
#include "pstat.h"
#define check(exp, msg) if(exp) {} else {\
   printf(1, "%s:%d check (" #exp ") failed: %s\n", __FILE__, __LINE__, msg);\
   exit();}
/*
void spin123()
{
	int i = 0, j = 0, k = 0;
	for(i = 0; i < 50; ++i)
	{
		for(j = 0; j < 10000000; ++j)
		{
			k = j % 10;
		}
	}
}
*/
void print(struct pstat *st)
{
   int i;
   for(i = 0; i < NPROC; i++) {
      if (st->inuse[i]) {
         printf(1, "pid: %d hticks: %d lticks: %d\n", st->pid[i], st->hticks[i], st->lticks[i]);
      }
   }

}

void spin()
{
	int i = 0, j = 0, k = 0;
	for(i = 0; i < 50; ++i)
	{
		for(j = 0; j < 10000000; ++j)
		{
			k = j % 10;
		}
	}
}

int
main(int argc, char *argv[])
{
   struct pstat st_before, st_after;
   int highpriority = 0;
   int processwithlowticks = 0;
   check(setpri(2) == 0, "setpri");
   
   int i;
   
   check(getpinfo(&st_before) == 0, "getpinfo");
   printf(1, "\n **** PInfo Before**** \n");
   print(&st_before);
   
   printf(1,"BEFORE SPIN\n");
   
   spin();
  
   printf(1,"After SPIN\n");
   
   check(getpinfo(&st_after) == 0, "getpinfo");
   printf(1, "\n **** PInfo After**** \n");
   print(&st_after);
   
   for(i = 0; i < NPROC; i++) {
      if (st_before.inuse[i] && st_after.inuse[i]) {
	 if(st_before.lticks[i] != st_after.lticks[i])
	 {
		processwithlowticks++;
	 } 
	 if(st_after.hticks[i] - st_before.hticks[i] > 0)
		highpriority++;
      }
   }
   check(processwithlowticks == 0, "low ticks shouldn't have been increased for any of the processes");
   check(highpriority == 1, "getpinfo should return 1 process with hticks greater than 0");
   printf(1, "Should print 1 then 2");
   exit();
}
