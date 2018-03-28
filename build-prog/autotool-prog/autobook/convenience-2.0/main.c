#include <stdio.h>
#include "error.h"

int
main (int argc, char *argv[])
{
  set_program_name (argv[0]);
  gratuitous ();
  error ("This program does nothing!");
}
