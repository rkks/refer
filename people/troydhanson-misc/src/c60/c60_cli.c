#include <stdlib.h>
#include <zmq.h>
#include "c60_internal.h"
#include "utstring.h"

/* read mapping from file. open push conn to each c60 node. */
void *c60_client_init_fromfile(char *file, UT_string *err) {
  c60_t *c60=NULL;
  int rc = -1;

  if ( (c60 = calloc(1,sizeof(*c60))) == NULL) goto done;
  if ( (c60->zcontext = zmq_init(1)) == NULL) goto done;
  if (c60_load_map(c60,file,err) == -1) goto done;

  rc = 0;

 done:
  if (rc == -1) {
    if (c60) free(c60);
    /* TODO deep clean up c60 if non-null */
    c60 = NULL;
  }
  return c60;
}

void c60_client_close(void *_c60) {
  c60_t *c60 = (c60_t*)_c60;
  int i;
  for(i=0; i< C60_NUM_BUCKETS; i++) {
    if (c60->bucket[i]->socket) {
      zmq_close(c60->bucket[i]->socket);
      c60->bucket[i]->socket = NULL;
    }
  }
  zmq_term(c60->zcontext); 
  /* TODO any deep cleaning if c60 grows */
  free(c60);
}

/* hash dest via the map; push msg to appropriate node */
int c60_send(void *_c60, char *dest, char *msg, size_t len) {
  c60_t *c60 = (c60_t*)_c60;
  int b;
  b = c60_get_bucket(dest);
  if (zmq_send(c60->bucket[b]->socket, msg, len, 0) == -1) {
  }
  return -1;
}


