#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

int do_read(int fd, void *buf, size_t len) {
  int rc;

  while(len) {
    if ( (rc = read(fd, buf, len)) < 0) {
      fprintf(stderr,"read error: %s\n", strerror(errno));
      return -1;
    }
    assert(rc > 0);
    len -= rc;
    buf += rc;
  }

  return 0;
}

int do_write(int fd, void *buf, size_t len) {
  int rc;

  while(len) {
    if ( (rc = write(fd, buf, len)) < 0) {
      fprintf(stderr,"write error: %s\n", strerror(errno));
      return -1;
    }
    assert(rc > 0);
    len -= rc;
    buf += rc;
  }

  return 0;
}
