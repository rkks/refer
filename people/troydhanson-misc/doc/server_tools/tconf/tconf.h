typedef int (*tconf_func_t)(char *key, void *arg);

typedef struct {
 char *name;
 enum {tconf_int, tconf_str, tconf_bool, tconf_func } type;
 void *addr;
} tconf_t;

#define TCONF_DISALLOW_UNKNOWN (1 << 0)

int tconf(char *file, tconf_t *tconf, int tclen, int opt);
