
<html>

<head>
<title>Project 2a: A Unix Shell</title> 
</head> 

<body text=black bgcolor=white link=#00aacc vlink=#00aacc alink=orange>

<center><table><tr><td width=1200pt>

<center>
<font color=#00aacc>
<h1>Project 2a: The Unix Shell</h1> 
</font> 
</center> 

<h2>Objectives</h2> 

<p>There are three objectives to this assignment:</p> 

<ul>
<li>To familiarize yourself with the Linux programming environment.</li> 

<li>To learn how processes are created, destroyed, and managed.</li> 

<li>To gain exposure to the necessary functionality in shells.</li> 
</ul> 

<h2>Updates</h2> 

<p>Read these updates to keep up with any small fixes in the specification.</p> 

<p>When a command is not found in the path, do not print <b>command not
found,</b> but rather the only error message.</p> 

<p>By default, the path should be set to <b>/bin</b> so that the user can type a
few commands without first setting the path. Type <code>ls /bin</code> to see what
commands are available.</p> 

<p>You do not have to support multiple commands being executed concurrently or
being specified on the same command line; each command line will contain at
most one command to run.</p> 


<h2>Overview</h2> 

<p>In this assignment, you will implement a <b>command line interpreter (CLI)</b> 
or, as it is more commonly known, a <b>shell.</b> The shell should operate in
this basic way: when you type in a command (in response to its prompt), the
shell creates a child process that executes the command you entered and then
prompts for more user input when it has finished.</p> 

<p>The shells you implement will be similar to, but simpler than, the one you
run every day in Unix. You can find out which shell you are running by typing
<code>"echo $SHELL"</code> at a prompt. You may then wish to look at the man pages
for the shell you are running (probably <code>bash</code> ) to learn more about all
of the functionality that can be present. For this project, you do not need to
implement too much functionality.</p> 

<h2>Program Specifications</h2> 

<h3>Basic Shell: WhooSH</h3> 

<p>Your basic shell, called <code>whoosh</code> , is basically an interactive loop:
it repeatedly prints a prompt <code>"whoosh> "</code> (note the space after the
greater-than sign), parses the input, executes the command specified on that line
of input, and waits for the command to finish. This is repeated until the user
types "exit". The name of your final executable should be <b>whoosh:</b> 

<blockquote><pre>
prompt> ./whoosh
whoosh>
</pre> </blockquote> 

<p>You should structure your shell such that it creates a new process for each
new command (note that there are a few exceptions to this, which we discuss
below). There are two advantages of creating a new process. First, it protects
the main shell process from any errors that occur in the new command. Second,
it allows for concurrency; that is, multiple commands can be started and
allowed to execute simultaneously. <b>However, in this project, you do not
have to build any support for running multiple commands at once.</b> </p> 

<p>Your basic shell should be able to parse a command, and run the
program corresponding to the command. For example, if the user types
<code>"ls -la /tmp"</code> , your shell should run the program <code>/bin/ls</code> with
all the given arguments and print the output on the screen.</p> 

<p>You might be wondering how the shell knows to run <code>/bin/ls</code> (which
means the program binary <code>ls</code> is found in the directory <code>/bin</code> ) when
you type <code>ls</code> . The shells knows this thanks to a <b>path</b> variable that
the user sets. The path variable contains the list of all directories to
search, in order, when the user types a command. We'll learn more about how to
deal with the path below.</p> 

<p><b>Important:</b> Note that the shell itself does not "implement" <code>ls</code> 
or really many other commands at all. All it does is find those executables in
one of the directories specified by <code>path</code> and create a new process to
run them. More on this below.</p> 

<p>The maximum length of a line of input to the shell is 128 bytes.</p> 

<h3>Built-in Commands</h3> 

<p>Whenever your shell accepts a command, it should check whether the command
is a <b>built-in command</b> or not. If it is, it should not be executed like
other programs. Instead, your shell will invoke your implementation of the
built-in command. For example, to implement the <code>exit</code> built-in command,
you simply call <code>exit(0);</code> in your C program.</p> 

<p>So far, you have added your own <code>exit</code> built-in command. Most Unix
shells have many others such as <code>cd</code> , <code>echo</code> , <code>pwd</code> , etc. In
this project, you should implement <b>exit,</b> <b>cd,</b> <b>pwd,</b> and <b>path.</b> </p> 

<p>The formats for <b>exit,</b> <b>cd,</b> and <b>pwd</b> are:</p> 

<pre><blockquote>
[optionalSpace]exit[optionalSpace]
[optionalSpace]pwd[optionalSpace]
[optionalSpace]cd[optionalSpace]
[optionalSpace]cd[oneOrMoreSpace]dir[optionalSpace]
</blockquote> </pre> 

<p>When you run <code>cd</code> (without arguments), your shell should change the
working directory to the path stored in the $HOME environment variable. Use
the call <code>getenv("HOME")</code> in your source code to obtain this value.</p> 

<p>You do not have to support tilde (~). Although in a typical Unix shell you
could go to a user's directory by typing <code>cd ~username,</code> in this project
you do not have to deal with tilde. You should treat it like a common
character, i.e., you should just pass the whole word (e.g. "~username") to
chdir(), and chdir will return an error.</p> 

<p>Basically, when a user types <code>pwd,</code> you simply call getcwd(), and show
the result. When a user changes the current working directory (e.g. "cd
somepath"), you simply call chdir(). Hence, if you run your shell, and then
run pwd, it should look like this:</p> 

<pre><blockquote>
% cd
% pwd
<b>/afs/cs.wisc.edu/u/m/j/username</b> 
% echo $PWD
<b>/u/m/j/username</b> 
% ./whoosh
<b>whoosh> pwd</b> 
<b>/afs/cs.wisc.edu/u/m/j/username</b> 
</blockquote> </pre> 

<p>The format of the <b>path</b> built-in command is:
<pre><blockquote>
[optionalSpace]path[oneOrMoreSpace]dir[optionalSpace] (and possibly
more directories, space separated)
</blockquote> </pre> </p> 

<p>A typical usage would be like this:
<pre><blockquote>
<b>whoosh> path /bin /usr/bin</b> 
</blockquote> </pre> 
By doing this, your shell will know to look in <code>/bin</code> and <code>/usr/bin</code> 
when a user types a command, to see if it can find the proper binary to
execute. If the user sets path to be empty, then the shell should not be able
to run any programs (but built-in commands, such as path, should still work).</p> 

<h3>Redirection</h3> 

<p>Many times, a shell user prefers to send the output of his/her program to a
file rather than to the screen. Usually, a shell provides this nice feature
with the <code>">"</code> character. Formally this is named as redirection of standard
output. To make your shell users happy, your shell should also include this
feature, but with a slight twist (explained below).</p> 

<p>For example, if a user types <code>"ls -la /tmp > output"</code> , nothing
should be printed on the screen. Instead, the standard output of the
<code>ls</code> program should be rerouted to the <code>output.out</code> file. In
addition, the standard error output of the file should be rerouted to the file
<code>output.err</code> (the twist is that this is a little different than standard
redirection).</p> 

<p>If the <code>output.out</code> or <code>output.err</code> files already exists before
you run your program, you should simple overwrite them (after truncating). If
the output file is not specified (e.g. the user types <code>ls ></code> ), you
should print an error message and not run the program <code>ls</code> .</p> 

<p>Here are some redirections that should <b>not</b> work:
<pre><blockquote>
ls > out1 out2
ls > out1 out2 out3
ls > out1 > out2
</blockquote> </pre> </p> 

<p>Note: don't worry about redirection for built-in commands (e.g., we will
not test what happens when you type <code>path /bin > file</code> ).

<h3>Program Errors</h3> 

<p><b>The one and only error message.</b> You should print this one and only
error message whenever you encounter an error of any type:</p> 

<pre><blockquote>
char error_message[30] = "An error has occurred\n";
write(STDERR_FILENO, error_message, strlen(error_message));
</blockquote> </pre> 

<p>The error message should be printed to stderr (standard error). Also,
do not add whitespaces or tabs or extra error messages.</p> 

<p>There is a difference between errors that your shell catches and those that
the program catches. Your shell should catch all the syntax errors specified
in this project page. If the syntax of the command looks perfect, you simply
run the specified program. If there is any program-related errors
(e.g. invalid arguments to <code>ls</code> when you run it, for example), let the
program prints its specific error messages in any manner it desires
(e.g. could be stdout or stderr).</p> 

<h3>White Spaces</h3> 

<p>The <code>">"</code> operator will be separated by spaces. Valid input may
include the following:</p> 

<pre><blockquote>
whoosh> ls
whoosh> ls > a
whoosh> ls > a
</blockquote> </pre> 

<p>But not this (it is ok if this works, it just doesn't have to):</p> 

<pre><blockquote>
whoosh> ls>a
</blockquote> </pre> 


<h3>Defensive Programming and Error Messages</h3> 

<p>Defensive programming is required. Your program should check all
parameters, error-codes, etc. before it trusts them. In general, there should
be no circumstances in which your C program will core dump, hang indefinitely,
or prematurely terminate. Therefore, your program must respond to all input in
a reasonable manner; by "reasonable", we mean print the error message (as
specified in the next paragraph) and either continue processing or exit,
depending upon the situation.</p> 

<p><b>Since your code will be graded with automated testing, you
should print this <i>one and only error message</i> whenever you
encounter an error of any type:</b> </p> 

<pre><blockquote>
char error_message[30] = "An error has occurred\n";
write(STDERR_FILENO, error_message, strlen(error_message));
</blockquote> </pre> 

<p><b>For this project, the error message should be printed to <i>stderr.</i> 
Also, do not attempt to add whitespaces or tabs or extra error messages.</b> </p> 

<p>You should consider the following situations as errors; in each
case, your shell should print the error message to stderr and
<b>exit</b> gracefully:</p> 

<ul>
<li>An incorrect number of command line arguments to your shell
program.</li> 
</ul> 

<p>For the following situation, you should print the error message to
stderr and <b>continue</b> processing:</p> 

<ul>
<li>A command does not exist or cannot be executed.</li> 
<li>A very long command line (over 128 bytes).</li> 
</ul> 

<p>
Your shell should also be able to handle the following scenarios below, which
are <b>not errors.</b> </p> 

<ul>
<li>An empty command line.</li> 
<li>Multiple white spaces on a command line.</li> 
</ul> 

<p>All of these requirements will be tested extensively.</p> 

<h2>Hints</h2> 

<p>Writing your shell in a simple manner is a matter of finding the relevant
library routines and calling them properly. To simplify things for you in
this assignment, we will suggest a few library routines you may want to use to
make your coding easier. You are free to use these routines if you want or to
disregard our suggestions. To find information on these library routines, look
at the manual pages.</p> 

<h3>Basic Shell</h3> 

<p><b>Parsing:</b> For reading lines of input, once again check out <b>
fgets().</b> To open a file and get a handle with type <b>FILE *</b> , look into <b>
fopen().</b> Be sure to check the return code of these routines for errors! (If
you see an error, the routine <b>perror()</b> is useful for displaying the
problem. <i>But do not print the error message from perror() to the
screen. You should only print the one and only error message that we have
specified above</i> ). You may find the <b>strtok()</b> routine useful for parsing
the command line (i.e., for extracting the arguments within a command
separated by whitespaces).</p> 

<p><b>Executing Commands:</b> Look into <b>fork,</b> <b>execv,</b> and <b>
wait/waitpid.</b> See the man pages for these functions, and also read
book chapter <a href=http://www.ostep.org/cpu-api.pdf>here.</a> </p> 

<p>You will note that there are a variety of commands in the <code>exec</code> 
family; for this project, you must use <b>execv.</b> You should <b>not</b> use the
<b>system()</b> call to run a command. Remember that if <code>execv()</code> is
successful, it will not return; if it does return, there was an error (e.g.,
the command does not exist). The most challenging part is getting the
arguments correctly specified. The first argument specifies the program that
should be executed, with the full path specified; this is
straight-forward. The second argument, <code>char *argv[]</code> matches those
that the program sees in its function prototype:</p> 

<blockquote><pre>
int main(int argc, char *argv[]);
</pre> </blockquote> 

<p>
Note that this argument is an array of strings, or an array of
pointers to characters. For example, if you invoke a program with:</p> 

<blockquote><pre>
foo 205 535
</pre> </blockquote> 

<p>and assuming that you find <code>foo</code> in directory <code>/bin</code> , then
argv[0] = "/bin/foo", argv[1] = "205" and argv[2] = "535".</p> 

<p>Important: the list of arguments must be terminated with a NULL pointer; in
our example, this means argv[3] = NULL. We strongly recommend that you
carefully check that you are constructing this array correctly!</p> 

<h3>Built-in Commands</h3> 

<p>For the <code>exit</code> built-in command, you should simply call <code>exit().</code> 
The corresponding process will exit, and the parent (i.e. your shell) will be
notified.</p> 

<p>For managing the current working directory, you should use <b>getenv,</b> <b>
chdir,</b> and <b>getcwd.</b> The <code>getenv()</code> call is useful when you want to
go to your HOME directory. <b>You do not have to manage the PWD environment
variable.</b> getcwd() system call is useful to know the current working
directory; i.e. if a user types pwd, you simply call getcwd(). And finally,
chdir is useful for moving directories. Fore more information on these topics,
read the man pages or the Advanced Unix Programming book <b>Chapters 4 and
7.</b> </p> 

<h3>Redirection</h3> 

<p>Redirection is relatively easy to implement. For example, to redirect
standard output to a file, just use <b>close()</b> on stdout, and then <b>open()</b> 
on a file. More on this below.</p> 

<p>With a file descriptor, you can perform read and write to a file. Maybe in
your life so far, you have only used <b>fopen()</b> , <b>fread()</b> , and <b>
fwrite()</b> for reading and writing to a file. Unfortunately, these functions
work on <b>FILE*</b> , which is more of a C library support; the file descriptors
are hidden.</p> 

<p>To work on a file descriptor, you should use <b>open()</b> , <b>read()</b> , and <b>
write()</b> system calls. These functions perform their work by using file
descriptors. To understand more about file I/O and file descriptors you
should read the Advanced Unix Programming book <b>Section 3</b> (specifically,
3.2 to 3.5, 3.7, 3.8, and 3.12), or just read the man pages. Before reading
forward, at this point, you should become more familiar file descriptors.</p> 

<p>The idea of redirection is to make the stdout descriptor point to
your output file descriptor. First of all, let's understand the
STDOUT_FILENO file descriptor. When a command <code>"ls -la /tmp"</code> 
runs, the ls program prints its output to the screen. But obviously,
the ls program does not know what a screen is. All it knows is that
the screen is basically pointed by the STDOUT_FILENO file
descriptor. In other words, you could rewrite <code>printf("hi")</code> in
this way: <code>write(STDOUT_FILENO, "hi", 2)</code> .</p> 

<p>To check if a particular file exists in a directory, use the <code>stat()</code> 
system call. For example, when the user types <code>ls</code> , and path is set to
include both <code>/bin</code> and <code>/usr/bin</code> , try <code>stat("/bin/ls")</code> . If
that fails, try <code>stat("/usr/bin/ls")</code> . If that fails too, print the
<b>only error message.</b> <strike>error message <code>Command not found.</code> </strike> </p> 

<h3>Miscellaneous Hints</h3> 

<p>Remember to get the <b>basic functionality</b> of your shell working
before worrying about all of the error conditions and end cases. For
example, first get a single command running (probably first a command
with no arguments, such as "ls"). Then try adding more arguments.</p> 

<p><strike>Next, try working on multiple commands. Make sure that you are
correctly handling all of the cases where there is miscellaneous white
space around commands or missing commands.</strike> Next, add built-in
commands. Finally, add redirection support.</p> 

<p>We strongly recommend that you check the return codes of all system
calls from the very beginning of your work. This will often catch
errors in how you are invoking these new system calls. And, it's just good
programming sense.</p> 

<p>Beat up your own code! You are the best (and in this case, the
only) tester of this code. Throw lots of junk at it and make sure the
shell behaves well. Good code comes through testing -- you must run
all sorts of different tests to make sure things work as
desired. Don't be gentle -- other users certainly won't be. Break it
now so we don't have to break it later.</p> 

<p>Keep versions of your code. More advanced programmers will use a
source control system such as git. Minimally, when you get a piece of
functionality working, make a copy of your .c file (perhaps a subdirectory
with a version number, such as v1, v2, etc.). By keeping older, working
versions around, you can comfortably work on adding new functionality, safe in
the knowledge you can always go back to an older, working version if need
be.</p> 

<h2>Handin</h2> 


<p>To ensure that we compile your C correctly for the demo, you will
need to create a simple <b>makefile;</b> this way our scripts can just
run <code>make</code> to compile your code with the right libraries and
flags. If you don't know how to write a makefile, you might want to
look at the man pages for <code>make</code> or better yet, read the tutorial.</p> 

<p>
The name of your final executable should be <code>whoosh</code> , i.e. your
C program must be invoked exactly as follows:</p> 

<pre><blockquote>
emperor1% ./whoosh
</blockquote> </pre> 

<p>Copy all of your .c source files into the appropriate subdirectory. Do <b>
not</b> submit any .o files. Make sure that your code runs correctly on the
linux machines in the galapagos (and similar) labs.</p> 

<h2>Contest</h2> 

<p>There is another contest for the shell. In your previous contest, you had
to write the <b>fastest</b> sort. Now, you have to write the <b>shortest,
completely working, readable</b> shell. What do we mean by <b>readable?</b> Well,
the code should not be short because you have removed all white space and made
it so we cannot understand what the code does. Rather, it should be short
because you have removed all unnecessary redundancy. We will count the code by
the number of (non-whitespace) lines; we will then examine the best few
entrants to see whose code is actually readable but compact, and then choose a
winner. Winner, as usual, gets a <b>FAMOUS 537 T-SHIRT.</b> Good luck!</p> 


<h2>Grading</h2> 

<p>We will run your program on a suite of test cases, some of which will
exercise your programs ability to correctly execute commands and some of which
will test your programs ability to catch error conditions.</p> 


</p> </p> </td> </tr> </table> </center> 









