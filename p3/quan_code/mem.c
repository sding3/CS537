#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>

typedef struct _header {
    int size; // size of the block, not including the header
    int used; // 1 if used; 0 if free.
    struct _header *next; // pointer to the next block
} header;

static header *head; // static global variable pointing to the start of the region

void *Mem_Init(int sizeOfRegion)
{
    if (sizeOfRegion <= 0 || head != NULL)
    {
        perror("Bad args");
    }

    // sizeOfRegion (in bytes) needs to be evenly divisible by the page size
    int pageSize = getpagesize();
    if (sizeOfRegion%pageSize != 0)
    {
        sizeOfRegion = (sizeOfRegion/pageSize + 1) * pageSize;
    }

    // open the /dev/zero device
    int fd = open("/dev/zero", O_RDWR);

    // call mmap() to get chunk of memory
    void *ptr = mmap(NULL, (size_t)sizeOfRegion, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0);
    if (ptr == MAP_FAILED) // game over
    {
        perror("mmap");
        head = NULL;
        exit(1);
    }

    // close the device (don't worry, mapping should be unaffected)
    close(fd);

    head = ptr;
    head->size = sizeOfRegion - (int) sizeof(header);
    head->used = 0;
    head->next = NULL;

    return head;
}

void *Mem_Alloc(int size)
{
    if (size <= 0) return NULL;

    header *current;
    header *blockPtr = NULL;

    current = head;
    while (current != NULL)
    {
        if (current->used || current->size < size)
        {
            current = current->next;
        }
        else
        {
            current->size = size;
            current->used = 1;
            blockPtr = current + sizeof(header); // calculate block address
            break;
        }
    }

    return blockPtr;
}

int Mem_Free(void *ptr)
{
    if (ptr == NULL) return -1;

    header *blockHdr; // the header of the block ptr points to
    header *node;

    blockHdr = (header *) (ptr - sizeof(header));

    // check if this block was allocated by Mem_Alloc()
    node = head;
    while (node != NULL)
    {
        if (blockHdr == node && node->used) break; // verified
        else
        {
            node = node->next;
            if (node == NULL) return -1; // not found
        }
    }

    blockHdr->used = 0; // free the block
    ptr = NULL;

    return 0;
}

void Mem_Dump()
{
    printf("===== debug info ======\n");

    printf("===== end debug info ======\n");
}


