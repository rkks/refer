#include <stdio.h>
#include "tconf.h"

int dir_func(char *key, char *value) {
  printf("got %s %s\n", key, value);
  return 0;
}

tconf_t tc[] = {{"dir", tconf_func, &dir_func}, };

int main(int argc, char * argv[]) {
  tconf(CONF, tc, sizeof(tc)/sizeof(*tc), 0);
}
