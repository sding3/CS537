#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include "mem.h"

#define USED 0xBEEFFACE
#define FREE 0x1564BEEF

static void *space=NULL;

struct global_header{
	int block_size;
	int biggest_free_slice;
	void *next_free;
};

struct slice_header{
	int magic; //magic is also used to indicate whether slice is free
	int size;
	void *previous_slice;
	void *next_slice;
	void *next_free;
};

static void create_space(int sizeOfRegion){
	int fd = open("/dev/zero",O_RDWR);
	void *ptr = mmap(NULL, sizeOfRegion, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0);
	if(ptr == MAP_FAILED){ perror("mmap"); exit(1);}
	close(fd); //closing the device, mapping won't be affected
	space = ptr;
}

void process_max(){
	printf("processing max [TO DO]\n");
	//after an allocation, biggest_free_slice may change.We handle that in this function
}

void *Mem_Init(int sizeOfRegion){
	int pagesize = getpagesize();
	//rounds up if not a whole page
	if(sizeOfRegion%pagesize!=0)
		sizeOfRegion=((sizeOfRegion/pagesize)+1)*pagesize;
	create_space(sizeOfRegion);
	//Initilize Global Header
	struct global_header *header = space;
	header->next_free = space + sizeof(struct global_header);
	header->block_size = sizeOfRegion;
	header->biggest_free_slice = (header->block_size)-sizeof(struct global_header)-sizeof(struct slice_header);
	//Initialize Slice Header
	struct slice_header *slice = header->next_free;
	slice->magic = FREE;
	slice->size = (header->block_size)-sizeof(struct global_header)-sizeof(struct slice_header);
	slice->previous_slice=NULL;
	slice->next_slice=NULL;
	slice->next_free=NULL;
	return space;
}

void *Mem_Alloc(int size){
	struct global_header *header = space;
	if(size>(header->biggest_free_slice) || header->next_free ==NULL)
		return NULL;//not enough contiguous space left
		
	struct slice_header *fslice = header-> next_free;
	struct slice_header *previous_fslice = NULL;
	
	int found=0;
	int actual_size=0;
	//@ this point, guaranteed to have a contiguous slice
	//just a matter of finding it
	do{
		//seek slice & allocate
		if(fslice->size >= size && fslice->next_slice == NULL){//last free slice
			found=1;
			fslice->size = size;
			fslice->magic = USED;
		}else if(fslice->next_slice != NULL){
			//Maybe I dont need actual_size here If I correct slice_header->size upon each free
			actual_size = fslice->next_slice - (void *)fslice - sizeof(struct slice_header);
			if(actual_size >= size){
				found=1;
				fslice->size = size;
				fslice->magic = USED;
			}
		}
		
		
		//post-processing when found
		if(found){
			if(previous_fslice == NULL && fslice->next_slice == NULL){
				//this branch is for when there is only one free slice & it is the last slice
				
				fslice->next_free = (void *)fslice+sizeof(struct slice_header)+size;		
				fslice->next_slice = fslice->next_free;
				int remain = header->block_size - (fslice->next_free-space);
				printf("%d remainging - ", remain);
				if(remain>=sizeof(struct slice_header)+sizeof(int)){
					struct slice_header *newslice = fslice->next_free;
					newslice->magic = FREE;
					newslice->size =(header->block_size)-((void*) newslice-space)-sizeof(struct slice_header);
					newslice->previous_slice=fslice;
					newslice->next_slice=NULL;
					newslice->next_free=NULL;
				
					header->biggest_free_slice = newslice->size;
					header->next_free = newslice;
				}else{//no more free blocks 
					header->biggest_free_slice = 0;
					header->next_free = NULL;
					fslice->next_free = NULL;
					fslice->next_slice = NULL;
				}
				return fslice+sizeof(struct slice_header);
			}
			//multiple free slices 
			previous_fslice->next_free = fslice->next_free;
			process_max();
			
			//when a large chunk exists after the allocation
			if(actual_size-size>=(sizeof(struct slice_header)+sizeof(int))){
				printf("Large remaining chunk detected !\n");
			}
		}
		//next
		previous_fslice = fslice;
		fslice = fslice->next_free;
		
	}while(!found);
	return NULL;
}
int Mem_Free(void *ptr){
	
	return 0;
}


void Mem_Dump(){
	if(space==NULL){
		printf("Memory has not been initialized yet!\n");
		return;
	}
	
	struct global_header *header = space;
	struct slice_header *slice = space+sizeof(struct global_header);
	
	printf("Global Header: %d\n",(int)sizeof(struct global_header));
	printf("Slice Header Size: %d\n\n\n",(int)sizeof(struct slice_header));
	
	
	printf("-------------------------\n");
	printf("|Global Header @ %p\n",header);
	printf("|Next_Free: %p\n",header->next_free);
	printf("|Block_Size: %d\n",header->block_size);
	printf("|Biggest_Free_Slice: %d\n",header->biggest_free_slice);
	
	int done=0;
	do{
		printf("-------------------------\n");
		printf("|Slice @ %p\n",slice);
		if(slice->magic==FREE)
			printf("|State: FREE\n");
		else if(slice->magic==USED)
			printf("|State: USED\n");
		else
			printf("|State: CORRUPT!!!\n");
			
		printf("|Slice_Size: %d\n",slice->size);
		printf("|Previous_Slice: %p\n",slice->previous_slice);
		printf("|Next_Slice: %p\n",slice->next_slice);
		printf("|Next_Free: %p\n",slice->next_free);
		
		if(slice->next_slice==NULL)
			done=1;
		else
			slice = slice->next_slice;
	}while(!done);
	
	printf("-------------------------\n");
}




