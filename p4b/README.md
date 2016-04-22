
<html>

<head>
<title>Project 4b: xv6 Threads</title> 
</head> 

<body text=black bgcolor=white link=#00aacc vlink=#00aacc alink=orange>

<center><table><tr><td width=1200pt>

<center>
<font color=#00aacc>
<h1>Project 4b: xv6 Threads</h1> 
</font> 
</center> 

<h2>Overview</h2> 

<p>In this project, you'll be adding real kernel threads to xv6. Sound like
fun? Well, it should. Because you are on your way to becoming a real kernel
hacker. And what could be more fun than that?</p> 

<p>Specifically, you'll do three things. First, you'll define a new system
call to create a kernel thread, called <code>clone()</code> , as well as one to wait
for a thread called <code>join()</code> . Then, you'll use <code>clone()</code> to build a
little thread library, with a <code>thread_create()</code> call and <code>
lock_acquire()</code> and <code>lock_release()</code> functions. Finally, you'll show
these things work by using the TA's tests. That's it! And now, for some
details.</p> 

<h2>Details</h2> 

<p>Your new clone system call should look like this: <code>int
clone(void(*fcn)(void*), void *arg, void*stack)</code> . This call creates a new
kernel thread which shares the calling process's address space. File
descriptors are copied as in fork. The new process uses <code>stack</code> as its
user stack, which is passed the given argument <code>arg</code> and uses a fake
return PC (0xffffffff). The stack should be one page in size and
page-aligned. The new thread starts executing at the address specified by
<code>fcn</code> . As with fork(), the PID of the new thread is returned to the
parent.</p> 

<p>The other new system call is <code>int join(void **stack)</code> . This call waits
for a child thread that shares the address space with the calling process. It
returns the PID of waited-for child or -1 if none. The location of the
child's user stack is copied into the argument <code>stack</code> .</p> 

<p>You also need to think about the semantics of a couple of existing system
calls. For example, <code>int wait()</code> should wait for a child process that does not
share the address space with this process. It should also free the address
space if this is last reference to it. Finally, <code>exit()</code> should work as
before but for both processes and threads; little change is required here.</p> 

<p>Your thread library will be built on top of this, and just have a simple
<code>thread_create(void (*start_routine)(void*), void *arg)</code> routine. This
routine should call <code>malloc()</code> to create a new user stack, use <code>
clone()</code> to create the child thread and get it running. A <code>thread_join()</code> 
call should also be used, which calls the underlying <code>join()</code> system call,
frees the user stack, and then returns.</p> 

<p>Your thread library should also have a simple spin lock. There should be a
type <code>lock_t</code> that one uses to declare a lock, and two routines <code>
lock_acquire(lock_t *)</code> and <code>lock_release(lock_t *)</code> , which acquire and
release the lock. The spin lock should use x86 atomic exchange to built the
spin lock (see the xv6 kernel for an example of something close to what you
need to do). One last routine, <code>lock_init(lock_t *)</code> , is used to
initialize the lock as need be.</p> 

<p>To test your code, use the TAs tests, as usual! But of course you should
write your own little code snippets to test pieces as you go.</p> 

<p>One thing you need to be careful with is when an address space is grown by
a thread in a multi-threaded process. Trace this code path carefully and see
where a new lock is needed and what else needs to be updated to grow an
address space in a multi-threaded process correctly.</p> 

<h2>Contest</h2> 

<p>The contest for this project will be a little different than usual: it is
based on who builds the <b>best</b> threads library, which should be more fully
functional than what is specified above. It can include things like <b>
condition variables</b> (as well as demonstrations that they work), <b>more
advanced locks</b> than spin locks (like ones that go to sleep when they cannot
acquire the lock), and other advanced features as discussed in the book. The
best few submissions will be selected for a demo, and then the best overall
threading system will be chosen.</p> 

<h2>The Code</h2> 

<p>The code (and associated README) can be found in <b>
~cs537-1/ta/xv6/</b> . Everything you need to build and run and even debug the
kernel is in there, as before.</p> 

<p>As usual, it might be good to read the xv6 book a bit: <a href=xv6book.pdf>
Here</a> .</p> 

<p>You may also find this book useful:
<a href=http://download.savannah.gnu.org/releases/pgubook/ProgrammingGroundUp-1-0-booksize.pdf>
Programming from the Ground Up</a> . Particular attention should be paid to the
first few chapters, including the calling convention (i.e., what's on the
stack when you make a function call, and how it all is manipulated).</p> 

</td> </tr> </table> </center> </body> </html> 










