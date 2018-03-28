/* error.c -- display formatted error diagnostics of varying severity
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

#include "common.h"
#include "error.h"

static void error (int exit_status, const char *mode,
		   const char *message, va_list ap);

const char *program_name = NULL;

void
set_program_name (const char *path)
{
  SIC_ASSERT (!program_name);
  program_name = xstrdup ((char *) basename (path));
}

static void
error (int exit_status, const char *mode, const char *message, va_list ap)
{
  fprintf (stderr, "%s: %s: ", program_name, mode);
  vfprintf (stderr, message, ap);
  fprintf (stderr, ".\n");

  if (exit_status >= 0)
    exit (exit_status);
}

void
sic_warning (const char *message, ...)
{
  va_list ap;
  va_start (ap, message);
  error (-1, "warning", message, ap);
  va_end (ap);
}

void
sic_error (const char *message, ...)
{
  va_list ap;
  va_start (ap, message);
  error (-1, "ERROR", message, ap);
  va_end (ap);
}

void
sic_fatal (const char *message, ...)
{
  va_list ap;
  va_start (ap, message);
  error (EXIT_FAILURE, "FATAL", message, ap);
  va_end (ap);
}
