#include <stdio.h>
#include <stdlib.h>
#include <string.h>


struct line{
	char *wholeline;
	char *word;
};

struct line *createLine(char *wholeline, char *word){
	//allocate memoroy for the pointers themselves 
	//for line struct
	struct line *l = malloc(sizeof(struct line));
	if( l == NULL){
        fprintf(stderr,"malloc failed");
        exit(1);
    }
	
	l->wholeline = strdup(wholeline);
 	if( l->wholeline == NULL){
	    fprintf(stderr,"malloc failed");
        exit(1);	
	} 
	l->word = strdup(word);
	if( l->word == NULL){
        fprintf(stderr,"malloc failed");
        exit(1);
    }
	return l;
}

int
main(int argc,char *argv[]){
	struct line *l1 = createLine("my first line","my");
	printf("line: %s \n",l1->wholeline);
    return 0;
}
