#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"
#include "fs.h"

void
test_failed()
{
	printf(1, "TEST FAILED\n");
	exit();
}

void
test_passed()
{
 printf(1, "TEST PASSED\n");
 exit();
}

#define NBLOCKS (NDIRECT+1)
#define SIZE NBLOCKS*4

int
main(int argc, char *argv[])
{
  int fd, i,j;
  char buf[SIZE];
  char buf2[SIZE];
  char tmp;
  struct stat st;
  
  for(i = 0; i < SIZE; i++){
    buf[i] = (char)(i+(int)'0');
  }
  memset(buf2, 0, SIZE);
  
  //open, write 1 byte to the end, close
  for(i = 0; i < SIZE+5; i++){
    if((fd = open("test_file.txt", O_CREATE | O_RDWR | O_SMALLFILE)) < 0){
      printf(1, "Failed to create the small file\n");
      test_failed();
    }
    j=0;
    while(read(fd, &tmp, 1) == 1) {
      j++;
    } //go to end of file
    if(write(fd, &buf[i], 1) != 1){
      printf(1,"\t\tBREAK!!!");
      break;
    }
    close(fd);
    printf(1,"in whlie for %d times\n",j);
  }
  printf(1,"for loop executed %d times\n",i);

  //read
  if((fd = open("test_file.txt", O_CREATE | O_RDWR | O_SMALLFILE)) < 0){
    printf(1, "Failed to open the small file\n");
    test_failed();
  }
  
  if(fstat(fd, &st) < 0){
    printf(1, "Failed to get stat on the small file\n");
    test_failed();
  }
  
    printf(1, "Size is %d\n",st.size);
  if(st.size != SIZE){
    printf(1, "Invalid file size.\n");
    test_failed();
  }
  
  if(read(fd, buf2, SIZE) != SIZE){
    printf(1, "Read failed!\n");
    test_failed();
  }
  close(fd);
  
  printf(1,"Expect %s\n",buf);
  printf(1,"Got    %s\n",buf2);
  for(i = 0; i < SIZE; i++){
    if(buf[i] != buf2[i]){
      printf(1, "Data mismatch.\n");
      test_failed();
    }
  }
  
  test_passed();
	exit();
}
