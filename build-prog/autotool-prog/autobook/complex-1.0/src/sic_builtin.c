/* sic_builtin.c -- basic builtin commands loaded statically
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

/** @start 1 */
#if HAVE_CONFIG_H
#  include <config.h>
#endif

#include "module.h"
#include "sic_repl.h"

/** @end 1 */
#if 0
/* This is a reduced block for the benefit of the book, the real
   macro, as used by the code, is further down. */
/** @start 1 */
/* List of built in functions. */
#define builtin_functions               \
        BUILTIN(exit,           0, 1)   \
        BUILTIN(load,           1, 1)   \
        BUILTIN(unload,         1, -1)

/** @end 1 */
#endif

/** @start 2 */
/* List of built in functions. */
#define builtin_functions               \
        BUILTIN(exit,           0, 1)   \
        BUILTIN(load,           1, 1)   \
        BUILTIN(source,         1, -1)  \
        BUILTIN(unload,         1, -1)

/** @end 2 */

#define BUILTIN(name, min, max) \
        extern SIC_BUILTIN(CONC(builtin_, name));
builtin_functions
#undef BUILTIN

/* name to function map */
Builtin builtin_table[] = {

#define BUILTIN(name, min, max) \
        { STR(name), CONC(builtin_, name), (min), (max) },
  builtin_functions
#undef BUILTIN

  { NULL, NULL, -1, -1 }
};

BUILTIN_DECLARATION(exit)
{
  return SIC_EXIT;
}

/** @start 1 */
BUILTIN_DECLARATION (load)
{
  int status = SIC_ERROR;

  if (module_load (sic, argv[1]) < 0)
    {
      sic_result_clear (sic);
      sic_result_append (sic, "module \"", argv[1], "\" not loaded: ",
                         module_error (), NULL);
    }
  else
    status = SIC_OKAY;

  return status;
}

/** @end 1 */
/** @start 2 */
BUILTIN_DECLARATION (source)
{
  int status = SIC_OKAY;
  int i;

  for (i = 1; status == SIC_OKAY && argv[i]; ++i)
    status = source (sic, argv[i]);

  return status;
}
/** @end 2 */
/** @start 1 */
BUILTIN_DECLARATION (unload)
{
  int status = SIC_ERROR;
  int i;

  for (i = 1; argv[i]; ++i)
    if (module_unload (sic, argv[i]) != SIC_OKAY)
      {
        sic_result_clear (sic);
        sic_result_append (sic, "module \"", argv[1],
                           "\" not unloaded: ", module_error (), NULL);
      }
    else
      status = SIC_OKAY;

  return status;
}
/* @end 1 */
