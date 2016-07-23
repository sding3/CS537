#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

int
main(int argc, char *argv[]){
    int fd = open("foo", O_CREATE | O_SMALLFILE | O_RDWR);
    //int fd = open("foo", O_CREATE | O_RDWR );
    printf(1,"open returned %d\n",fd);
    char *buf = "abcdefg\n";
    int rc = write(fd,buf,9);
    printf(1,"write return %d\n",rc);
    close(fd);
  
    printf(1,"size of uint is %d\n", (int) sizeof(uint));
    printf(1,"size of char is %d\n", (int) sizeof(char));
    exit();
}
