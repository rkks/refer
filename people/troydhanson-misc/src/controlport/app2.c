/* test application for control port library */

#include <time.h>
#include <stdio.h>
#include <string.h>
#include "controlport.h"

#define SOCKETNAME "./socket"

int wc_cmd(void *cp, cp_arg_t *arg, void *data) {
  int i;
  char buf[40];
  for(i=0; i < arg->argc; i++) {
    snprintf(buf,sizeof(buf),"#%u: %u bytes\n", i, arg->lenv[i]);
    cp_add_reply(cp, buf, strlen(buf));
  }
  return 0;
}

cp_cmd_t cmds[] = {
  {"wc", wc_cmd },
  {NULL, NULL},
};

int main(int argc, char *argv[]) {
  void *cp;

  cp = cp_init(SOCKETNAME, cmds, NULL, 0);
  cp_run(cp);
}

