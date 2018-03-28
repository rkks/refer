#ifndef __ZIA_STRING_
#define __ZIA_STRING_

#include  <string.h>

namespace mozia
{
  namespace ZiaString
  {
    inline char  *dup(const char *str)
    {
      size_t len;
      char  *out;

      len = strlen(str) + 1;
      out = new char[len];
      out[len - 1] = 0;
      memcpy(out, str, len);
      return (out);
    }

    inline int  casecmp(const char *str1, const char *str2)
    {
#ifdef WIN32
      return (_stricmp(str1, str2));
#else
      return (strcasecmp(str1, str2));
#endif /* WIN32 */
    }

    inline int  casencmp(const char *str1, const char *str2, int length)
    {
#ifdef WIN32
      return (_strnicmp(str1, str2, length));
#else
      return (strncasecmp(str1, str2, length));
#endif /* WIN32 */
    }
  }
}

#endif /*__ZIA_STRING_*/
