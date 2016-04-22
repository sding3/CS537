

<html>

<head>
<title>Project 3: Malloc and Free</title> 
</head> 

<body text=black bgcolor=white link=#00aacc vlink=#00aacc alink=orange>

<center><table><tr><td width=1200pt>

<center>
<font color=#00aacc>
<h1>Project 3a: Malloc and Free</h1> 
</font> 
</center> 

<h2>Only section 3 does this part!</h2> 

<h2>Objectives</h2> 

<p>There are three objectives to this part of the assignment:
<ul>
<li>To understand the nuances of building a memory allocator.</li> 
<li>To do so in a performance-efficient manner.</li> 
<li>To create a shared library.</li> 
</ul> 
</p> 

<h2>Note</h2> 

<p>At some point you will decide to use a header per each allocated block. The
maximum size of such a header is 32 bytes.</p> 

<h2>Overview</h2> 

<p>In this project, you will be implementing a memory allocator for the heap
of a user-level process. Your functions will be to build your own malloc() and
free().</p> 

<p>Memory allocators have two distinct tasks. First, the memory allocator asks
the operating system to expand the heap portion of the process's address space
by calling either sbrk or mmap. Second, the memory allocator doles out this
memory to the calling process. This involves managing a free list of memory
and finding a contiguous chunk of memory that is large enough for the user's
request; when the user later frees memory, it is added back to this list.</p> 

<p>This memory allocator is usually provided as part of a standard library and
is not part of the OS. To be clear, the memory allocator operates entirely
within the virtual address space of a single process and knows nothing about
which physical pages have been allocated to this process or the mapping from
logical addresses to physical addresses.</p> 

<p>When implementing this basic functionality in your project, we have
a couple guidelines. First, when requesting memory from the OS, you
must use <b>mmap()</b> (which is easier to use than sbrk()). Second,
although a real memory allocator requests more memory from the OS
whenever it can't satisfy a request from the user, your memory
allocator must call mmap() only one time (when it is first
initialized).

<p>Classic malloc() and free() are defined as follows:
<ul>
<li>void *malloc(size_t size): malloc() allocates size bytes and returns a
pointer to the allocated memory. The memory is not cleared.</li> 
<li>void free(void *ptr): free() frees the memory space pointed to by ptr,
which must have been returned by a previous call to malloc() (or calloc() or
realloc()). Otherwise, or if free(ptr) has already been called before,
undefined behaviour occurs. If ptr is NULL, no operation is performed.</li> 
</ul> </p> 

<p>For simplicity, your implementations of Mem_Alloc(int size) and
Mem_Free(void *ptr) should basically follow what malloc() and free()
do; see below for details.</p> 

<p>You will also provide a supporting function, Mem_Dump(),
described below; this routine simply prints which regions are currently free
and should be used by you for debugging purposes.</p> 

<h2>Program Specifications</h2> 

<p>For this project, you will be implementing several different routines as
part of a shared library. Note that you will not be writing a main() routine
for the code that you handin (but you should implement one for your own
testing). We have provided the prototypes for these functions in the file
<b>mem.h</b> (which is available at ~cs537-3/public/mem.h); you should
include this header file in your code to ensure that you are adhering to the
specification exactly. <b>You should not change mem.h in any way!</b> We now
define each of these routines more precisely.</p> 

<p><ul>

<li><b>void *Mem_Init(int sizeOfRegion):</b> Mem_Init is called
one time by a process using your routines. sizeOfRegion is the number
of bytes that you should request from the OS using mmap. Mem_Init
must return the address of the region returned by mmap (this will be
used by our tests). If mmap fails, return NULL. Outside of the
mmap'd region, you are allowed only one static variable used to point
to the region. Note that this means you need to use the allocated
memory region returned by mmap for your own data structures as well;
that is, your infrastructure for tracking the mapping from addresses
to memory objects has to be placed in this region as well. You are not
allowed to use malloc(), or any other related function, in any of your
routines! Similarly, you should not allocate global arrays!
<p></p> 
Note that you may need to round up sizeOfRegion so that you request memory in
units of the page size (see the man pages for getpagesize()).</li> 
<p></p> 

<li><b>void *Mem_Alloc(int size):</b> Mem_Alloc() is similar to the
library function malloc(). Mem_Alloc takes as input the size in bytes
of the object to be allocated and returns a pointer to the start of
that object. The function returns NULL if there is not enough
contiguous free space within <b>sizeOfRegion</b> allocated by Mem_Init to
satisfy this request.</li> 
<p></p> 

<li><b>int Mem_Free(void *ptr):</b> Mem_Free() frees the memory
object that ptr points to. Just like with the standard free(), if ptr is
NULL, then no operation is performed. The function returns 0 on success, and
-1 otherwise.</li> 
<p></p> 

<li><b>void Mem_Dump():</b> This is just a debugging routine for your own
use. Have it print the regions of free memory to the screen.</li> 
<p></p> 
</ul> </p> 

<p>You are not required to coalesce memory. However, free'd regions
should be reusable for future allocations that are equal or smaller.</p> 


<h2>Shared Library</h2> 

<p>You must provide these routines in a shared library named
"libmem.so". Placing the routines in a shared library instead of a simple
object file makes it easier for other programmers to link with your
code. There are further advantages to shared (dynamic) libraries over static
libraries. When you link with a static library, the code for the entire
library is merged with your object code to create your executable; if you link
to many static libraries, your executable will be enormous. However, when you
link to a shared library, the library's code is not merged with your program's
object code; instead, a small amount of stub code is inserted into your object
code and the stub code finds and invokes the library code when you execute the
program. Therefore, shared libraries have two advantages: they lead to smaller
executables and they enable users to use the most recent version of the
library at run-time. To create a shared library named libmem.so, use the
following commands (assuming your library code is in a single file "mem.c"):</p> 

<p><pre>
gcc -c -fpic mem.c -Wall -Werror
gcc -shared -o libmem.so mem.o
</pre> </p> 

<p>To link with this library, you simply specify the base name of the library
with "-lmem" and the path so that the linker can find the library "-L.".</p> 
<p><pre>
gcc -lmem -L. -o myprogram mymain.c -Wall -Werror
</pre> </p> 

<p>Of course, these commands should be placed in a Makefile. Before you run
"myprogram", you will need to set the environment variable, LD_LIBRARY_PATH,
so that the system can find your library at run-time. Assuming you always run
myprogram from this same directory, you can use the command:</p> 
<p><pre>
export LD_LIBRARY_PATH=.:$LD_LIBRARY_PATH
</pre> </p> 

<p>Note that export is what you use in bash; if you are not using
bash, you'll have to figure out the alternative command to set the
environment.</p> 

<h2>Unix Hints</h2> 

<p>In this project, you will use mmap to map zero'd pages (i.e., allocate new
pages) into the address space of the calling process. Note there are a number
of different ways that you can call mmap to achieve this same goal; we give
one example here:</p> 

<p><pre>
// open the /dev/zero device
int fd = open("/dev/zero", O_RDWR);

// sizeOfRegion (in bytes) needs to be evenly divisible by the page size
void *ptr = mmap(NULL, sizeOfRegion, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0);
if (ptr == MAP_FAILED) { perror("mmap"); exit(1); }

// close the device (don't worry, mapping should be unaffected)
close(fd);
return 0;
</pre> </p> 

<h2>Grading</h2> 

<p>Your implementation will be graded on functionality. However,
we will also be comparing the performance of each of your projects,
so try to be efficient!</p> 


</p> </td> </tr> </table> </center> </body> 








