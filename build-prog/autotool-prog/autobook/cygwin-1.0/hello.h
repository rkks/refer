#ifndef HELLO_H
#define HELLO_H 1

#if HAVE_CONFIG_H
#  include <config.h>
#endif

#if _WIN32
#  if DLL_EXPORT
#    define HELLO_SCOPE   __declspec(dllexport)
#  else
#    if LIBHELLO_DLL_IMPORT
#      define HELLO_SCOPE   extern __declspec(dllimport)
#    endif
#  endif
#endif
#ifndef HELLO_SCOPE
#  define HELLO_SCOPE     extern
#endif

HELLO_SCOPE const char *greet;

extern int hello (const char *who);

#endif /* !HELLO_H */
