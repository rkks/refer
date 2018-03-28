#ifndef __ON_FILE_H__
# define __ON_FILE_H__

# include <string>
# include <fstream>

# include "DBuffer.h"
# include "Reader.h"
# include "IModuleData.h"

using namespace std;
using namespace mozia::modules::utils;
using namespace mozia::modules;
using namespace mozia::answering;

namespace mozia
{
  namespace filereq
  {
    class COnFile : public IModuleData
    {
    private:
      unsigned __int64*      _getRange;
      int  _curRange;
      unsigned int  _nbRange;
      unsigned __int64       _inRange;
      Reader        *_reader;

    public:
      void  SetRanges(unsigned __int64* theRange, unsigned int nb);
      void  SetReader(Reader *reader);
      Buffer *GetData();
      Reader  &GetReader(void);
	  const string&  GetType() const;
      COnFile(Reader *reader);
      ~COnFile(void);
    };
  }
}
#endif /* ON_FILE_H_ */