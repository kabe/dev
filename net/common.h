#ifndef __COMMON_H__
#define __COMMON_H__

#include <stdio.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <pthread.h>

#define DUMP0(...) fprintf(stderr, __VA_ARGS__)
#define DUMP1(s, ...) {                                                 \
    struct timespec ts;                                                 \
    clock_gettime(CLOCK_REALTIME, &ts);                                 \
    fprintf (stderr, "%ld %ld %d " s "\n", ts.tv_sec, ts.tv_nsec, getpid(), __VA_ARGS__); \
  }

int write_b(int, unsigned char*, size_t);
void* safe_malloc(size_t size);
ssize_t safe_write(int fd, const void* buf, size_t count);

#endif /* __COMMON_H__ */

