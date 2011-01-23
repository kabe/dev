/**
 * @file tcp_server.c
 * @brief infinitely receives a message.
 */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/socket.h>
#include "common.h"
#include "tcp.h"

#define PORT 15000
#define OUTFILE "/dev/null"

int main(int argc, char **argv) {
  int e;
  int csock;
  int port = PORT;
  char* peer;
  int recv_bytes;
  int fd;
  char* buf;
  char* infile = NULL;
  size_t bufsize;
  /* arg */
  if (argc != 4) {
    fprintf(stderr, "Usage: %s peer bufsize infile\n", argv[0]);
    exit(1);
  }
  peer = argv[1];
  bufsize = atoi(argv[2]);
  infile = argv[3];
  DUMP1("Input file: %s", infile);
  /* prepare buffer */
  buf = (char*) safe_malloc(bufsize);
  /* prepare infile */
  fd = open(infile, O_RDONLY);
  if (fd < 0) {
    e = errno;
    DUMP1("Cannot open outfile \"%s\" (%s)", infile, strerror(e));
    exit(8);
  }
  /* client */
  csock = tcp_connect(peer, port);
  if (csock < 0) {
    exit(8);
  }
  recv_bytes = read_and_send(fd, csock, bufsize);
  DUMP1("Sending completed (%zd bytes)", recv_bytes);
  return 0;
}
