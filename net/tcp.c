#include "common.h"
#include "tcp.h"

#include <sys/socket.h>
#include <netdb.h>
#include <errno.h>
#include <arpa/inet.h>
#include <fcntl.h>

int tcp_connect(char* peer, int port) {
  /* IPv4 socket */
  int sock = socket(PF_INET, SOCK_STREAM, 0);
  int ret;
  struct sockaddr_in addr;
#ifdef USE_GETADDRINFO
  /* Name resolution */
  struct addrinfo hints;
  struct addrinfo* result;
  struct in_addr addrs;
  memset(&hints, 0, sizeof(struct addrinfo));
  hints.ai_family = AF_INET;    /* Allow IPv4 or IPv6 */
  hints.ai_socktype = SOCK_STREAM;
  ret = getaddrinfo(peer, NULL, &hints, &result);
  if (ret < 0) {
    DUMP0("Address resolution failed: %s\n", gai_strerror(ret));
    return -1;
  }
  memcpy(&addrs, &((struct sockaddr_in*)(result->ai_addr))->sin_addr, sizeof(addrs));
#ifdef DEBUG
  printf("ip address : %s\n", inet_ntoa(addrs));
#endif /* DEBUG */
  memcpy(&addr, result->ai_addr, sizeof(addr));
#else /* USE_GETADDRINFO */
  /* Address expression convert */
  addr.sin_family = AF_INET;
  ret = inet_aton(peer, &addr.sin_addr);
  if (ret == 0) { /* address invalid */
    DUMP0("Address %s invalid\n", peer);
    return -1;
  }
#endif /* USE_GETADDRINFO */
  addr.sin_port = htons(port);
  /* connect */
  ret = connect(sock,
                (struct sockaddr*)&addr,
                sizeof(addr));
  if (ret != 0) {
    int e = errno;
    DUMP0("Connect failed: %s(%d)\n", strerror(e), e);
    return -1;
  }
  return sock;
}

int tcp_listen_any(int port) {
  /**
   * Create socket, bind on port and listen there.
   * Finally returns socket integer if succeeds,
   * otherwise negative value will be returned
   */
  int ret;
  /* IPv4 Socket */
  int sock = socket(PF_INET, SOCK_STREAM, 0);
  struct sockaddr_in addr;
  memset(&addr, 0, sizeof(struct sockaddr_in));
  addr.sin_family = AF_INET;
  addr.sin_port = htons(port);
  addr.sin_addr.s_addr = INADDR_ANY;
#if USE_REUSEADDR
  int reuseaddr_flag = 1;
  setsockopt(sock, SOL_SOCKET, SO_REUSEADDR,
             &reuseaddr_flag, sizeof(reuseaddr_flag));
#endif
  ret = bind(sock, (struct sockaddr*)&addr, sizeof(struct sockaddr_in));
  if (ret == -1) {
    int e = errno;
    DUMP0("Bind failed: %s(%d)\n", strerror(e), e);
    return -1;
  }
  ret = listen(sock, BACKLOG);
  if (ret == -1) {
    int e = errno;
    DUMP0("Listen failed: %s(%d)\n", strerror(e), e);
    return -1;
  }
  return sock;
}

int tcp_end(int sock) {
  int ret;
  ret = shutdown(sock, SHUT_RDWR);
  if (ret == -1) {
    int e = errno;
    DUMP0("Shutdown failed: %s(%d)\n", strerror(e), e);
    return -1;
  }
  close(sock);
  if (ret == -1) {
    int e = errno;
    DUMP0("Close failed: %s(%d)\n", strerror(e), e);
    return -1;
  }
  return 0;
}

int read_and_send(int in_fd, int out_sock, ssize_t bufsize) {
  unsigned char buf[bufsize];
  ssize_t len;
  ssize_t read_bytes = 0;
  int ret;
  while (1) {
    len = read(in_fd, buf, bufsize);
    if (len == 0) {
      break;
    }
    if (len == -1) {
      int e = errno;
      DUMP0("Read error: %s(%d)", strerror(e), e);
      return -1;
    }
    ret = write_b(out_sock, buf, len);
    read_bytes += len;
  }
  return read_bytes;
}

int recv_and_dump(int sock) {
  ssize_t ret;
  ssize_t recv_bytes = 0;
  ssize_t buf_len = BUFSIZE;
  char buf[BUFSIZE];
#ifdef USE_SELECT
  fd_set rfds;
  FD_ZERO(&rfds);
  FD_SET(sock, &rfds);
  int flags = fcntl(sock, F_GETFL, 0);
  fcntl(sock, F_SETFL, O_NONBLOCK | flags);
#endif
  while (1) {
    ret = recv(sock, buf, buf_len, 0);
    if (ret == -1) {
      int e = errno;
      if (e == EAGAIN) {
#ifdef USE_SELECT
        ret = select(sock + 1, &rfds, NULL, NULL, NULL);
        if (ret == -1) {
          e = errno;
          DUMP0("Select failed: %s (%d)", strerror(e), e);
          return -1;
        }
#endif
        continue;
      } else {
        DUMP0("Recv error: %s(%d)", strerror(e), e);
        return -1;
      }
    }
    if (ret == 0) break;
    fwrite(buf, 1, ret, stdout);
    recv_bytes += ret;
  }
  return recv_bytes;
}

ssize_t loopsend(int sock, void* buf, size_t size, int flags) {
  size_t k = 0;
  ssize_t ret;
  int e;
  while (k < size) {
    ret = send(sock, buf + k, size - k, flags);
    if (ret < 0) {
      e = errno;
      DUMP0("Send error (%s)", strerror(e));
      return ret;
    } else if (ret == 0) {
      DUMP0("send(2) returns 0");
    }
    k += ret;
  }
  return size;
}

ssize_t looprecv(int sock, void* buf, size_t size, int flags) {
  size_t k = 0;
  ssize_t ret;
  int e;
  while (k < size) {
    ret = recv(sock, buf + k, size - k, flags);
    if (ret < 0) {
      e = errno;
      DUMP0("Recv error (%s)", strerror(e));
      return ret;
    } else if (ret == 0) {
      DUMP0("recv(2) returns 0");
      return k;
    }
    k += ret;
  }
  return k;
}
