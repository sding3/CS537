#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int
main(int argc,char *argv[]){
  int var = 123;
  printf("size of var is %d\n",(int)sizeof(var));
  const int magic = 0xBEEFFACE;
  memcpy((void*)&var,&magic,sizeof(magic));

  printf("clone: var is %x\n",*(int*)&var);
  return 0;
}

