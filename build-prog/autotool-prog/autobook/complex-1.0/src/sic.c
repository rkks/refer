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

#include "common.h"
#include "sic_repl.h"

#ifndef BUFSIZ
#  define BUFSIZ 256
#endif

#define SICRCFILE	".sicrc"

static int evalsicrc (Sic *sic);

int
main (int argc, char *const argv[], char *const envp[])
{
  int result = EXIT_SUCCESS;
  FILE *in = stdin;
  Sic *sic = sic_new ();
  
/** @start 1 */
  /* initialise the module subsystem */
  if (module_init () != SIC_OKAY)
      sic_fatal ("module initialisation failed");

  if (module_load (sic, NULL) != SIC_OKAY)
      sic_fatal ("sic initialisation failed");
/** @end 1 */

  /* initial symbols */
  sicstate_set (sic, "PS1", "] ", NULL);
  sicstate_set (sic, "PS2", "- ", NULL);
  for (; *envp; ++envp)
    {
      size_t delim = strcspn (*envp, "=");
      char *name = XMALLOC (char, 1 + delim);

      strncpy (name, *envp, delim);
      name[delim] = 0;

      sicstate_set (sic, name, *envp + delim +1, NULL);
    }
  sicstate_set (sic, "SHELL", argv[0], NULL);
  sicstate_set (sic, "SIC_VERSION", VERSION, NULL);
  
  /* evaluate the startup file */
  evalsicrc (sic);
  
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

/** @start 2 */
static int
evalsicrc (Sic *sic)
{
  int result = SIC_OKAY;
  char *home = getenv ("HOME");
  char *sicrcpath, *separator = "";
  int len;

  if (!home)
    home = "";

  len = strlen (home);
  if (len && home[len -1] != '/')
    separator = "/";

  len += strlen (separator) + strlen (SICRCFILE) + 1;

  sicrcpath = XMALLOC (char, len);
  sprintf (sicrcpath, "%s%s%s", home, separator, SICRCFILE);

  if (access (sicrcpath, R_OK) == 0)
    result = source (sic, sicrcpath);

  return result;
}
/** @end 2 */
