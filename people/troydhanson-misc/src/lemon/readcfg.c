#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "readcfg.h"
#include "cfg.h"

/* lemon functions */
void *ParseAlloc();
void Parse();
void ParseFree();

server_t *servers;
int servers_used=0, servers_left=10; /* used with newtop for vector of servers*/

int verbose=0;

void usage(char *prog) {
  fprintf(stderr, "usage: %s [-v] <file>\n", prog);
  exit(-1);
}
 
void new_server(parse_t *ps) {
  if ((ps->top->name == NULL) || (ps->top->port == 0)) {
    printf("missing name or port (%s line %d)\n", ps->file, ps->line);
    ps->rc = -1;
  }
  else newtop(ps->top, servers);
}

int main(int argc, char *argv[]) {

  servers = malloc(sizeof(server_t)*10);

  char **toks = malloc(sizeof(char*)*10); /* token copies produced in parsing */
  int toks_used=0, toks_left=10;         /* used in conjunction with newtop */

  char *file, *buf, *c, **tk;
  size_t len,toklen;
  int opt,id,i;
  parse_t ps; /* our own "supplemental" parser state */
  void *p;    /* lemon parser */

  while ( (opt = getopt(argc, argv, "v+")) != -1) {
    switch (opt) {
      case 'v': verbose++; break;
      default: usage(argv[0]); break;
    }
  }
 
  if (optind < argc) file=argv[optind++];
  else usage(argv[0]);
  buf = slurp(file, &len);
  p = ParseAlloc(malloc);

  ps.file = file;
  ps.line = 1;
  ps.rc = 0;
  newtop(ps.top, servers); /* initial server element to be populated */
  ps.top->name = NULL;
  ps.top->port = 0;


  c = buf;
  while ( (id=tok(&c,&len,&toklen,&ps.line)) > 0) {
    /*printf("got token [%.*s] id=%d line=%d\n",(int)toklen, c, id, ps.line);*/
    newtop(tk,toks);
    *tk = strndup(c,toklen);
    Parse(p, id, *tk, &ps);
    if (ps.rc == -1) goto done;
    len -= toklen;
    c += toklen;
  }
  if (id == -1) {
    printf("tokenizer error in %s line %d\n", ps.file, ps.line);
    ps.rc = -1;
    goto done;
  }
  Parse(p, 0, NULL, &ps);
  if (ps.rc == -1) goto done;

  /* parsing succeeded */
  servers_used--; servers_left++;  /* throw away the unused top */

  for(i=0; i < servers_used; i++) {
    printf("server %d: %s:%d\n", i, servers[i].name, servers[i].port);
  }

 done:

  for(i=0; i < toks_used; i++) free(toks[i]);
  free(toks);
  for(i=0; i < servers_used; i++) free(servers[i].name);
  free(servers);
  ParseFree(p, free);
  free(buf);
  return 0;
}
