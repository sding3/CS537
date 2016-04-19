#include "types.h"
#include "stat.h"
#include "user.h"
#include "pstat.h"

void
work(){
  int i,j,k;
  
  for(i=0,k=75;i<1000;i++){
	for(j=0;j<10000;j++){
  		
  		k=j%7;
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

  struct pstat ps;
  getpinfo(&ps);
  
  setpri(2);
  print(&ps);
  work();
  getpinfo(&ps);
  print(&ps);
  
  exit();
  
  
}
