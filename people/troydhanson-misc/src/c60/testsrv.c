#include <stdio.h>
#include "utstring.h"
#include "c60_internal.h"

int main() {
  UT_string *err;
  utstring_new(err);
  void *c60s;

  c60s = c60_server_init_fromfile("config.txt",err);
  if (!c60s) fprintf(stderr,"init failed: %s",utstring_body(err));
  return 0;

}
