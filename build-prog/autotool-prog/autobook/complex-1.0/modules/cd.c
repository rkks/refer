/* cd.c -- implement the `cd'  dynamically loadable builtin
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

#include <sic/module.h>

#define builtin_table	cd_LTX_builtin_table

/* Generate prototype. */
SIC_BUILTIN(builtin_cd);

Builtin builtin_table[] = {
  { "cd", builtin_cd, 0, 1 },
  { 0, 0, -1, -1 }
};

BUILTIN_DECLARATION(cd)
{
  const char *destination = argv[1];
  int status = SIC_ERROR;

  if (!destination)
    destination = sicstate_get (sic, "HOME");
  
  if  (!destination)
      sic_result_set (sic, "unbound variable \"HOME\"", -1);
  else if (chdir (destination) < 0)
    {
      sic_result_clear (sic);
      sic_result_append (sic, destination, ": ", strerror (errno), NULL);
    }
  else
    status = SIC_OKAY;

  return status;
}
