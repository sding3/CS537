/* Check if the program gives the correct output for a simple graph */
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include "crawler.h"
#include <pthread.h>
#include <semaphore.h>
#include "helper.h"

pthread_mutex_t buffer_mutex;
int fill = 0;
char buffer[100][50];
char *fetch(char *link) {
  int fd = open(link, O_RDONLY);
  if (fd < 0) {
        fprintf(stderr, "failed to open file: %s", link);
    	return NULL;
  }
  int size = lseek(fd, 0, SEEK_END);
  assert(size >= 0);
  char *buf = Malloc(size+1);
  buf[size] = '\0';
  assert(buf);
  lseek(fd, 0, SEEK_SET);
  char *pos = buf;
  while(pos < buf+size) {
    int rv = read(fd, pos, buf+size-pos);
    assert(rv > 0);
    pos += rv;
  }
  close(fd);
  return buf;
}

void edge(char *from, char *to) {
	return;
}

int main(int argc, char *argv[]) {

  pthread_mutex_init(&buffer_mutex, NULL);

  int rc = crawl("/u/c/s/cs537-1/ta/tests/4a/tests/files/num_threads/pagea", 5, 4, 15, fetch, edge);
  assert(rc == 0);
  return 0;
}
