#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

char *path;

void
parse(char *in){
	printf("Path = %s \n",path);
	int argc = 0;
	char *token;
	//if(in[strlen(in)-1]=='\n')
	in[strlen(in)-1]=' ';
	char *indup = strdup(in);
	for(token = strtok(in," ");token!=NULL;token=strtok(NULL," "),argc++){
		printf("%d: %s\n", argc+1, token);
		if(strcmp(token,"path")==0){
			printf("found path!! %s\n",indup+(strlen(token)));
			strcpy(path,indup+(strlen(token)));
			printf("path %s\n",path);
			break;
		}
	}
	printf("argc = %d\n",argc);
	
	char *argv[argc+1];
	int i=0;
	for(token = strtok(indup," ");token!=NULL;token=strtok(NULL," "),i++){
		argv[i]=malloc(sizeof(char)*(strlen(token)+1));
		argv[i]= strdup(token);
	}
	argv[argc]=NULL;
	
	
	/*
	for(i=0;i<argc;i++){
		printf("%s\n",argv[i]);
		free(argv[i]);
	}
	*/
}

int
main(int argc,char *argv[]){
	char *in = malloc(514);
	path = malloc(130);
	for(;;){
			printf("input> ");
			fgets(in,130,stdin);
			int l = strlen(in);
			if(l>128){
				// input too long
				printf(" input too long");
			}else{/*process input*/
				parse(in);
			}
	}
}
