#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>


const int HSIZE = 20;
const char error_message[30] = "An error has occured\n";
const char *EXIT = "exit";
const char *PATH = "path";
const char *PWD= "pwd";
const char *CD= "cd";
char *path;
int hn = 0;
int ind = 0;


void
pin(char* in){
	printf("Path = %s \n",path);
	int argc=0;
	char *token;
	in[strlen(in)-1]=' ';
	char *indup = strdup(in);
	for(token = strtok(in," ");token!=NULL;token=strtok(NULL," "),argc++){
		printf("%d: %s\n", argc+1, token);
		if(strcmp(token,PATH)==0){
			strcpy(path,indup+(strlen(token)));
			return;
		}
	}
	if(argc==0)
		return;
	
	char *argv[argc+1];
	int i=0;
	for(token = strtok(indup," ");token!=NULL;token=strtok(NULL," "),i++){
		argv[i]=malloc(sizeof(char)*(strlen(token)+1));
		argv[i]= strdup(token);
	}
	argv[argc]=NULL;
	
	if(strcmp(argv[0],EXIT)==0||strcmp(argv[0],PWD)==0||strcmp(argv[0],CD)==0){
		if(strcmp(argv[0],EXIT)==0)
			exit(0);
		else if(strcmp(argv[0],PWD)==0 && argc==1)
			printf("%s\n",getcwd(NULL,0));
		else if(strcmp(argv[0],CD)==0 && argc==1)
			chdir(getenv("HOME"));
		else if(strcmp(argv[0],CD)==0 && argc==2){
			if(chdir(argv[1])!=0)
				write(STDERR_FILENO, error_message,strlen(error_message));
		}
		return;
	}

	//try exec program
	int rc = fork();
	if(rc <0){
		write(STDERR_FILENO, error_message,strlen(error_message));
		exit(1);
	}else if(rc == 0){
		//child
		printf("Child: \n");
		for(token = strtok(path," ");token!=NULL;token=strtok(NULL," ")){
			char *c = malloc(strlen(token)+strlen(argv[0])+4);
			c = strcat(strcat(strcat(c,token),"/"),argv[0]);
			//printf("attemping %s\n",c);
			execv(c,argv);
			free(c);
		}
		write(STDERR_FILENO, error_message,strlen(error_message));
		exit(1);
	}else{
		wait(NULL);
	}
}

int
main(int argc,char *argv[]){
	char *in = malloc(130);
	path = malloc(130);
	//allocate array of strings for history of commands

	if(argc == 1){
		for(printf("whoosh> "); ;printf("whoosh> ")){
			fgets(in,130,stdin);
			if(strlen(in)>128){
				write(STDERR_FILENO, error_message,strlen(error_message));
			}else{
				in[strlen(in)]='\0';
				pin(in);	
			}
		}
	}else{
		write(STDERR_FILENO, error_message,strlen(error_message));
		return 1;
	}
	return 0;
}
