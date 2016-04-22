
<html>

<head>
<title>Project 2b: xv6 Scheduler</title> 
</head> 

<body text=black bgcolor=white link=#00aacc vlink=#00aacc alink=orange>

<center><table><tr><td width=1200pt>

<center>
<font color=#00aacc>
<h1>Project 2b: xv6 Scheduler</h1> 
</font> 
</center> 

<h2>Objectives</h2> 

<p>There are two objectives to this assignment:</p> 

<ul>
<li>To familiarize yourself with a real scheduler.</li> 

<li>To change that scheduler to a new algorithm.</li> 
</ul> 

<h2>Updates</h2> 

<p>Read these updates to keep up with any small fixes in the specification.</p> 

<p>When a bad priority (not 1 or 2) is passed to your setpri() system call,
return -1 (indicating an error). Otherwise return 0 (indicating
success). Similarly, if a bad pointer is passed to your getpinfo() call,
return -1; otherwise, if the call is successful, return 0.</p> 

In your Makefile, replace <code>CPUS := 2</code> with <code>CPUS := 1</code> . The
old setting runs xv6 with two CPU cores, but you only need to do
scheduling for a single core in this project.

<h2>Overview</h2> 

<p>In this project, you'll be putting a new scheduler into xv6. It is called a
simple <b>priority-based scheduler</b> . The basic idea is simple: assign each
running process a <b>priority,</b> which is an integer number, in this case
either 1 (low priority) or 2 (high priority). At any given instance, the
scheduler should run processes that have the high priority (2). If there are
two or more processes that have the same high priority, the scheduler should
round-robin between them. A low-priority (level 1) process does NOT run as
long as there are high-priority jobs available to run. Note that this is a
simpler scheduler than the one discussed in the book chapter on
<a href=http://www.ostep.org/cpu-sched-mlfq.pdf>MLFQ.</a> </p> 

<h2>Details</h2> 

<p>You will need to implement a couple of new system calls to implement this
scheduler. The first is <b>int setpri(int num),</b> which sets the priority for
the calling process. By default, each process should get a priority of 1;
calling this routine makes it such that a process can raise or lower its
priority.</p> 

<p>The second is <b>int getpinfo(struct pstat *).</b> This routine returns some
basic information about each running process, including <strike>how many times
it has been chosen to run</strike> how long it has run at each priority (measured in
clock ticks) and its process ID. You can use this system call to build a
variant of the command line program <b>ps,</b> which can then be called to see
what is going on.</p> 

<p>If either of these calls are passed bad parameters, return -1 to indicate a
failure. Otherwise, return 0 upon success.</p> 

<p>One thing you'll have to do is make sure to initialize the priority of a
process correctly. All processes, when created, should start at priority level
1. Only by calling <b>setpri()</b> can a process change its priority to 2.</p> 

<h2>Tips</h2> 

<p>Most of the code for the scheduler is quite localized and can be found in
<b>proc.c.</b> The associated header file, <b>proc.h</b> is also quite useful to
examine. To change the scheduler, not much needs to be done; study its control
flow and then try some small changes.</p> 

<p>You'll need to understand how to fill in the structure <b>pstat</b> in the
kernel and pass the results to user space. The structure looks like what you
see in <a href=pstat.h>here.</a> </p> 

<p><b>Random xv6 tip:</b> To run the xv6 environment, use <b>make qemu-nox.</b> Doing so
avoids the use of X windows and is generally fast and easy. However, quitting
is not so easy; to quit, you have to know the shortcuts provided by the
machine emulator, qemu. Type <b>control-a</b> followed by <b>x</b> to exit the
emulation. There are a few other commands like this available; to see them,
type <b>control-a</b> followed by an <b>h.</b> </p> 

<h2>The Code</h2> 

<p>The source code for xv6 (and associated README) can be found in <b>
~cs537-1/ta/xv6/</b> . Everything you need to build and run and even debug the
kernel is in there.</p> 

<p>You may also find the following readings about xv6 useful, written by the
same team that ported xv6 to x86:
<a href=https://pdos.csail.mit.edu/6.828/2014/xv6/book-rev8.pdf>xv6 book</a> </p> 

<p><b>Particularly useful for this project: Chapter 5.</b> </p> 

<h2>What To Turn In</h2> 

<p>Turn in your source code, as usual.</p> 

</td> </tr> </table> </center> </body> </html> 








