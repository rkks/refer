#include "IDirHandling.h"

namespace mozia
{
  namespace listdir
  {
    void    IDirHandling::cpyAvoidingSlashRepeat(char *dst, const char* src)
    {
      int   i;
      int   j;
      bool  isSep = false;
      static bool  prevWasSlash = false;

      if (dst == 0x0)
      {
        prevWasSlash = false;
        return;
      }
      for (i = 0, j =0; src[i]; i++)
      {
        isSep = (src[i] == '/' || src[i] == '\\');
        if (!isSep || prevWasSlash == false)
        {
          if (isSep)
            prevWasSlash = true;
          else
            prevWasSlash = false;
          dst[j++] = (isSep ? '/' : src[i]);
        }
      }
      dst[j] = 0;
    }
 
    string  IDirHandling::GetPath()
    {
      return (this->_path);
    }

    void    IDirHandling::SetPath(const char* Path, const char* Url)
    {
      char* tmp = new char[strlen(Path) + strlen(Url) + 1];
      this->cpyAvoidingSlashRepeat(tmp, Path);
      this->cpyAvoidingSlashRepeat(tmp + strlen(Path), Url);
      this->_path = string(tmp);
      delete[] tmp;
    }

    void  IDirHandling::SetBuffer(string buffer)
    {
      this->_buff.setData(buffer);
    }

    Buffer* IDirHandling::GetBuffer()
    {
      return (&(this->_buff));
    }
  }
}