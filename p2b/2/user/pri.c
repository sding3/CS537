#include "types.h"
#include "stat.h"
#include "user.h"
#include "pstat.h"

void
work(){
  int i,j,k;
  
  for(i=0,k=75;i<100000;i++){
	for(j=0;j<10000;j++){
  		
  		k=j%7;
	}
  }
}

int
main(int argc, char *argv[])
{

  getprocs();
  setpri(2);
  work();
  getprocs();
  exit();
}
