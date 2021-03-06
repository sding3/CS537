#include "types.h"
#include "stat.h"
#include "user.h"
#include "pstat.h"
#define check(exp, msg) if(exp) {} else {\
   printf(1, "%s:%d check (" #exp ") failed: %s\n", __FILE__, __LINE__, msg);\
   exit();}

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

void print(struct pstat *st)
{
   int i;
   for(i = 0; i < NPROC; i++) {
      if (st->inuse[i]) {
         printf(1, "pid: %d hticks: %d lticks: %d\n", st->pid[i], st->hticks[i], st->lticks[i]);
      }
   }

}


int
main(int argc, char *argv[])
{
 
   struct pstat st_before, st_after;
   int lowpriorityrun = 0, highpriorityrun = 0;
   int rc = fork();
   int i;
   check(setpri(2) == 0, "setpri");
   if(rc == 0)
   {

   	check(setpri(2) == 0, "setpri");
	sleep(200);
	int j = 0, k = 0;
	for(i = 0; i < 50; ++i)
	{
		for(j = 0; j < 10000000; ++j)
		{
			k = j % 10;
		}
	}
	exit();
   }
   int rc1 = fork();
   //running low priority process
   if(rc1 == 0)
   {
	sleep(200);
	int j = 0, k = 0;
	for(i = 0; i < 50; ++i)
	{
		for(j = 0; j < 10000000; ++j)
		{
			k = j % 10;
		}
	}
        printf(1, " then 2");
	exit();
   }

   
   sleep(200);
   check(getpinfo(&st_before) == 0, "getpinfo");
   printf(1, "\n ****PInfo before**** \n");
   print(&st_before);
   int j = 0, k = 0;
   for(i = 0; i < 50; ++i)
	{
		for(j = 0; j < 10000000; ++j)
		{
			k = j % 10;
		}
	}
	 
   check(getpinfo(&st_after) == 0, "getpinfo");
   printf(1, "\n ****PInfo after**** \n");
   print(&st_after);
   for(i = 0; i < NPROC; ++i)
   {
	if(st_before.lticks[i] < st_after.lticks[i])
			lowpriorityrun++;
	if(st_before.hticks[i] < st_after.hticks[i])
			highpriorityrun++;
   }
   check(lowpriorityrun == 0, "Expected no low priority processes to run when high priority running");
   check(highpriorityrun == 2, "Expected two high priority processes to be run");
   printf(1, "Should print 1"); 
   sleep(200);
   wait();
   wait();
   exit();
}
