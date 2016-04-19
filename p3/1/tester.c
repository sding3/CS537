#include <stdlib.h>
#include <stdio.h>
#include "mem.h"

int main(int argc, char *argv[]){
	void *space = Mem_Init(4096);
	printf("got space %p\n",space);
	
	int *x1;
	int *x2;
	int *x3;
	x1 = Mem_Alloc(sizeof(int));
	if(x1 == NULL)
		printf("couldn't allocate!\n");
	*x1 = 12345;
	x2 = Mem_Alloc(sizeof(int));
	if(x2 == NULL)
		printf("couldn't allocate!\n");
	*x2 = 23456;
	x3 = Mem_Alloc(sizeof(int));
	if(x3 == NULL)
		printf("couldn't allocate!\n");
	*x3 = 34567;
	
	/*
	int i =1;
	while(Mem_Alloc(24)!=NULL){
		printf("got 4 bytes @ %d\n",i*36+16);
		i++;
	}
	*/
	//int *ptr = Mem_Alloc(sizeof(int));
	
	
	Mem_Dump();
	
	printf("%d \n%d\n%d\n",*x1,*x2,*x3);
	return 0;
}
