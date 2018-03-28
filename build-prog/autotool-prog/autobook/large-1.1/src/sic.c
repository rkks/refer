/* sic.c -- read commands, evaluate them and print the results
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

#include <signal.h>

#include "sic_builtin.h"
#include "sic_repl.h"
#include "sic_syntax.h"

#ifndef BUFSIZ
#  define BUFSIZ 256
#endif

static int sic_init  PARAMS((Sic *sic));

int
main (argc, argv)
     int argc;
     char * const argv[];
{
  int result = EXIT_SUCCESS;
  FILE *in = stdin;
  Sic *sic = sic_new ();
  
  /* initialise the system */
  if (sic_init (sic) != SIC_OKAY)
      sic_fatal ("sic initialisation failed");

  /* initial symbols */
  sicstate_set (sic, "PS1", "] ", NULL);
  sicstate_set (sic, "PS2", "- ", NULL);
  
  /* Interactive implies unbuffered output and ignore interrupts. */
  if (argc > 1 && argv[1][0] != '-')
    in = fopen (argv[1], "rt");
  else if (isatty(0))
    {
      signal (SIGINT, SIG_IGN);
      setbuf (stdout, NULL);
      is_interactive = 1;
    }

  /* evaluate the input stream */
  evalstream (sic, in);

  exit (result);
}

static int
sic_init (sic)
     Sic *sic;
{
  int status = SIC_ERROR;

  /* Push syntax initialisation and finishing functions onto their
     respective stacks. */
  sic->syntax_init = list_cons (list_new (syntax_init),
				    sic->syntax_init);
  sic->syntax_finish = list_cons (list_new (syntax_finish),
				  sic->syntax_finish);

  /* Initialise the `builtin' functions. */
  status = builtin_install (sic, builtin_table);

  /* Initialise the `syntax' tables. */
  if (status == SIC_OKAY)
    status = syntax_install (sic, syntax_table);

  return status;
}

