// Mozia - $Id: cmpfunc.h 313 2005-11-27 19:48:08Z yann $

#ifndef __CMPFUNC_H__
# define __CMPFUNC_H__

#include "ZiaString.h"

using namespace mozia;

namespace mozia
{
  namespace utils
  {
    struct eqstr
    {
      bool operator()(const char* s1, const char* s2) const
      {
        return (strcmp(s1, s2) < 0);
      }
    };

	struct eqistr
    {
      bool operator()(const char* s1, const char* s2) const
      {
        return (ZiaString::casecmp(s1, s2) < 0);
      }
    };

    struct eqint
    {
      bool operator()(const int a, const int b) const
      {
        return (a < b);
      }
    };

    struct eqchr
    {
      bool operator()(const char c1, const char c2) const
      {
        return (c1 < c2);
      }
    };
  }
}

#endif //!__CPMFUNC_H__
