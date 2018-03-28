/* syntax.h -- create and administer syntax tables
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

#ifndef SIC_SYNTAX_H
#define SIC_SYNTAX_H 1

#include <sic/common.h>

BEGIN_C_DECLS

struct sic;

/* Declaration generator. */
#define SYNTAX_DECLARATION(name)				\
	int							\
	CONC(syntax_, name) (sic, in, out)			\
	     struct sic *sic;					\
	     BufferIn *in;					\
	     BufferOut *out;

typedef struct {
  char *start;			/* start of buffered string */
  size_t i;			/* offset to next unused char */
  size_t lim;			/* upper limit for I */
} Buffer;

typedef struct {
  Buffer buf;			/* the buffer details */
  char *start;			/* start of the current command */
} BufferIn;

typedef struct {
  Buffer buf;			/* the buffer details */
  int *offv;			/* vector of offsets */
  size_t offc;			/* count of offsets in OFFV */
  size_t argind;		/* offset to start of current argument */
} BufferOut;

typedef int SyntaxHandler PARAMS((struct sic *sic, BufferIn *in,
				  BufferOut *out));
typedef int SyntaxInit PARAMS((struct sic *sic));
typedef int SyntaxFinish PARAMS((struct sic *sic, BufferIn *in,
				  BufferOut *out));

typedef struct syntaxnode SyntaxTable;

typedef struct syntax {
  SyntaxHandler *handler;
  char *ch;
} Syntax;

extern int syntax_install PARAMS((struct sic *sic, Syntax *table));
extern SyntaxHandler *syntax_handler PARAMS((struct sic *sic, int ch));

END_C_DECLS

#endif /* !SIC_SYNTAX_H */
