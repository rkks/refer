#if HAVE_CONFIG_H
#  include <config.h>
#endif

#include <stdio.h>

/* Use the system size_t if it has one, or fallback to config.h */
#if STDC_HEADERS || HAVE_STDDEF_H
#  include <stddef.h>
#endif
#if HAVE_SYS_TYPES_H
#  include <sys/types.h>
#endif

/* One of the following headers will have prototypes for malloc
   and free on most systems.  If not, we don't add explicit
   prototypes which may generate a compiler warning in some
   cases -- explicit  prototypes would certainly cause
   compilation to fail with a type clash on some platforms. */
#if STDC_HEADERS || HAVE_STDLIB_H
#  include <stdlib.h>
#endif
#if HAVE_MEMORY_H
#  include <memory.h>
#endif

#if HAVE_STRING_H
#  include <string.h>
#else
#  if HAVE_STRINGS_H
#    include <strings.h>
#  endif /* !HAVE_STRINGS_H */
#endif /* !HAVE_STRING_H */

#if ! HAVE_STRCHR

/* BSD based systems have index() instead of strchr() */
#  if HAVE_INDEX
#    define strchr index
#  else /* ! HAVE_INDEX */

/* Very old C libraries have neither index() or strchr() */
#    define strchr rpl_strchr

static inline const char *strchr (const char *str, int ch);

static inline const char *
strchr (const char *str, int ch)
{
  const char *p = str;
  while (p && *p && *p != (char) ch)
    {
      ++p;
    }

  return (*p == (char) ch) ? p : 0;
}
#  endif /* HAVE_INDEX */

#endif /* HAVE_STRCHR */

/* BSD based systems have bcopy() instead of strcpy() */
#if ! HAVE_STRCPY
# define strcpy(dest, src)	bcopy(src, dest, strlen(src) + 1)
#endif

/* Very old C libraries have no strdup(). */
#if ! HAVE_STRDUP
# define strdup(str)		strcpy(malloc(strlen(str) + 1), str)
#endif

char*
rpl_fopen (const char *pathname, char *mode)
{
    char *result = NULL;
    char *p = mode;

    /* Scan to the end of mode until we find 'b' or 't'. */ 
    while (*p && *p != 'b' && *p != 't')
      {
        ++p;
      }

    if (!*p)
      {
        fprintf(stderr,
            "*WARNING* rpl_fopen called without mode 'b' or 't'\n");
      }

#if USE_FOPEN_BINARY && USE_FOPEN_TEXT
    result = fopen(pathname, mode);
#else
    {
        char ignore[3]= "bt";
        char *newmode = strdup(mode);
        char *q       = newmode;

        p = newmode;

#  if ! USE_FOPEN_TEXT
        strcpy(ignore, "b")
#  endif
#  if ! USE_FOPEN_BINARY
        strcpy(ignore, "t")
#  endif

        /* Copy characters from mode to newmode missing out
           `b' and/or `t'. */
        while (*p)
          {
            while (strchr(ignore, *p))
              {
                ++p;
              }
            *q++ = *p++;
          }
        *q = '\0';

        result = fopen(pathname, newmode);

        free(newmode);
    }
#endif /* USE_FOPEN_BINARY && USE_FOPEN_TEXT */

    return result;
}
