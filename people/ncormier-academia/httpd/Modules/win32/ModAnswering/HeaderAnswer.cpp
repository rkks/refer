#include <iostream>
#include <fstream>
#include <sstream>

#include <time.h>
#include <errno.h>

#include "Reader.h"
#include "Answering.h"
#include "XmlContainer.h"
#include "ServerAnswer.h"
#include "DBuffer.h"

using namespace	std;
using namespace mozia::answering;
using namespace mozia;

/*
"Date", "Server", "Last-Modified",
"ETag", "Accept-Ranges", "Content-Length",
"Connection", "Content-type", NULL}
*/

namespace	mozia
{
  namespace	answering
  {
    void	Answering::SwitchHeader(HeaderResponse field)
    {
      switch (field)
      {
      case Date :
        char    tmp[32];
        time_t	ttemp;

        ttemp = time(NULL);
#pragma warning (disable : 4996)
        strftime(tmp, 31, "%a, %d %b %Y %H:%M:%S GMT", gmtime(&ttemp));
        this->_answer->addField("Date", tmp);
        break ;
      case Server :
          this->_answer->addField("Server", (char *) (*this->arg->xmlconf)["general"].Get("name")->c_str());
          break ;
      case Last_Modified :
        if (this->_answer->GetFileError() == READER_ERROR_NOERROR)
          this->_answer->addField("Last-Modified", this->_answer->MDate);
        break ;
      case ETag :
        break ;
      case Accept_Ranges :
        if (this->_answer->GetValid())
          this->_answer->addField("Accept-Ranges", "bytes");
        break ;
      case Content_Range :
        if (this->_answer->GetValid() && this->_answer->IsRange())
        {
          ostringstream	oss;

          oss << this->_answer->GetFileSize();
          string range = (*this->_answer->GetRequest())["Range"];
          range = range.replace(5, 1, " ") + "/" + oss.str();
          this->_answer->addField("Content-Range", (char *) range.c_str());
        }
        break ;
      case Content_Length :
        this->_answer->addField("Content-Length", this->_answer->GetResponseSize());
        break ;
      case Connection :
        this->_answer->addField("Connection", "close");
        break ;
      case Content_type :
        {
        char  *ext = this->getExtension(this->_answer->GetPath());
        if (ext && (*this->arg->xmlconf).Exists("mimetypes") && (*this->arg->xmlconf)["mimetypes"].Get(ext))
        {
          this->_answer->addField("Content-type", (char *)(*this->arg->xmlconf)["mimetypes"].Get(ext)->c_str());
        }
        break ;
        }
      case Allow :
        this->_answer->addField("Allow", "GET, HEAD, OPTIONS");
        break ;
      }
    }
  }
}