#include "FileInfo.h"

namespace mozia
{
  namespace listdir
  {
    string  FileInfo::FormatDate(time_t st_time)
    {
      char  buffer[26];

      ctime_s(buffer, 26, &st_time);
      buffer[24] = 0;
      return (buffer);
    }  

    string        FileInfo::GetSize()
    {
      static char buffer[128];

      sprintf_s(buffer, "%lld", this->Size);
      return (buffer);
    }
  }
}