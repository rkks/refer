#if HAVE_CONFIG_H
#  include <config.h>
#endif

#if HAVE_MATH_H
#  include <math.h>
#endif

void
foo (double argument)
{
  printf ("cos (%g) => %g\n", argument, cos (argument));
}

