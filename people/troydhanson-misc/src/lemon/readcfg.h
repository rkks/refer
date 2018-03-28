typedef struct {
  char *name;
  int port;
} server_t;

typedef struct {
  int line;
  char *file;
  int rc;
  server_t *top;
} parse_t;

char *slurp(char *file, size_t *len);
int tok(char **c, size_t *bsz, size_t *toksz, int *line);
void new_server(parse_t *ps);

#define newtop(out,ary)                                       \
do {                                                          \
  if (ary ## _left == 0) {                                    \
    void *nary = realloc(ary,(ary ## _used+10)*sizeof(*ary)); \
    if (!nary) {                                              \
      printf("out of memory\n");                              \
      exit(-1);                                               \
    }                                                         \
    ary = nary;                                               \
    ary ## _left = 10;                                        \
  }                                                           \
  out = &ary[ary ## _used];                                   \
  (ary ## _left)--;                                           \
  (ary ## _used)++;                                           \
} while(0);

