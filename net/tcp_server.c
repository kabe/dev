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
  int servsock;
  int asock;
  int port = PORT;
  int recv_counter;
  int fd;
  ssize_t ret;
  char* buf;
  struct sockaddr_in sa;
  socklen_t socklen = sizeof(struct sockaddr_in);
  char* outfile = OUTFILE;
  size_t bufsize;
  /* arg */
  if (argc < 2 || argc > 3) {
    fprintf(stderr, "Usage: %s bufsize [outfile]\n", argv[0]);
    exit(1);
  }
  bufsize = atoi(argv[1]);
  if (argc == 3) {
    outfile = argv[2];
  }
  DUMP1("Dump over %s", outfile);
  /* prepare buffer */
  buf = (char*) safe_malloc(bufsize);
  /* prepare outfile */
  fd = open(outfile, O_RDONLY);
  if (fd < 0) {
    e = errno;
    DUMP1("Cannot open outfile \"%s\" (%s)", outfile, strerror(e));
    exit(8);
  }
  /* server */
  servsock = tcp_listen_any(port);
  if (servsock < 0) {
    e = errno;
    DUMP1("Accept on port %d Error (%s)", port, strerror(e));
    exit(3);
  }
  DUMP1("Listens on port %d with socket %d", port, servsock);
  asock = accept(servsock, (struct sockaddr*)&sa, (socklen_t*)&socklen);
  DUMP1("Accepted with sock %d", asock);
  if (asock < 0) {
    e = errno;
    DUMP1("accept(2) error (%s)", strerror(e));
    exit(7);
  }
  recv_counter = 0;
  while(1) {
    ret = looprecv(asock, buf, bufsize, 0);
    if (ret == 0) break;
    if (recv_counter % 100 == 0) {
      DUMP1("Recv (%d)", recv_counter++);
    }
    //safe_write(fd, buf, bufsize);
  }
  return 0;
}
