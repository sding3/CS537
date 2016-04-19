#include "types.h"
#include "stat.h"
#include "user.h"
#include "pstat.h"


int
main(int argc, char *argv[])
{
	struct pstat st;
	setpri(2);
	int ret=99;
	
	//printf(1,"in user proram,before syscall, pstat is at %p\n",&st);
	ret = getpinfo(&st);
	//printf(1,"return value = %d", ret);
	//printf(1,"in user proram,after  syscall, pstat is at %p\n",&st);
	
	int i;
   	for(i = 0; i < NPROC; i++) {
      if (st.inuse[i]) {
         printf(1, "pid: %d hticks: %d lticks: %d\n", st.pid[i], st.hticks[i], st.lticks[i]);
      }
   	}
   	printf(1,"\n\n");
   	getprocs();
   	
	exit();
}

