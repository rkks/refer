
#include "FileInfo.h"


namespace mozia
{
  namespace listdir
  {
    string  FileInfo::FormatDate(time_t st_time)
    {
      char  buffer[26];

      ctime_r(&st_time, buffer);
      buffer[24] = 0;
      return (buffer);
    }

    string        FileInfo::GetSize()
    {
      static char buffer[128];

      snprintf(buffer, sizeof(buffer), "%lld", this->Size);
      return (buffer);
    }
  }
}
