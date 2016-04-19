#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

struct line{
	char *wholeline;
	char *word;
};

int line_cmp(const void *a, const void *b)
{
    struct line *ia = *(struct line * const *)a;
    struct line *ib = *(struct line * const *)b;
    return strcmp(ia->word, ib->word);
}
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
main(int argc, char *argv[]){
	char *input_file;
	long int column = 1;
	if(argc==2){
		input_file = argv[1];
	}else if(argc==3){
		//process the option needs to be in this format "-x"
		char *option = argv[1];
		if (option[0]!='-'){
			fprintf(stderr,"Error: Bad command line parameters\n");
			exit(1);
		}
		column = strtol(option,NULL,10); 
		if (column >= 0){
			fprintf(stderr,"Error: Bad command line parameters\n");
            exit(1);
		}
		column = -column;
		//printf("column: %ld\n",column);
		input_file = argv[2];	
	}else{
		fprintf(stderr,"Error: Bad command line parameters\n");
		exit(1);
	}
	
	FILE *fp=fopen(input_file, "r");
	if(fp == NULL){
		fprintf(stderr,"Error: Cannot open file %s\n",input_file);
		exit(1);
	}

	//file is opened	
	char *str = malloc(sizeof(char)*131); 
	const char nlc = '\n';
	char *newline;
	int len;
	int filelen=0;
	//get line number and check for line that's too long
	while( fgets(str,129,fp)!=NULL ) {
		newline = strchr(str,nlc);
		if(newline == NULL){
			fprintf(stderr,"Line too long\n");
			exit(1);
		}
		len = strlen(str);
		filelen++;
    }
	rewind(fp);
	struct line *x[filelen];
	filelen=0;
	//populate array of struct pointers
	int tok ;
	while( fgets(str,129,fp)!=NULL ){
		char *wholeline = strdup(str);
		if(wholeline == NULL ){
			fprintf(stderr,"malloc failed");
			exit(1);
		}
		tok = 1;
		char *token = strtok(str," ");
		char *lasttoken = token;
		while(tok < column){
			token = strtok(NULL," ");
			if (token != NULL)
				lasttoken = token;
			else{
				token = lasttoken;
				tok = column;
			}
			tok++;
		}
		//printf("\nTOKEN: %s ",token);
		struct line *l1 = createLine(wholeline,token);			
 	    x[filelen++] = l1;
	}
	fclose(fp);	
	int i;
	/*for(i=0;i<filelen;i++){
		printf("%d:",i);
        printf("%s",x[i]->wholeline);
    }*/

	qsort(x, filelen, sizeof(struct line *), line_cmp);	
		
	for(i=0;i<filelen;i++){
		//printf("%d:",i);
		//printf("<%s>",x[i]->word);
		printf("%s",x[i]->wholeline);
	}
	return 0;
}
