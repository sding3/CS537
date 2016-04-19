#include <stdlib.h>
#include <stdio.h>
#include "mem.h"

int main(int argc, char *argv[])
{
    int sizeOfRegion = atoi(argv[1]);

    // bad
    void *p0 = Mem_Alloc(1000);
    Mem_Free(p0);

    Mem_Init(sizeOfRegion);

    // good
    void *p1 = Mem_Alloc(123);
    void *p2 = Mem_Alloc(456);
    Mem_Alloc(24);
    Mem_Free(p2);
    Mem_Alloc(10556);
    Mem_Free(p1);
    Mem_Alloc(100);

    // bad
    Mem_Alloc(0);
    Mem_Alloc(-17);
    Mem_Alloc(999999999);
    Mem_Free(NULL);
    Mem_Free(p1 + 333);

    Mem_Dump();

    return 0;
}
