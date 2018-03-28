#include <string>

# include "Dbuffer.h"
# include "ServerAnswer.h"
# include "CgiSave.h"

using namespace std;
using namespace mozia::answering;
using namespace mozia::modules::utils;

namespace mozia
{
  namespace modules
  {
    namespace cgi
    {
      CgiSave::CgiSave(ServerAnswer *answer)
      {
        this->_sanswer = answer;
        this->_delete = false;
      }

      CgiSave::~CgiSave(void)
      {
      }

      ServerAnswer  *CgiSave::getServerAnswer(void)
      {
        return (this->_sanswer);
      }

      Buffer  *CgiSave::getContent(void)
      {
        return (&this->_content);
      }

      void  CgiSave::addContent(const Buffer &buffer)
      {
        this->_content.addData(buffer);
      }

      void  CgiSave::setDelete(void)
      {
        this->_delete = true;
      }

      bool  CgiSave::getDelete(void)
      {
        return (this->_delete);
      }

      Buffer  *CgiSave::getBuffer(void)
      {
        return (&this->_buf);
      }

      Buffer  *CgiSave::getTBuffer(void)
      {
        return (&this->_tbuf);
      }

	  const string&  CgiSave::GetType() const
	  {
		  static  string type = "CgiSave";
		  return (type);
	  }
    }
  }
}
