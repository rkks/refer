#if HAVE_CONFIG_H
#  include <config.h>
#endif

#if HAVE_STRING_H
#  include <string.h>
#elif HAVE_STRINGS_H
#  include <strings.h>
#endif

#if !HAVE_STRRCHR
#  ifndef strrchr
#    define strrchr rindex
#  endif
#endif

char*
basename (char *path)
{
  /* Search for the last directory separator in PATH.  */
  char *basename = strrchr (path, '/');
  
  /* If found, return the address of the following character,
     or the start of the parameter passed in.  */
  return basename ? ++basename : path;
}

