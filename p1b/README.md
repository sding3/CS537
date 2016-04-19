
<html>

<head>
<title>Project 1b: xv6 Intro</title> 
</head> 

<body text=black bgcolor=white link=#00aacc vlink=#00aacc alink=orange>

<center><table><tr><td width=1200pt>

<center>
<font color=#00aacc>
<h1>Project 1b: xv6 Intro</h1> 
</font> 
</center> 

<p>We'll be doing kernel hacking projects in <b>xv6.</b> Xv6 is a port of a
classic version of unix to a modern processor, Intel's x86. It is a clean and
beautiful little kernel, and thus a perfect object for our study and usage.</p> 

<p>This first project is just a warmup, and thus relatively light on work. The
goal of the project is simple: to add a system call to xv6. Your system call,
<b>getprocs()</b> , simply returns how many processes exist in the system at the
time of the call.</p> 

<h2>Details</h2> 

<p>Your new syscall should look like this: <b>int getprocs(void)</b> </p> 

<p>Your system call returns the number of processes that exist in the system
at the time of the call.</p> 

<h2>Tips</h2> 

<p>Find some other system call, like <b>getpid()</b> or any other simple
call. Basically, copy it in all the ways you think are needed. Then modify it
to do what you need.</p> 

<p>Most of the time will be spent on understanding the code. There shouldn't
be a whole lot of code added.</p> 

<p>Using gdb (the debugger) may be helpful in understanding code, doing code
traces, and is helpful for later projects too. Get familiar with this fine
tool!</p> 

<h2>The Code</h2> 

<p>The source code for xv6 (and associated README) can be found in <b>
~cs537-1/ta/xv6/</b> . Everything you need to build and run and even debug the
kernel is in there.</p> 


<p>You may also find the following readings about xv6 useful, written by the
same team that ported xv6 to x86:
<a href=https://pdos.csail.mit.edu/6.828/2014/xv6/book-rev8.pdf>xv6 book.</a> 
However, note that the kernel version we use is a little different than the
book.</p> 

<p><b>Particularly useful for this project: Chapters 0, 1, 2.</b> </p> 

</td> </tr> </table> </center> </body> </html> 








