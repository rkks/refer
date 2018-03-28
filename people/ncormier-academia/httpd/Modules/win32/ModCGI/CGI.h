// Mozia - $Id$

#ifndef _CGI_H_
# define _CGI_H_

# include "TicketsHandler.h"
# include "DBuffer.h"
# include "ServerAnswer.h"
# include "CgiSave.h"

using namespace mozia::answering;
using namespace mozia::modules::cgi;

namespace mozia
{
  namespace modules
  {
    namespace cgi
    {
      class CGI
      {
      public:
        CGI(TicketsHandler *th, XmlContainer *xmlconf);
        ~CGI(void);
        void  mainLoop(void);
      private:
        string  _isAScript(const string &s);
        LPVOID  _createNewEnv(const string &args, const string &additional);
        string  _forgeEnv(ServerAnswer *sanswer);
        int _runProcess(CgiSave *save, const string &path, const string &arguments, const string &additional, const Buffer *topipe);
        void  _runCGI(CgiSave *save);
        void  _bufferize(CgiSave *save);
        void  _postProcess(CgiSave *save);
        void  _internalServerError(CgiSave *save);

        TicketsHandler *_th;
        XmlContainer  *_xml;
        string  _endl;
      };
    }
  }
}

#endif /* !_CGI_H_ */
