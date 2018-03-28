#ifndef _C60_INTERNAL_
#define _C60_INTERNAL_

#include "utstring.h"

#define C60_NUM_BUCKETS 4096

/* TODO extern C */

typedef struct {
  void *socket;
} c60_conn;

typedef struct {
  void *zcontext; /* zero MQ context */
  c60_conn *bucket[C60_NUM_BUCKETS];
} c60_t;

typedef struct {
  c60_t c60;
} c60_server_t;

int c60_load_map(c60_t *c60, char *file, UT_string *err);
void *c60_server_init_fromfile(char *file, UT_string *err);

#endif // _C60_INTERNAL_
