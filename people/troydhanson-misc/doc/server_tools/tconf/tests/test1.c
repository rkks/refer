#include <stdio.h>
#include "tconf.h"

struct {
  int servers;
  char *name;
} Cf = {
  .name = "default",
  .servers = 1,
};

tconf_t tc[] = {{"servers", tconf_int, &Cf.servers},
                {"name", tconf_str, &Cf.name}};

int main(int argc, char * argv[]) {
  tconf(CONF, tc, sizeof(tc)/sizeof(*tc), 0);
  printf("servers: %d\n", Cf.servers);
  printf("name: [%s]\n", Cf.name);
}
