<body text=black bgcolor=white link=#00aacc vlink=#00aacc alink=orange>

<center><table><tr><td width=1000pt>

<center>
<font color=#00aacc>
<h1>Project 1: Warm-up Project</h1> 
</font> 
</center> 

<h2>Important Dates</h2> 

<p><b>Due:</b> <strike>Monday, 2/01</strike> <b>Tuesday 2/02</b> by 10pm.</p> 

<h2>Questions?</h2> 

<p>Send questions to <b>537-help@cs.wisc.edu</b> (or, of course, visit us in
person during office hours!). If the question is about your code, copy all of
of your code into your handin directory (details below) and include your login
in your email (you are free to modify the contents of your handin directory
prior to the due date). Also include all other relevant information, such as
cutting and pasting what you typed and the results from the screen. In
general, the more information you give, the more we can help.</p> 

<h2>Overview</h2> 

<p>There are two parts to this project:
<ul>
<li><a href=p1a.html>Sorting:</a> to be done on the lab machines (macaroni-*,
adelie-*, galapagos-*, king-*), so you can learn more about programming in C
on a typical UNIX-based platform (Linux)</li> 
<li><a href=p1b.html>Kernel Intro:</a> to be done in our xv6 hacking
environment, so you can learn more about what actually goes on in a real
kernel.</li> 
</ul> 
</p> 

<p>Click on the above links to learn more about what you should do. READ EACH CAREFULLY!</p> 

<h2>Notes</h2> 

<p><b>Before beginning:</b> Read
<a href=http://pages.cs.wisc.edu/~remzi/OSTEP/lab-tutorial.pdf>this tutorial.</a> 
It has some useful tips for programming in the C environment.</p> 

<p><b>This project should be done alone.</b> Copying code (from others) is
considered cheating. Read <a href=../dontcheat.html>this</a> for more info on
what is OK and what is not. Please help us all have a good semester by not
doing this.</p> 

<h2>Handing It In</h2> 

<p>For the C/Linux part of this project (sorting), you should turn in one
file, called <code>fastsort.c</code> . We will compile it in the following way: <blockquote>
<code>shell% gcc -O -Wall -o fastsort fastsort.c</code> </blockquote> so make sure it compiles in
such a manner. You should copy this file into your handin directory into the
subdirectory called <code>linux</code> .</p> 

<p>The handin directory is <code>~cs537-1/handin/login/p1</code> where <code>login</code> is
your login. For example, Remzi's login is <code>remzi</code> , and thus he would copy
his beautiful code into <code>~cs537-1/handin/remzi/p1</code> . Copying of
these files is accomplished with the <code>cp</code> program, as follows:
<blockquote><pre>
shell% cp fastsort.c ~cs537-1/handin/remzi/p1/linux/
</pre> </blockquote> </p> 

<p>For the xv6 part of the project, copy all of your source files (but not .o
files, please, or binaries!) into the <code>xv6/</code> subdirectory of your p1
directory. A simple way to do this is to copy everything into the destination
directory, then type <code>make</code> to make sure it builds, and then
type <code>make clean</code> to remove unneeded files.</p> 

<p>
<blockquote><pre>
shell% cp -r . ~cs537-1/handin/remzi/p1/xv6
shell% cd ~cs537-1/handin/remzi/p1/xv6
shell% make
shell% make clean
</pre> </blockquote> 

</p> 

<p>Finally, into your p1 directory, please include a README file. In there,
describe what you did a little bit. There is no particular requirement for the
length of the README; just get in the habit of writing a little bit about what
you did, so that another human could understand it.</p> 


</td> </tr> </table> </center> </body>
