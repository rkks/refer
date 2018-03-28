/* sic.h -- create and maintain Sic ADT
   Copyright (C) 2000 Gary V. Vaughan
  
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.
  
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
  
   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/

#ifndef SIC_SIC_H
#define SIC_SIC_H 1

#include <sic/common.h>
#include <sic/error.h>
#include <sic/list.h>
#include <sic/syntax.h>

BEGIN_C_DECLS

enum {
  SIC_ERROR = -1,
  SIC_OKAY = 0,
  SIC_INCOMPLETE,
  SIC_BREAK,
  SIC_CONTINUE,
  SIC_EXIT
};

struct builtintab;

typedef struct statedata {
  struct statedata *next;	/* so they can be chained */
  const char *key;		/* used as a key to find the right data */
  sic_ptr data;			/* associated state data */
  void (*delete) PARAMS((sic_ptr data));
} SicState;

typedef struct sic {
  char *result;			/* result string */
  size_t len;			/* bytes used by result field */
  size_t lim;			/* bytes allocated to result field */
  struct builtintab *builtins;	/* tables of builtin functions */
  SyntaxTable **syntax;		/* dispatch table for syntax of input */
  List *syntax_init;		/* stack of syntax state initialisers */
  List *syntax_finish;		/* stack of syntax state cleanup funcs */
  SicState *state;		/* state data from syntax extensions */
} Sic;

extern Sic *sic_new PARAMS((void));
extern Sic *sic_delete PARAMS((Sic *stale));

extern int sicstate_set   PARAMS((Sic *sic, const char *key, sic_ptr value,
				  void (*delete) (sic_ptr value)));
extern sic_ptr sicstate_get   PARAMS((Sic *sic, const char *key));
extern int sicstate_clear PARAMS((Sic *sic, const char *key));

extern Sic *sic_result_clear PARAMS((Sic *sic));
extern const char *sic_result_get PARAMS((Sic *sic));
extern Sic *sic_result_set PARAMS((Sic *sic, const char *value, size_t len));
extern Sic *sic_result_append PARAMS((Sic *sic, const char *value, ...));

END_C_DECLS

#endif /* !SIC_SIC_H */
