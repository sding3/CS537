
<html>

<head>
<title>Project 5a: File System Checker</title> 
</head> 

<body text=black bgcolor=white link=#00aacc vlink=#00aacc alink=orange>

<center><table><tr><td width=1200pt>

<center>
<font color=#00aacc>
<h1>Project 5a: File System Checker</h1> 
</font> 
</center> 

<h2>Notes</h2> 

<p>None.</p> 

<h2>Background</h2> 

<p>In this assignment, you will be developing a working <b>file system
checker.</b> A checker reads in a file system image and makes sure that it is
consistent. When it isn't, the checker takes steps to fix the problems it
sees; however, we won't be doing any fixes this time to keep your life a
little simpler.</p> 

<h2>A Basic Checker</h2> 

<p>For this project, we will use the xv6 file system image as the basic image
that we will be reading and checking. The file <b>include/fs.h</b> includes the
basic structures you need to understand, including the superblock, on disk
inode format (struct dinode), and directory entry format (struct dirent).
The tool <b>tools/mkfs.c</b> will also be useful to look at, in order to see how
an empty file-system image is created.</p> 

<p>Much of this project will be puzzling out the exact on-disk format xv6 uses
for its simple file system, and then writing checks to see if various parts of
that structure are consistent. Thus, reading through mkfs and the file system
code itself will help you understand how xv6 uses the bits in the image to
record persistent information.</p> 

<p>Your checker should read through the file system image and determine the
consistency of a number of things, including the following. When one of these
does not hold, print the error message (also shown below) and exit
immediately.</p> 

<p>
<ul>
<li>Each inode is either unallocated or one of the valid types (T_FILE, T_DIR,
T_DEV). ERROR: <b>bad inode.</b> </li> 
<li>For in-use inodes, each address that is used by inode is valid (points to a
valid datablock address within the image). Note: must check indirect blocks
too, when they are in use. ERROR: <b>bad address in inode.</b> </li> 
<li>Root directory exists, and it is inode number 1. ERROR MESSAGE: <b>root
directory does not exist.</b> </li> 
<li>Each directory contains <b>.</b> and <b>..</b> entries. ERROR: <b>directory not properly
formatted.</b> </li> 
<li>Each <b>..</b> entry in directory refers to the proper parent inode, and
parent inode points back to it. ERROR: <b>parent directory mismatch.</b> </li> 
<li>For in-use inodes, each address in use is also marked in use in the
bitmap. ERROR: <b>address used by inode but marked free in bitmap.</b> </li> 
<li>For blocks marked in-use in bitmap, actually is in-use in an inode or
indirect block somewhere. ERROR: <b>bitmap marks block in use but it is not in
use.</b> </li> 
<li>For in-use inodes, any address in use is only used once. ERROR: <b>address
used more than once.</b> </li> 
<li>For inodes marked used in inode table, must be referred to in at least one
directory. ERROR: <b>inode marked use but not found in a directory.</b> </li> 
<li>For inode numbers referred to in a valid directory, actually marked in use
in inode table. ERROR: <b>inode referred to in directory but marked free.</b> </li> 
<li>Reference counts (number of links) for regular files match the number of
times file is referred to in directories (i.e., hard links work
correctly). ERROR: <b>bad reference count for file.</b> </li> 
<li>No extra links allowed for directories (each directory only appears in one
other directory). ERROR: <b>directory appears more than once in file system.</b> </li> 
</ul> 
</p> 

<h2>Other Specifications</h2> 

<p>Your server program must be invoked exactly as follows:

<pre>
prompt> fscheck file_system_image
</pre> </p> 

<p>The image file is a file that contains the file system image.
If the file system image does not exist, you should print the error <b>
image not found.</b> to stderr and exit with the error code of 1. If the checker
detects one of the errors listed above, it should print the proper error to
stderr and exit with error code 1. Otherwise, the checker should exit with
return code of 0.</p> 

<h2>Hints</h2> 

<p>It may be worth looking into using mmap() for the project.</p> 

<p>Make sure to look at <b>fs.img</b> , which is a file system image created when
you make xv6 by the tool <b>mkfs</b> (found in the tools/ directory of xv6). The
output of this tool is the file <b>fs.img</b> and it is a consistent file-system
image. The tests, of course, will put inconsistencies into this image, but
your tool should work over a consistent image as well. Study mkfs and its
output to begin to make progress on this project.</p> 



<h2>Contest</h2> 

<p>We will create a few very large images and the competition will be for who
can build the fastest checker. May the best team win!</p> 


</td> </tr> </table> </center> </body> </html> 






