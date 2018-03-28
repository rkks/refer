#ifndef __FILEINFO_H__
# define __FILEINFO_H__

# include <stdio.h>
# include <string>
# include <sys/stat.h>
# include <time.h>

using namespace std;

namespace mozia
{
  namespace listdir
  {
    class FileInfo
    {
    public:
			char*     File;
			long long Size;
			int       Type;
      time_t    Adate;
      time_t    Cdate;
      time_t    Mdate;
      string    FormatDate(time_t st_time);
      string    GetSize();
    };
  }
}

#endif // __FILEINFO_H__
