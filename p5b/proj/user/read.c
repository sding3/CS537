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

#define MAX 25

int
main(int argc, char *argv[])
{
  int fd;
  char buf[MAX];
  char buf2[MAX];
  int n;
  int i;
  
  for(i = 0; i < MAX; i++){
    buf[i] = (char)(i+(int)'0');
  }
  
  if((fd = open("test_file.txt", O_CREATE | O_RDWR | O_SMALLFILE)) < 0){
    printf(1, "Failed to create the small file\n");
    test_failed();
    exit();
  }
  
  if((n = write(fd, buf, MAX)) != MAX){
    printf(1, "Write failed!\n");
    test_failed();
  }
  printf(1, "bytes written = %d\n", n);
  close(fd);
  
  if((fd = open("test_file.txt", O_CREATE | O_RDWR | O_SMALLFILE )) < 0){
    printf(1, "Failed to open the small file\n");
    test_failed();
  }
  
  if((n = read(fd, buf2, MAX*2)) != MAX){
    printf(1, "Read failed!\n");
    test_failed();
  }
  printf(1, "bytes read = %d\n", n);
  close(fd);

  for(i = 0; i < MAX; i++){
    if(buf[i] != buf2[i]){
      printf(1, "Data mismatch.\n");
      test_failed();
    }
  }
  
  test_passed();
	exit();
}
