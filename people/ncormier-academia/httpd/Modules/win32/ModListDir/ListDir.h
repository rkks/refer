#ifndef __LISTDIR_H__
# define __LISTDIR_H__

# include <windows.h>
# include <stdio.h>
# include <string>

# include "ClientRequest.h"
# include "TicketsHandler.h"
# include "ModuleArg.h"
# include "DirHandling.h"
# include "DirCache.h"
# include "DBuffer.h"

using namespace std;
using namespace mozia::protocol;

namespace mozia
{
  namespace listdir
  {
    class ListDir
    {
    public:
		  ListDir();
		  ~ListDir();

		  void      Start(s_ModuleArg*);
      bool      RenderFile(DirHandling *, ClientRequest*);

      char*     GetUrl();
      char*     GetHost();

      void      AddCache(DirCache*);
      bool      FindCachedDir();
      string    GenerateListDir(DirCache*, char*);
      DirCache* GetCacheDir();

    private:
      Buffer	      _buff;
      string        _path;
      HANDLE        hFind;
      s_ModuleArg*  _arg;

      char*         _url;
      char*         _host;
      char*         _filename;
      bool          _isCurrentDir;
      bool          _isParentDir;

      DirCache*     _cached[10];
      int           _cacheLeft;
      int           _cachedDir;

      void          setUrl(const char*);
      void          setHost(const char*);
      void          setFileName(const char*);
      char*         removeTrailingSlashes(const char*);
      string        addSpaces(int nb);
      string        includeFile(string filename);
    };
  }
}

#endif // __LISTDIR_H__