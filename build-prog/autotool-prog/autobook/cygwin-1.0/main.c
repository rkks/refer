#if HAVE_CONFIG_H
#  include <config.h>
#endif

#include "hello.h"

int
main (int argc, const char *const argv[])
{
    if (argc > 1)
      {
        greet = argv[1];
      }
    return hello("World");
}
