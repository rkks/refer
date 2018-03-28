#ifndef __DIRCACHE_H__
# define __DIRCACHE_H__

# include <list>
# include "FileInfo.h"

# define COLUMN_SIZE  10

namespace mozia
{
  namespace listdir
  {
    class DirCache
    {
    private:
      string  _host;
      string  _url;
      int     _maxSize;
      int     _maxFile;
      int     _cachedMDate;

    public:
      string  Host();
      string  Url();

      list<class FileInfo*>  Files;
      void  Add(FileInfo*);
      void  Sort(const char*);
      void  SortByName();
      void  SortBySize();
      void  SortByDate();
      int   MaxFileLength();
      int   MaxSizeLength();
      int   GetMDate();
      DirCache(const string& host, const string& url);
      ~DirCache();
    };
  }
}

#endif // __DIRCACHE_H__