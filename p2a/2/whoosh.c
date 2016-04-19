#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>

const char error_message[30] = "An error has occurred\n";;
char *path;

void pin(char* in){
	//printf("Path = %s \n",path);
	int argc=0;
	char *token;
	in[strlen(in)-1]=' ';
	char *indup = strdup(in);
	for(token = strtok(in," ");token!=NULL;token=strtok(NULL," "),argc++){
		//printf("%d: %s\n", argc+1, token);
		if(strcmp(token,"path")==0){
			strcpy(path,indup+(strlen(token)));
			return;
		}
	}
	if(argc==0)
		return;
	
	char *argv[argc+1];
	int i=0;
	for(token = strtok(indup," ");token!=NULL;token=strtok(NULL," "),i++){
		//argv[i]=malloc(sizeof(char)*(strlen(token)+1));
		argv[i]= strdup(token);
	}
	argv[argc]=NULL;
	
	if(argc==1&&(strcmp(argv[0],"exit")==0||strcmp(argv[0],"pwd")==0||strcmp(argv[0],"cd")==0)){
		if(strcmp(argv[0],"exit")==0)
			exit(0);
		else if(strcmp(argv[0],"pwd")==0){
			write(STDOUT_FILENO,getcwd(NULL,0),strlen(getcwd(NULL,0)));
			write(STDOUT_FILENO,"\n",1);
		}
		else if(strcmp(argv[0],"cd")==0){
			chdir(getenv("HOME"));
		}else{
			write(STDERR_FILENO, error_message,strlen(error_message));
			//return;
		}
		return;
	}else if((argc==2) && (strcmp(argv[0],"cd")==0)){
		if(chdir(argv[1])!=0)
			write(STDERR_FILENO, error_message,strlen(error_message));
		return;
	}

	//try exec program
	int rc = fork();
	if(rc <0){
		write(STDERR_FILENO, error_message,strlen(error_message));
		exit(1);
	}else if(rc == 0){
		//child
		//printf("Child: \n");
		
		int redir,ind,i;
		for(redir=0,ind=0,i=0;i<argc;i++){
			if(strcmp(argv[i],">")==0){
				redir++;
				ind=i;
			}
		}
		char *file;
		char *err;
		if(redir){
			if(redir==1 && ind ==argc-2){
				close(STDOUT_FILENO);
				file = strcat(strdup(argv[ind+1]),".out");
				if(open(file,O_CREAT|O_TRUNC|O_WRONLY,S_IRUSR|S_IWUSR)== -1){
					write(STDERR_FILENO, error_message,strlen(error_message));
					exit(1);
				}
				close(STDERR_FILENO);
				err = strcat(strdup(argv[ind+1]),".err");
				if(open(err,O_CREAT|O_TRUNC|O_WRONLY,S_IRUSR|S_IWUSR)==-1){
					write(STDERR_FILENO, error_message,strlen(error_message));
					exit(1);
				}
				argv[ind]=NULL;
			}else{
				write(STDERR_FILENO, error_message,strlen(error_message));
				exit(1);
			}
		}

		for(token = strtok(path," ");token!=NULL;token=strtok(NULL," ")){
			
			char *c = malloc(strlen(token)+strlen(argv[0])+4);
			c = strcat(strcat(strcat(c,token),"/"),argv[0]);
			char *tmp = strdup(argv[0]);
			argv[0] = strdup(c);
			//printf("attempting %s\n",argv[0]);
			execv(c,argv);
			strcpy(argv[0],tmp);
			//printf("resttoring argv[0] = %s\n",argv[0]);
			free(c);
		}
		if(redir==1 && ind ==argc-2){
			remove(file);
			remove(err);
		}	
		write(STDERR_FILENO, error_message,strlen(error_message));
		exit(1);
	}else{
		wait(NULL);
	}
}

int main(int argc,char *argv[]){
	char *in = malloc(1000);
	path = malloc(130);
	strcpy(path,"/bin ");
	//allocate array of strings for history of commands

	if(argc == 1){
		for(write(STDOUT_FILENO,"whoosh> ",8);;write(STDOUT_FILENO,"whoosh> ",8)){
			if(fgets(in,1000,stdin)==NULL){
				write(STDERR_FILENO, error_message,strlen(error_message));
			}else if(strlen(in)>129){
				//printf("%d chars long\n",(int) strlen(in));
				write(STDERR_FILENO, error_message,strlen(error_message));
			}else{
				//printf("%d chars long\n",(int) strlen(in));
				pin(in);	
			}
		}
	}else{
		write(STDERR_FILENO, error_message,strlen(error_message));
		return 1;
	}
	return 0;
}
