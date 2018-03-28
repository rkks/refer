#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

/* wrapper around write that drains out short writes */
#define do_write(fd,buf,len) \
do { \
  size_t _len=len; \
  char *_buf=(char*)buf; \
  while(_len) { \
    int rc = write(fd,_buf,_len);  \
    if (rc > 0) {  \
      _len -= rc; \
      _buf += rc;  \
    } else if (rc == -1) { \
      fprintf(stderr,"write error: %s\n", strerror(errno)); \
      break; \
    } \
  } \
} while(0)

/* read wrapper; wants len bytes, loops if it takes many short reads */
#define do_read(fd,buf,len)  \
do { \
  size_t _len=len; \
  char *_buf=(char*)buf; \
  while(_len) { \
    int rc = read(fd,_buf,_len); \
    if (rc > 0) { \
      _len -= rc; \
      _buf += rc; \
    } else if (rc <= 0) { \
      fprintf(stderr,"read error: %s\n",((rc==0)?"eof":strerror(errno))); \
      break; \
    } \
  } \
} while(0)
