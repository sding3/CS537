#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int cstring_cmp(const void *a, const void *b) 
{ 
    const char **ia = (const char **)a;
    const char **ib = (const char **)b;
    return strcmp(*ia, *ib);
	/* strcmp functions works exactly as expected from
 	* 	comparison function */ 
} 

char *x[4];

int
main(int argc,char *argv[]){
	x[0] = "bob asdasd";
	x[1] = "aple asdd";
    x[2] = "dd asds";
    x[3] = "cc 231";
	size_t len = sizeof(x)/sizeof(char*);	
	
	int i; 
	for(i=0; i<4; i++){
		printf("%s\n",x[i]);
	}

	qsort(x, len, sizeof(char *), cstring_cmp);

    for(i=0; i<4; i++){
        printf("%s\n",x[i]);
    }

	return 0;
}


