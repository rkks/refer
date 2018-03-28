#ifndef CONTROL_PORT_H
#define CONTROL_PORT_H

#include <stdlib.h>

typedef struct {
  int     argc; /* conventional argc/argv args */
  char  **argv;
  size_t *lenv; // len of each arg, useful if client sends null-containing args
} cp_arg_t;

typedef int (cp_cmd_f)(void *cp, cp_arg_t *arg, void *data);

typedef struct {
  char *name;
  cp_cmd_f *cmdf;
  char *help;
} cp_cmd_t;

void *cp_init(char *path, cp_cmd_t *cmds, void *data, int timeout);
void cp_add_cmd(void*, char *name, cp_cmd_f *cmdf, char *help, void *data);
int cp_run(void*);
void cp_shutdown(void*); /* callable from another thread */

/* these are used within command callbacks */
void cp_disconnect(void*);
void cp_add_reply(void *, void *buf, size_t len);

#endif 
