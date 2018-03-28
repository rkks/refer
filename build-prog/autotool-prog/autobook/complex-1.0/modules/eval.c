/* eval.c -- implement the eval dynamically loadable builtin
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

#define builtin_table	eval_LTX_builtin_table

/* Generate prototype. */
SIC_BUILTIN(builtin_eval);

Builtin builtin_table[] = {
  { "eval", builtin_eval, 1, -1 },
  { 0, 0, -1, -1 }
};

BUILTIN_DECLARATION (eval)
{
  int status = SIC_ERROR;
  Tokens *tokens;
  char *command;

  {
    Tokens tmp;
    tmp.argv = (char **) &argv[1];
    tmp.argc = argc -1;
    tmp.lim  = 0;
  
    status = untokenize (sic, &command, &tmp);
  }

  if (status == SIC_OKAY)
    status = tokenize (sic, &tokens, &command);

  if (status == SIC_INCOMPLETE)
    {
      sic_result_set (sic, "incomplete command", -1);
      status = SIC_ERROR;
      goto error;
    }

  status = eval (sic, tokens);

 error:
  free (*tokens->argv);
  XFREE (tokens->argv);
  XFREE (tokens);
  XFREE (command);
  
  return status;
}
