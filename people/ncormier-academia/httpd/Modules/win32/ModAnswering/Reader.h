#ifndef __READER_H__
# define __READER_H__

# include <string>
# include <fstream>
# include <windows.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <io.h>
#include <errno.h>

#ifndef S_IRUSR
# define S_IRUSR _S_IREAD
#endif

# include "DBuffer.h"
# include "IModuleData.h"

using namespace std;
using namespace mozia::modules::utils;
using namespace mozia::modules;

namespace mozia
{
  namespace answering
  {
    class Reader: public IModuleData
    {
    private:
      int		_chunkSize;
      __int64		_fileSize;
      char		_mDate[32];
      HANDLE	_handle;
      char		*_ibuf;
# define READER_ERROR_NOERROR   0
# define READER_ERROR_NOFILE    1
# define READER_ERROR_EOF       2
# define READER_ERROR_INTERNAL  3
# define READER_ERROR_ISDIR		4
# define READER_ERROR_ACCESS		5
      int _error;
      string  _fileName;
      Buffer  _buf;

    public:
# define READER_CHUNK_SIZE  4096
      Reader(string path);
      ~Reader(void);
      Reader(void);

	  const string&  GetType() const;
      int		getChunkSize(void);
      void		setChunkSize(int size);
      int		getError(void);
      Buffer	*getData(void);
      char		*getMDate();
      void		Close(void);
      __int64		getFileSize(void);
      BOOL    seekData(unsigned __int64 begin);
      static int getFileStat(string path, __int64 &size, char MDate[32]);
    };
  }
}

#endif //__READER_H__
