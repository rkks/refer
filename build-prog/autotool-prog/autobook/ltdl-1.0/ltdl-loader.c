#include <stdio.h>
#include <stdlib.h>
#ifndef EXIT_FAILURE
#  define EXIT_FAILURE	1
#  define EXIT_SUCCESS	0
#endif

#include <limits.h>
#ifndef PATH_MAX
#  define PATH_MAX 255
#endif

#include <string.h>
#include <ltdl.h>

#ifndef MODULE_PATH_ENV
#  define MODULE_PATH_ENV	"MODULE_PATH"
#endif

typedef int entrypoint (const char *argument);

/* Save and return a copy of the dlerror() error  message,
   since the next API call may overwrite the original. */
static char *dlerrordup (char *errormsg);

int
main (int argc, const char *argv[])
{
  char *errormsg = NULL;
  lt_dlhandle module = NULL;
  entrypoint *run = NULL;
  int errors = 0;

  if (argc != 3)
    {
      fprintf (stderr, "USAGE: main MODULENAME ARGUMENT\n");
      exit (EXIT_FAILURE);
    }

  /* Initialise libltdl. */
  errors = lt_dlinit ();

  /* Set the module search path. */
  if (!errors)
    {
      const char *path = getenv (MODULE_PATH_ENV);

      if (path != NULL)
	errors = lt_dlsetsearchpath (path);
    }
  
  /* Load the module. */
  if (!errors)
    module = lt_dlopenext (argv[1]);

  /* Find the entry point. */
  if (module)
    {
      run = (entrypoint *) lt_dlsym (module, "run");

      /* In principle, run might legitimately be NULL, so
	 I don't use `run == NULL' as an error indicator
	 in general. */
      errormsg = dlerrordup (errormsg);
      if (errormsg != NULL)
	{
	  errors = lt_dlclose (module);
	  module = NULL;
	}
    }
  else
    errors = 1;

  /* Call the entry point function. */
  if (!errors)
    {
      int result = (*run) (argv[2]);
      if (result < 0)
	errormsg = strdup ("module entry point execution failed");
      else
	printf ("\t=> %d\n", result);
    }

  /* Unload the module, now that we are done with it. */
  if (!errors)
    errors = lt_dlclose (module);

  if (errors)
    {
      /* Diagnose the encountered error. */
      errormsg = dlerrordup (errormsg);

      if (!errormsg)
	{
	  fprintf (stderr, "%s: dlerror() failed.\n", argv[0]);
	  return EXIT_FAILURE;
	}
    }

  /* Finished with ltdl now. */
  if (!errors)
    if (lt_dlexit () != 0)
      errormsg = dlerrordup (errormsg);

  if (errormsg)
    {
      fprintf (stderr, "%s: %s.\n", argv[0], errormsg);
      free (errormsg);
      exit (EXIT_FAILURE);
    }

  return EXIT_SUCCESS;
}

/* Be careful to save a copy of the error message,
   since the  next API call may overwrite the original. */
static char *
dlerrordup (char *errormsg)
{
  char *error = (char *) lt_dlerror ();
  if (error && !errormsg)
    errormsg = strdup (error);
  return errormsg;
}
