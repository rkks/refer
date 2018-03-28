/* builtin.h -- installing and removing sic builtin commands
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

#ifndef SIC_BUILTIN_H
#define SIC_BUILTIN_H 1

#include <sic/common.h>
#include <sic/eval.h>

BEGIN_C_DECLS

/* Declaration generator. */
#define BUILTIN_DECLARATION(name)				\
	int							\
	CONC(builtin_, name) (sic, argc, argv)			\
	     Sic *sic;						\
	     int argc;						\
	     char *const argv[];

typedef int (*builtin_handler) PARAMS((Sic *sic,
				       int argc, char *const argv[]));

typedef struct {
  const char *name;
  builtin_handler func;
  int min, max;
} Builtin;

typedef struct builtintab BuiltinTab;

extern Builtin *builtin_find PARAMS((Sic *sic, const char *name));
extern int builtin_install PARAMS((Sic *sic, Builtin *table));
extern int builtin_remove PARAMS((Sic *sic, Builtin *table));

END_C_DECLS

#endif /* !SIC_BUILTIN_H */
