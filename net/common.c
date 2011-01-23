#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include "common.h"

/**
 * Write binary sequence to file.
 * @param fd   file descriptor to which write data
 * @param src  character buffer to write
 * @param size write size by byte
 * @returns succeeded: the number of bytes wrote. error occurred: -1
 */
int write_b(int fd, unsigned char* src, size_t size) {
  size_t wrote_bytes = 0;
  size_t ctr;
  while (wrote_bytes < size) {
    ctr = write(fd, src + wrote_bytes, size - wrote_bytes);
    if (ctr == -1) {
      DUMP1("Write Error: %s", strerror(errno));
      return -1;
    }
    wrote_bytes += ctr;
  }
  return wrote_bytes;
}

void* safe_malloc(size_t size) {
  void* p;
  p = malloc(size);
  if (!p) {
    DUMP1("Malloc (size=%zd) failed!", size);
    exit(1);
  }
  return p;
}

ssize_t safe_write(int fd, const void* buf, size_t count) {
  size_t k = 0;
  int e;
  ssize_t ret;
  while(k < count) {
    ret = write(fd, buf + k, count - k);
    if (ret < 0) {
      e = errno;
      DUMP1("Write(2) error (%s)", strerror(e));
      exit(1);
    }
    k += ret;
  }
  return count;
}
