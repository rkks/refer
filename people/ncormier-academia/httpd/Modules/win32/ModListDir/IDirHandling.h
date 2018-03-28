#ifndef __IDIRHANDLING_H__
# define __IDIRHANDLING_H__

# include "DBuffer.h"
# include "FileInfo.h"
# include "ModuleArg.h"

using namespace std;
using namespace mozia::modules::utils;

namespace mozia
{
  namespace listdir
  {
    class IDirHandling
    {
			virtual void      findFirst(void) = 0;
			virtual void      formatResult(void) = 0;
      void              cpyAvoidingSlashRepeat(char*, const char*);

    protected:
      Buffer	          _buff;
      bool              _opened;
			bool              _exists;
			string            _root;
			string            _path;

		public:
			virtual bool        NextFile(void) = 0;
      virtual FileInfo*   GetFileInfo() = 0;

      string              GetPath();
      void                SetBuffer(string buffer);
      Buffer*             GetBuffer();
      void                SetPath(const char* path, const char* url);
    };
  }
}

#endif // __IDIRHANDLING_H__