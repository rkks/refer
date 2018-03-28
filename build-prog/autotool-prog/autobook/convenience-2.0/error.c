#include <stdio.h>

#include "source.h"

static char *program_name = NULL;

void
gratuitous (void)
{
  /* Gratuitous display of convenience library functionality!  */
  double argument = 0.0;
  foo (argument);
}

void
set_program_name (char *path)
{
  if (!program_name)
    program_name = basename (path);
}

void
error (char *message)
{
  fprintf (stderr, "%s: ERROR: %s\n", program_name, message);
  exit (1);
}
