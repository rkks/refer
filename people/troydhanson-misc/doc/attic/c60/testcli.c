#include <stdio.h>
#include "c60.h"
int main() {
  UT_string *err;
  utstring_new(err);
  void *c60;

  c60 = c60_client_init_fromfile("config.txt",err);
  if (!c60) fprintf(stderr,"init failed: %s",utstring_body(err));
  c60_send(c60, "recipient", "hello!", 7);
  c60_client_close(c60);
  return 0;

}
