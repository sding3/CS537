#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int
main(int argc,char *argv[]){
	char *str1,*str2,*token;
	int j;
	
	for(j = 1, str1 = argv[1]; ; j++, str1 = NULL){
		token = strtok(str1,argv[2]);
		if(token == NULL)
			break;
		printf("%d: %s\n", j, token);
	}
	
	return 0;
}

