/* sqrt.c -- implement the `sqrt'  dynamically loadable builtin
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

#if HAVE_CONFIG_H
#  include <config.h>
#endif

#include <math.h>
#include <sic/module.h>

#define builtin_table	sqrt_LTX_builtin_table

/* Generate prototype. */
SIC_BUILTIN(builtin_sqrt);

Builtin builtin_table[] = {
  { "sqrt", builtin_sqrt, 1, 1 },
  { 0, 0, -1, -1 }
};

BUILTIN_DECLARATION(sqrt)
{
  char *endptr = NULL;
  int status = SIC_ERROR;
  long argument =   strtol (argv[1], &endptr, 0);

  if  (!endptr || *endptr != 0)
    {
      sic_result_clear (sic);
      sic_result_append (sic, "\"", argv[1], "\" is not a valid integer",
			 NULL);
    }
  else if (argument < 0)
    {
      sic_result_clear (sic);
      sic_result_append (sic, "sqrt failed: ", strerror (errno), NULL);
    }
  else
    {
      printf ("%.2f\n", sqrt ((double) argument)); 
      status = SIC_OKAY;
    }
  
  return status;
}
