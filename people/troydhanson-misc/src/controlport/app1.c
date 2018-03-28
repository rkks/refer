/* test application for control port library */

#include <time.h>
#include <stdio.h>
#include <string.h>
#include "controlport.h"

#define SOCKETNAME "./socket"

int time_cmd(void *cp, cp_arg_t *arg, void *data) {
  time_t now = time(NULL);
  char *t = asctime(localtime(&now));
  cp_add_reply(cp, t, strlen(t));
  return 0;
}

cp_cmd_t cmds[] = {
  {"time", time_cmd },
  {NULL, NULL},
};

int main(int argc, char *argv[]) {
  void *cp;

  cp = cp_init(SOCKETNAME, cmds, NULL, 0);
  cp_run(cp);
}

