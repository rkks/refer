%include {#include <assert.h>}
%include {#include <stdlib.h>}
%include {#include <string.h>}
%include {#include "readcfg.h"}
%token_prefix TOK_
%token_type {char*}
%extra_argument {parse_t *ps}
%syntax_error  {printf("error in %s line %d\n", ps->file, ps->line); ps->rc=-1;}
%parse_failure {printf("error in %s line %d\n", ps->file, ps->line); ps->rc=-1;}

file ::= sections.
sections ::= sections section.
sections ::= section.
section ::= SERVER LCURLY sbody RCURLY. { new_server(ps); }
sbody ::= sbody kvpair.
sbody ::= kvpair.
kvpair ::= NAME DOTALN(A).  {ps->top->name = strdup(A); }
kvpair ::= PORT NUMBER(A).  {ps->top->port = atoi(A); }
