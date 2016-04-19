#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include "mem.h"

typedef struct _header
{
    int size; // size of the block, not including the header
    int used; // 1 if used; 0 if free.
    struct _header *next; // pointer to the next block
} Header;

typedef struct _GlobalHeader
{
    int totalSpace; // size of total space from mmap() call
    void *next;
} GlobalHeader;

static void *space; // static global variable pointing to the start of the region

void *Mem_Init(int sizeOfRegion)
{
    if (sizeOfRegion <= 0 || space != NULL)
    {
        perror("Bad args");
        exit(1);
    }

    // sizeOfRegion (in bytes) needs to be evenly divisible by the page size
    int pageSize = getpagesize();
    if (sizeOfRegion % pageSize != 0)
    {
        sizeOfRegion = (sizeOfRegion / pageSize + 1) * pageSize;
    }

    // open the /dev/zero device
    int fd = open("/dev/zero", O_RDWR);

    // call mmap() to get chunk of memory
    void *ptr = mmap(NULL, (size_t) sizeOfRegion, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0);
    if (ptr == MAP_FAILED) // game over
    {
        perror("mmap");
        space = NULL;
        exit(1);
    }

    // close the device (don't worry, mapping should be unaffected)
    close(fd);

    space = ptr; // set global static pointer

    // initialize global header
    GlobalHeader *head = space;
    head->totalSpace = sizeOfRegion;
    head->next = space + (int) sizeof(GlobalHeader);

    // initialize first block header
    Header *blockHdr = head->next;
    blockHdr->size = head->totalSpace - (int) sizeof(GlobalHeader) - (int) sizeof(Header);
    blockHdr->used = 0;
    blockHdr->next = NULL;

    return head;
}

void *Mem_Alloc(int size)
{
    if (size <= 0 || space == NULL) return NULL;

    GlobalHeader *head = space;
    Header *current = head->next;
    void *blockPtr = NULL; // begin address of block space, not including header

    while (current != NULL)
    {
        // find first free block that fits requested size
        if (current->used || current->size < size)
        {
            current = current->next;
        }
        else
        {
            int oldSize = current->size; // original size of the block
            current->size = size;
            current->used = 1;
            blockPtr = (void *)current + sizeof(Header); // calculate block address
            int remainSize = oldSize - size;
            if (remainSize > sizeof(Header)) // if there is enough space left to create a new block
            {
                // create a new block
//                printf("new block size: %d\n", remainSize - (int) sizeof(Header));
                Header *newBlockHdr = (void *)current + sizeof(Header) +size;
                newBlockHdr->size = remainSize - (int) sizeof(Header);
                newBlockHdr->next = current->next;
                current->next = newBlockHdr;
            }
            break;
        }
    }

    if (current == NULL) return NULL;

    return blockPtr;
}

int Mem_Free(void *ptr)
{
    if (ptr == NULL || space == NULL) return -1;

    GlobalHeader *head = space;
    Header *blockHdr; // the header of the block ptr points to
    Header *node;

    blockHdr = (Header *) (ptr - sizeof(Header));

    // check if this block was allocated by Mem_Alloc()
    node = head->next;
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

    return 0;
}

void Mem_Dump()
{
    printf("===== debug info ======\n");

    if (space == NULL)
    {
        printf("Memory has not been initialized yet!\n");
        return;
    }

    GlobalHeader *head = space;

    printf("Page size: %d\n", getpagesize());
    printf("Global Header size: %d\n", (int) sizeof(GlobalHeader));
    printf("Block Header size: %d\n", (int) sizeof(Header));
    printf("Global Header address: %p\n", head);
    printf("Total space: %d\n\n", head->totalSpace);

    printf("-------------------------\n");

    Header *node = head->next;

    int i = 1;
    while (node != NULL)
    {
        printf("| Block#: %d\n", i);
        printf("| Block header address: %p\n", node);
        printf("| Block size: %d\n", node->size);
        printf("| Block status: %s\n\n", node->used ? "USED" : "FREE");
        node = node->next;
        i++;
    }

    printf("-------------------------\n");

    printf("===== end debug info ======\n");
}


