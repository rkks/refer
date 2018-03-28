#ifndef __DIRHANDLING_H__
# define __DIRHANDLING_H__

# include "IModuleData.h"
# include "IDirHandling.h"
# include "FileInfo.h"
# include <sys/types.h>
# include <dirent.h>

using namespace std;
using namespace mozia::modules;

namespace mozia
{
  namespace listdir
    {
      class DirHandling : public IModuleData, public IDirHandling
	{
	private:
	  char*             _file;
	  int               _size;
	  int               _type;
	  time_t            _adate;
	  time_t            _cdate;
	  time_t            _mdate;

	  void              findFirst(void);
	  void              formatResult(void);

/* 	  HANDLE            _hFind; */
/* 	  WIN32_FIND_DATAA  _FindFileData; */

	  DIR		    *_handle;
	  struct dirent	    *_ent;

	public:
	  DirHandling(const string path);
	  ~DirHandling(void);

	  bool            NextFile(void);
	  FileInfo*       GetFileInfo();
	  const string&  GetType() const;

	  bool            End;
	};
    }
}

#endif // __DIRHANDLING_H__
