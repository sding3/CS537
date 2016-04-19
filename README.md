##UW-Madison CS537 Section 3, Spring 2016
---------------------------------------------
### Project 1: Warm-up
http://pages.cs.wisc.edu/~remzi/Classes/537/Spring2016/Projects/p1.html

Part a: a simple sorting program to warm up with C
Part b: the goal of the project is simple: to add a system call to xv6. Your system call, getprocs() , simply returns how many processes exist in the system at the time of the call.

---------------------------------------------
### Project 2: Processes and Scheduling
http://pages.cs.wisc.edu/~remzi/Classes/537/Spring2016/Projects/p2.html

Part a: implement a simple command line interpreter (CLI) or, as it is more commonly known, a shell.
Part b: implement a new scheduler into xv6. It is called a simple priority-based scheduler. 

---------------------------------------------
### Project 3: Memory Management
http://pages.cs.wisc.edu/~remzi/Classes/537/Spring2016/Projects/p3-section3.html

Write your own malloc() and free().

---------------------------------------------
### Project 4: Concurrency
http://pages.cs.wisc.edu/~remzi/Classes/537/Spring2016/Projects/p4.html

Part a: build a scalable web crawler 
Part b: add thread support into xv6. Implement the following system calls:
	int clone(void(*fcn)(void*), void *arg, void*stack)
	int join(void **stack)
	and create a thread library.


