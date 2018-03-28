#ifndef _CGISAVE_H_
# define _CGISAVE_H_

#include <string>

# include "ServerAnswer.h"
# include "Dbuffer.h"

using namespace std;
using namespace mozia::answering;
using namespace mozia::modules::utils;

namespace mozia
{
  namespace modules
  {
    namespace cgi
    {
      class CgiSave : public IModuleData
      {
      public:
        CgiSave(ServerAnswer *answer);
        virtual ~CgiSave(void);
        ServerAnswer  *getServerAnswer(void);
        Buffer  *getContent(void);
        void  addContent(const Buffer &buffer);
        void  setDelete(void);
        bool  getDelete(void);
        Buffer  *getBuffer(void);
        Buffer  *getTBuffer(void);
		const string&  GetType() const;
      private:
        ServerAnswer  *_sanswer;
        Buffer  _content;
        Buffer  _buf;
        Buffer  _tbuf;
        bool  _delete;
      };
    }
  }
}

#endif /* !_CGISAVE_H_ */
