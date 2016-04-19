#include <stdlib.h>
#include <stdio.h>
#include "mem.h"

int main(int argc, char *argv[]){
	void *space = Mem_Init(4096);
	printf("got space %p\n",space);
	
	int *x1;
	int *x2;
	
	x1=Mem_Alloc(8);
	x2=Mem_Alloc(16);
	
	Mem_Free(x1);
	Mem_Free(x2);
	//Mem_Dump();
	
	//x1=Mem_Alloc(32);
	//Mem_Dump();
	
	x2=Mem_Alloc(8);
	Mem_Dump();
	
	/*
	Mem_Free(x1);
	Mem_Free(x2);
	Mem_Dump();
	*/
	return 0;
}
