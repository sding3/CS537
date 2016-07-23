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

#define SMALLWRITE  10
#define BIGWRITE    100

int
main(int argc, char *argv[])
{
  int fd, i;
  char buf[SMALLWRITE + BIGWRITE];
  char buf2[SMALLWRITE + BIGWRITE];
  struct stat st;
  
  for(i = 0; i < SIZE; i++){
    buf[i] = (char)(i+(int)'0');
  }
  memset(buf2, 0, SIZE);
  
  //writes
  if((fd = open("test_file.txt", O_CREATE | O_SMALLFILE | O_RDWR)) < 0){
    printf(1, "Failed to create the small file\n");
    test_failed();
  }
  
  if(write(fd, buf, SMALLWRITE) != SMALLWRITE){
    printf(1, "Failed to do a small write\n");
    test_failed();
  }
  
  if(write(fd, buf+SMALLWRITE, BIGWRITE) != SIZE-SMALLWRITE){
    printf(1, "Failed to do a truncated big write\n");
    test_failed();
  }
  close(fd);
  
  //read
  if((fd = open("test_file.txt", O_CREATE | O_SMALLFILE | O_RDWR)) < 0){
    printf(1, "Failed to open the small file\n");
    test_failed();
  }
  
  if(fstat(fd, &st) < 0){
    printf(1, "Failed to get stat on the small file\n");
    test_failed();
  }
  
  if(st.size != SIZE){
    printf(1, "Invalid file size.\n");
    test_failed();
  }
  
  if(read(fd, buf2, SIZE) != SIZE){
    printf(1, "Read failed!\n");
    test_failed();
  }
  close(fd);
  
  for(i = 0; i < SIZE; i++){
    if(buf[i] != buf2[i]){
      printf(1, "Data mismatch.\n");
      test_failed();
    }
  }
  
  test_passed();
	exit();
}
