/**
 * fdcheck.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char **argv) {
  if (argc < 2) {
    fprintf(stderr, "Usage: %s fd\n", argv[0]);
    exit(1);
  }
  int fd = atoi(argv[1]);
  char* str = "Hello World\n";
  size_t len = strlen(str);
  write(fd, str, len);
  return 0;
}
