#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int
main(int argc,char *argv[]){
	void *ptr;
	printf("page size is %d.\n",getpagesize());
	printf("pointer size is %d.\n",(int) sizeof(ptr));
	printf("int size is %d.\n",(int) sizeof(int));
	return 0;
}

