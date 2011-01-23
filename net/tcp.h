#ifndef _TCP_H_
#define _TCP_H_

#include <netinet/in.h>
#include <netinet/tcp.h>
#include <netinet/udp.h>

int tcp_connect(char* peer, int port);
int tcp_end(int sock);
int tcp_listen_any(int port);

int read_and_send(int in_fd, int out_sock, ssize_t bufsize);
int recv_and_dump(int sock);

ssize_t loopsend(int sock, void* buf, size_t size, int flags);
ssize_t looprecv(int sock, void* buf, size_t size, int flags);

#define BACKLOG 10 /* # of connections */
#define USE_GETADDRINFO
#define USE_REUSEADDR 1
#define USE_SELECT
#define BUFSIZE 1024

#endif /* _TCP_H_ */
