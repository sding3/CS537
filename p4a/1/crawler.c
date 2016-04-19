#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdint.h>
#include <pthread.h>
#include "ha.h"

/*
OVERVIEW
1) queue of links (parsers to downloaders). Fixed size
2) queue of pages (downloaders to parsers). Unbounded
3) hash set (to make sure you don't visit the same page twice)

QUEUE OF LINKS(fixed)
parser waits when queue is full (need CV)
downloaders waits when queue is empty (another CV)
need mutex 

QUEUE OF PAGE(unbounded)
parser waits when queue is empty (need CV)
downloader doesn't need to wait
need mutex

HASH SET
need mutex

TODO - WAITING UNTIL DONE
1 CV, 1 mutex

*/

int gotWork=0;
pthread_mutex_t workMutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t workEmpty = PTHREAD_COND_INITIALIZER;

//Hashset data structure components
hash_table_t *pagesHash = NULL;
pthread_mutex_t hashMutex = PTHREAD_MUTEX_INITIALIZER;

//Both the linksQ and the pagesQ uses the node struct
typedef struct node{
	char *data;
	struct node *next;
} NODE;

//Utility struct for passing callback functions to threads
//Not really needed, but nice to have
typedef struct args{
	void *arg1;
	void *arg2;
} ARGS;

//linksQ data structure components
NODE *linksQ=NULL;
int linksMax=0;
int linksCount=0;
pthread_cond_t linksEmpty = PTHREAD_COND_INITIALIZER;
pthread_cond_t linksFill = PTHREAD_COND_INITIALIZER;
pthread_mutex_t linksMutex = PTHREAD_MUTEX_INITIALIZER;

//pagessQ data structure components
NODE *pagesQ=NULL;
int pagesCount=0;
pthread_cond_t pagesFill = PTHREAD_COND_INITIALIZER; 
pthread_mutex_t pagesMutex = PTHREAD_MUTEX_INITIALIZER;


NODE *make_node(char* str){
	NODE *node;
	if(!(node=malloc(sizeof(NODE)))) return NULL;
	if(!(node->data = malloc(strlen(str)+1))) return NULL;
	strcpy(node->data, str);
	node->next = NULL;
	return node;
}

//LINKS QUEUE OPERATIONS
	//parser produces link
void push_link(NODE* node){
	node->next = linksQ;
	linksQ = node;
	linksCount++;
}
	//downloader consumes link
void pop_link(){
	assert(linksQ!=NULL);
	free(linksQ->data);
	free(linksQ);
	linksQ=linksQ->next;
	linksCount--;
}

//PAGES QUEUE OPERATIONS
	//downloader produces page
void push_page(NODE* node){
	node->next = pagesQ;
	pagesQ = node;
	pagesCount++;
}
	//parser consumes page
void pop_page(){
	assert(pagesQ!=NULL);
	free(pagesQ->data);
	free(pagesQ);
	pagesQ=pagesQ->next;
	pagesCount--;
}


// parser consumes pages & produces links
void *parser(void* arguments){
	ARGS *args = arguments;
	void (*_edge_fn)(char *from, char *to) = args->arg2;

	while(gotWork){
		//BEGIN consume page
		pthread_mutex_lock(&pagesMutex);//LOCK pagesQ
		while(pagesCount==0)
			pthread_cond_wait(&pagesFill,&pagesMutex);
		char *page=strdup(pagesQ->data);
		pop_page();
		pthread_mutex_unlock(&pagesMutex);//UNLCOK pagesQ
		//END consume page
		
		//BEGIN produce links
		char *save;
		char *token;
		char *from = strtok_r(page, " \n", &save);
		hash_table_t *link_hash_table;
		link_hash_table = create_hash_table(17);
		
		printf("Parser consuming PAGE: %s \n",from);

		for(token=strtok_r(NULL, " \n", &save);//parsing
			token!=NULL; 
			token=strtok_r(NULL, " \n", &save)){
			
    		if((strncmp("link:",token ,5)==0) && 
			   (!add_string(link_hash_table,token+5))){

				_edge_fn(from,token+5);
				NODE *new = make_node(token+5);

				pthread_mutex_lock(&linksMutex);//LOCK linksQ
				while(linksCount == linksMax)
					pthread_cond_wait(&linksEmpty,&linksMutex);
				push_link(new);
				pthread_cond_signal(&linksFill);
				pthread_mutex_unlock(&linksMutex);//UNLCOK linksQ
			}
		}
		//END produce links
		free(page);
		free_table(link_hash_table);
	}
	return NULL;
}


// downloader consumes links * produce pages
void *downloader(void* arguments ){
	ARGS *args = arguments;
	char * (*_fetch_fn)(char *url) = args->arg1;

	while(gotWork){
		//BEGIN consume link
		pthread_mutex_lock(&linksMutex); //LOCK linksQ
		while(linksCount==0)
			pthread_cond_wait(&linksFill,&linksMutex);
		char *link=strdup(linksQ->data);
		pop_link();
		pthread_cond_signal(&linksEmpty);
		pthread_mutex_unlock(&linksMutex); //UNLOCK linksQ
		//END consume link
		
		//BEGIN produce page
		pthread_mutex_lock(&hashMutex);
		if(!add_string(pagesHash,link)){//link is not duplicate & added to hash
			pthread_mutex_unlock(&hashMutex);
			char *page=_fetch_fn(link);	
			printf("downloader got link: %s\n",link);
			assert(page!=NULL);
			char* page1 = malloc(strlen(page)+strlen(link)+5);
			strcpy(page1,link);
			strcat(strcat(page1, " "),page);
			NODE *new = make_node(page1);
			free(link);
			free(page);
			free(page1);

			pthread_mutex_lock(&pagesMutex);//LOCK pagesQ
			push_page(new);
			//printf("downloader produced PAGE:%s. %d pages in Q\n",link,pagesCount);
			pthread_cond_signal(&pagesFill);
			pthread_mutex_unlock(&pagesMutex);//UNLCOK pagesQ
		}else{
			pthread_mutex_unlock(&hashMutex);
			printf("downloader thinks %s is a duplicate.\n",link);
			free(link);
		}
		//END produce page
	}
	return NULL;
}

int crawl(char *start_url,
	  int download_workers,
	  int parse_workers,
	  int queue_size,//size of queue of links
	  char * (*_fetch_fn)(char *url),
	  void (*_edge_fn)(char *from, char *to)) {
	
	struct args * arguments = malloc(sizeof(struct args));
	arguments->arg1 = _fetch_fn;
	arguments->arg2 = _edge_fn;
	linksMax = queue_size;
	NODE* new = make_node(start_url);
	push_link(new);
	gotWork++;
	pagesHash = create_hash_table(53);
	
	pthread_t t;
	int i;
	for(i=0;i<download_workers;i++)
		pthread_create(&t,NULL,downloader,(void *) arguments);
	for(i=0;i<parse_workers;i++)
		pthread_create(&t,NULL,parser,(void *) arguments);
	
	sleep(6);

//TODO: when are we really done?

	free(pagesHash);
  	return -1;
}
