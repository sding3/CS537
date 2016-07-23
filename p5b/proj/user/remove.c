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
  int fd, pid, i;
  
  char *filename = "test_file.txt";
  char buf[SIZE];
  
  uint *sector = (uint *)&buf;
  for(i = 0; i < NBLOCKS; i++, sector++){
    *sector = i;
  }
  
  if((fd = open(filename, O_CREATE | O_SMALLFILE | O_RDWR)) < 0){
    printf(1, "Failed to create a small file\n");
    test_failed();
  }
  
  if(write(fd, buf, SIZE) != SIZE){
    printf(1, "Failed to write to small file\n");
    test_failed();
  }
  close(fd);
  
  pid = fork();
  if(pid < 0){
    printf(1, "Fork failed\n");
    test_failed();
  }
  else if(pid == 0) {
    char *args[3] = {"rm", filename, 0};
    exec(args[0], args);
    printf(1, "exec failed!\n");
    test_failed();
  }
  else {
    wait();
  }
  
  test_passed();
  exit();
}