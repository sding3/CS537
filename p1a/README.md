
<html>

<head>
<title>Project 1a: Sorting</title> 
</head> 

<body text=black bgcolor=white link=#00aacc vlink=#00aacc alink=orange>

<center><table><tr><td width=1200pt>

<center>
<font color=#00aacc>
<h1>Project 1a: Text Sorting</h1> 
</font> 
</center> 

<p>You will write a simple sorting program. This program should be invoked as
follows:
<blockquote><pre>
shell% ./fastsort [ -3] file
</pre> </blockquote> </p> 

<p>The above line means the users typed in the name of the sorting program
<code>./fastsort</code> and gave it one or two inputs. If just one input is given,
the input file <code>file</code> should be sorted and the sorted output printed to
the screen; it is assumed that the file is a text file (full of ASCII
characters) and, when no other arguments are given, the sorting should be over
the first word in each line.</p> 

<p>If the optional argument is included ( <code>-3</code> in the example), the
program should sort the text input file using the specified word as the key to
sort upon (with <code>-3</code> , the program should find the 3rd word in each line
and sort the lines based upon that).</p> 

<h2>Examples</h2> 

<p>Let's say you have the following file:
<pre>
this line is first
but this line is second
finally there is this line
</pre> </p> 

<p>If you run your fastsort and give it this file as input, it should print:
<pre>
but this line is second
finally there is this line
this line is first
</pre> 

because <code>but</code> is alphabetically before <code>finally</code> is before <code>
this</code> .</p> 

<p>If, however, you pass in a flag to sort a different key, you'll get a
different output. For example, if you call <code>fastsort -2</code> on this file,
you should get:

<pre>
this line is first
finally there is this line
but this line is second
</pre> 

because <code>line</code> comes before <code>there</code> comes before <code>this</code> .
Yes, we are assuming <code>-2</code> means the second word in each line (like most
people would, except computer scientists who always want to start at 0).</p> 


<h2>Hints</h2> 

<p>In your sorting program, you should just use <code>fopen()</code> to open the
input file, <code>fgets()</code> to read each line of the file, and <code>fclose()</code> 
when you are done with the input file.</p> 

<p>If you want to figure out how big in the input file is before reading it
in, use the <code>stat()</code> or <code>fstat()</code> calls.</p> 

<p>To compare strings, use the <code>strcmp()</code> library call.</p> 

<p>To sort the data, use any sort that you'd like to use. An easy way to go is
to use the library routine <code>qsort()</code> .</p> 

<p>To chop lines into words, you could use <code>strtok()</code> . Be careful,
though; it is destructive and will change the contents of the lines. Thus, if
you use it, make sure to make a copy of the line to output.</p> 

<p>The routine <code>atoi()</code> (or better yet, <code>strtol()</code> ) can be used to
transform a string into an integer.</p> 

<p>To exit, call <code>exit()</code> with a single argument. This argument to exit()
is then available to the user to see if the program returned an error (i.e.,
return 1 by calling <code>exit(1)</code> ) or exited cleanly (i.e., returned 0 by
calling <code>exit(0)</code> ). You can also just return from the <code>main()</code> 
function and pass the return code that way where appropriate.</p> 

<p>The routine <code>malloc()</code> is useful for memory allocation. Make sure to
exit cleanly if malloc fails!</p> 

<p>If you don't know how to use these functions, use the man pages. For
example, typing <code>man qsort</code> at the command line will give you a lot of
information on how to use the library sorting routine.</p> 

<h2>Assumptions and Errors</h2> 

<p>
<blockquote><b>The return code upon success is zero.</b> When the program runs normally and
no errors are encountered, you should return an error code of 0.</blockquote> 
<blockquote><b>Only space characters (i.e., what you get when you hit spacebar) will
be used to separate words in the input.</b> Thus, you don't have to worry about
tabs or other whitespace. However, your program should correctly handle the
case where there are two or more spaces between words, i.e., it should treat
that as one big separator between the words.</blockquote> 
<blockquote><b>Max line length will be 128.</b> If you get a line longer than this
(detected by the lack of a newline character in the last position), please
print <code>Line too long</code> to standard error and exit with return code 1.</blockquote> 
<blockquote><b>You should check the arguments of fastsort carefully.</b> If more than two
arguments are passed, or two are passed but the second does not fit the format
of a dash followed by a number, you should EXACTLY print (to standard error):
<code>Error: Bad command line parameters</code> and exit with return code 1.</blockquote> 
<blockquote><b>Key does not exist on one line of input file:</b> If the specified key
does not exist on a particular line of the input file, you should just use the
last word of that line as the key. For example, if the user wants to sort on
the 4th word ( <code>-4</code> ), and the sort encounters a line like this ( <code>sample
line</code> ), the sort should use the word <code>line</code> to sort that line.</blockquote> 
<blockquote><b>Empty line:</b> You should use an empty string to sort any empty lines
(i.e., lines that are just a newline or spaces and a newline character).</blockquote> 
<blockquote><b>File length:</b> May be pretty long! However, no need to implement a
fancy two-pass sort or anything like that; the data set will fit into memory
and you shouldn't have to do anything special to handle this. However, if
malloc() does fail, please print <code>malloc failed</code> to standard error and
exit with code 1.</blockquote> 
<blockquote><b>Invalid files:</b> If the user specifies an input file that you cannot
open (for whatever reason), the sort should EXACTLY print (to standard error):
<code>Error: Cannot open file foo</code> with no extra spaces (if the file was
named <code>foo</code> ) and then exit with return code 1.</blockquote> <p><b>Important:</b> On any
error code, you should print the error to the screen using <code>fprintf()</code> ,
and send the error message to <code>stderr</code> (standard error) and not <code>
stdout</code> (standard output). This is accomplished in your C code as follows: <blockquote>
<code>fprintf(stderr, &ldquo;whatever the error message is\n&rdquo;);</code> </blockquote> </p> </p> 

<h2>History and a Contest</h2> 

<p>This sorting assignment is reminiscent yearly competition to make the
fastest disk-to-disk sort in the world. See <a href=http://sortbenchmark.org/>
the sort home page</a> for details. If you look closely, you will see that your
professor was once -- yes, wait for it -- the fastest sorter in the world.</p> 

<p>To continue in this tradition, we will also be holding a <b>sorting
competition.</b> Whoever turns in the fastest sorting program on a few different
inputs will win a fancy 537 T-shirt. Read more about sorting, including
perhaps the <a href=http://pages.cs.wisc.edu/~remzi/Postscript/nowSort.pdf>
NOW-Sort paper</a> , for some hints on how to make a sort run really fast. Or just
use your common sense! Hint: you'll have to think a bit about hardware
caches.</p> 

<p><b>Restriction: No threads.</b> You cannot implement a multi-threaded sort for
this assignment or competition. Just make the fastest single-threaded sort
that you can!</p> 

<h2>General Advice</h2> 

<p><b>Start small, and get things working incrementally.</b> For example, first
get a program that simply reads in the input file, one line at a time, and
prints out what it reads in. Then, slowly add features and test them as you
go.</p> 

<p><b>Testing is critical.</b> One great programmer I once knew said you have to
write 5-10 lines of test code for every line of code you produce; testing your
code to make sure it works is crucial. Write tests to see if your code handles
all the cases you think it should. Be as comprehensive as you can be. Of
course, when grading your projects, we will be. Thus, it is better if you find
your bugs first, before we do.</p> 

<p><b>Keep old versions around.</b> Keep copies of older versions of your program
around, as you may introduce bugs and not be able to easily undo them. A
simple way to do this is to keep copies around, by explicitly making copies of
the file at various points during development. For example, let's say you get
a simple version of <code>fastsort.c</code> working (say, that just reads in the
file); type <code>cp fastsort.c fastsort.v1.c</code> to make a copy into the file <code>
fastsort.v1.c</code> . More sophisticated developers use version control systems like
git or mercurial, but we'll not get into that here (yet).</p> 

<p><b>Keep your source code in a private directory.</b> An easy way to do this is
to log into your account and first change directories into <code>private/</code> and
then make a directory therein (say <code>p1</code> , by typing <code>mkdir p1</code> after
you've typed <code>cd private/</code> to change into the private
directory). However, you can always check who can read the contents of your
AFS directory by using the <code>fs</code> command. For example, by typing in <code>
fs listacl .</code> you will see who can access files in your current directory. If
you see that <code>system:anyuser</code> can read (r) files, your directory contents
are readable by anybody. To fix this, you would type <code>fs setacl
. system:anyuser &ldquo;&rdquo;</code> in the directory you wish to make private. The
dot &ldquo;.&rdquo; referred to in both of these examples is just shorthand for
the current working directory.</p> 

</td> </tr> </table> </center> </body> </html> 





