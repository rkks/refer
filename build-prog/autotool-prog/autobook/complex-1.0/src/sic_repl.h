/* sic_repl.h -- read commands, evaluate them and print the results
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

#ifndef SIC_REPL_H
#define SIC_REPL_H 1

#include <sic/common.h>
#include <sic/sic.h>

BEGIN_C_DECLS

extern int is_interactive;

extern int source     (Sic *sic, const char *path);
extern int evalstream (Sic *sic, FILE *stream);
extern int evalline   (Sic *sic, char **pline);

END_C_DECLS

#endif /* !SIC_REPL_H */

