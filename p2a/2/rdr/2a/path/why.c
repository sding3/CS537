#include <stdio.h>
#include <unistd.h>
#include <string.h>

char* error_msg = "Too many arguments\n";
char* std_msg = "There is no why\n";

int main(int argc, char* argv[]) {
  if (argc > 1) {
    write(STDOUT_FILENO, error_msg, strlen(error_msg));
  } else {
    write(STDOUT_FILENO, std_msg, strlen(std_msg));
  }
  return 0;
}