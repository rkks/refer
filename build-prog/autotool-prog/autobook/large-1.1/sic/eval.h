/* eval.h -- create and dispatch command vectors
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

#ifndef SIC_EVAL_H
#define SIC_EVAL_H 1

#include <sic/common.h>
#include <sic/sic.h>

BEGIN_C_DECLS

/* A special property of TOKENS structures is that all of the pointers
   in ARGV point into a single block of memory which are freed all at
   once by freeing the first element. */
typedef struct {
  int  argc;			/* number of elements in ARGV */
  char **argv;			/* array of pointers to elements */
  size_t lim;			/* number of bytes allocated */
} Tokens;

extern int eval PARAMS((Sic *sic, Tokens *tokens));
extern int untokenize PARAMS((Sic *sic, char **pcommand, Tokens *tokens));
extern int tokenize PARAMS((Sic *sic, Tokens **ptokens, char **pcommand));

END_C_DECLS

#endif /* !SIC_EVAL_H */
