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

#include <dlfcn.h>
/* This is missing from very old Linux libc. */
#ifndef RTLD_NOW
#  define RTLD_NOW 2
#endif

typedef int entrypoint (const char *argument);

/* Save and return a copy of the dlerror() error  message,
   since the next API call may overwrite the original. */
static char *dlerrordup (char *errormsg);

int
main (int argc, const char *argv[])
{
  const char modulepath[1+ PATH_MAX];
  const char *errormsg = NULL;
  void *module = NULL;
  entrypoint *run = NULL;
  int errors = 0;

  if (argc != 3)
    {
      fprintf (stderr, "USAGE: main MODULENAME ARGUMENT\n");
      exit (EXIT_FAILURE);
    }

  /* Set the module search path. */
  getcwd (modulepath, PATH_MAX);
  strcat (modulepath, "/");
  strcat (modulepath, argv[1]);
  
  /* Load the module. */
  module = dlopen (modulepath, RTLD_NOW);
  if (!module)
    {
      strcat (modulepath, ".so");
      module = dlopen (modulepath, RTLD_NOW);
    }
  if (!module)
    errors = 1;

  /* Find the entry point. */
  if (!errors)
    {
      run = dlsym (module, "run");
      /* In principle, run might legitimately be NULL, so
	 I don't use `run == NULL' as an error indicator. */
      errormsg = dlerrordup (errormsg);

      if (errormsg != NULL)
	errors = dlclose (module);
    }

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
    errors = dlclose (module);

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
  
  if (errormsg)
    {
      fprintf (stderr, "%s: %s.\n", argv[0], errormsg);
      free (errormsg);
      return EXIT_FAILURE;
    }
  
  return EXIT_SUCCESS;
}

/* Be careful to save a copy of the error message,
   since the next API call may overwrite the original. */
static char *
dlerrordup (char *errormsg)
{
  char *error = (char *) dlerror ();
  if (error && !errormsg)
    errormsg = strdup (error);
  return errormsg;
}
