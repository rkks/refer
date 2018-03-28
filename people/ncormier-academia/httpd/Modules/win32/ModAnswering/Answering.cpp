#include <iostream>
#include <fstream>

#include <time.h>
#include <errno.h>

#include "Reader.h"
#include "Answering.h"
#include "XmlContainer.h"
#include "ServerAnswer.h"
#include "UrlEnc.h"
#include "DBuffer.h"
#include "Actions.h"

using namespace	std;
using namespace mozia::answering;

/*
**
** Todo :
**	- Determination de l'extension
**	- Determination du content type
**
*/

namespace	mozia
{
  namespace	answering
  {
    typedef struct  sMethodTab
    {
      char  *method;
      HeaderResponse key[11];
    }               tMethodTab;

    tMethodTab  glMethodTab[] =
    {
      {"GET", {Date, Server, Last_Modified, ETag, Accept_Ranges,
      Connection, Content_type, Content_Length, Content_Range, RespNone}},
      {"HEAD", {Date, Server, Last_Modified, ETag, Accept_Ranges,
      Connection, Content_type, Content_Length, Content_Range, RespNone}},
      {"POST", {Date, Server, Last_Modified, ETag, Accept_Ranges,
      Connection, Content_type, Content_Length, Content_Range, RespNone}},
      {"OPTIONS", {Date, Server, Content_Length, Allow, RespNone}}
    };

    Answering::Answering(s_ModuleArg *arg)
    {
      this->arg = arg;
    }

    Answering::~Answering(void)
    {
    }

    char	*Answering::getExtension(const char *name)
    {
		if (name == NULL)
			return (NULL);
		
		string	sname = string(name);
      size_t	pos;

      pos = sname.rfind('.', -1);
      if (pos < 0)
        return (NULL);
      return ((char *)name + pos + 1);
    }

    char  *Answering::getMsg(int status)
    {
      char  stat[4];

      sprintf_s(stat, 4, "%3d", status);
      string method = this->_request->GetMethod();
      if (!(*this->arg->xmlconf)["codes"].Exists(method.c_str()))
        method = "default";
      if (!(*this->arg->xmlconf)["codes"][method.c_str()].Get(stat))
        method = "default";
      if (!(*this->arg->xmlconf)["codes"][method.c_str()].Get(stat))
        return ("Ok");
      return ((char *)(*this->arg->xmlconf)["codes"][method.c_str()].Get(stat)->c_str());
    }

    void Answering::Split(const string& str,
      vector<string>& tokens,
      const string& delimiters = ",")
    {
      string::size_type pos;
      string::size_type lastPos = 0;
      do
      {
        pos = str.find_first_of(delimiters, lastPos);
        if (pos == string::npos)
          tokens.push_back(str.substr(lastPos));
        else
          tokens.push_back(str.substr(lastPos, pos - lastPos));
        lastPos = pos + delimiters.length();
      }
      while (string::npos != pos);
    }

    bool  Answering::setRanges()
    {
      string::size_type ret = 0;
      string range = (*this->_answer->GetRequest())["Range"];
      vector<string> ranges;
      vector<string> inranges;
      vector<string>::iterator iranges;
      vector<string>::iterator iinranges;
      char  *begin;
      char  *end;
      __int64  ibegin;
      __int64  iend;
      __int64  ibegin2;
      __int64  iend2;
      int     i;
      int     j;

      ret = range.find("bytes=");
      if (ret != 0)
        return (false);
      range.erase(0, 6);
      if (range.length() == 0)
        return (false);
      this->Split(range, ranges);
      iranges = ranges.begin();
      while (iranges != ranges.end())
      {
        inranges.clear();
        begin = NULL;
        end = NULL;
        this->Split(*iranges, inranges, "-");
        iinranges = inranges.begin();
        if (iinranges != inranges.end())
        {
          begin = (char *) (*iinranges).c_str();
          iinranges++;
        }
        if (iinranges != inranges.end())
          end = (char *) (*iinranges).c_str();
        this->_answer->addRange(begin, end);
        iranges++;
      }
      this->_answer->IsRange(true);
      for (i = 0; this->_answer->GetRange(ibegin, iend, i); i++)
        for (j = 0; this->_answer->GetRange(ibegin2, iend2, j); j++)
        {
          if (j != i && ibegin2 >= ibegin && ibegin2 <= iend)
          {
            this->_answer->changeRange(ibegin, iend2, i);
            this->_answer->delRange(j);
            j--;
          }
          if (j != i && iend2 >= ibegin && iend2 <= iend)
          {
            this->_answer->changeRange(ibegin2, iend, i);
            this->_answer->delRange(j);
            j--;
          }
        }
      return (true);
    }

    void  Answering::chkDefaultPage(void)
    {
      if (this->_answer->GetFileError() == READER_ERROR_ISDIR)
      {
        if ((*this->arg->xmlconf)["virtualhosts"][this->_request->GetHost()].Exists("defaultpages"))
        {
          const vector_value  *vect = (*this->arg->xmlconf)["virtualhosts"][this->_request->GetHost()]["defaultpages"].GetVector("defaultpage");
          vector_value::const_iterator iter_c = vect->begin();
          string  path = this->_answer->GetPath();
          string  tpath;
          string  mpath;
          __int64 size;
          __int64 msize;
          int     error;
          int     merror;

          if (path.substr(path.length() - 1, 1) != "/")
            path += "/";
          while (iter_c != vect->end())
          {
            tpath = path + *(*iter_c);
            error = Reader::getFileStat(tpath, size, this->_answer->MDate);
            if (error == READER_ERROR_NOERROR)
            {
              this->_answer->SetFileError(error);
              this->_answer->SetType(REG_FILE);
              this->_answer->SetFileSize(size);
              delete [] this->_answer->GetPath();
              this->_answer->SetPath(ZiaString::dup(tpath.c_str()));
              return ;
            }
            else if (error != READER_ERROR_NOFILE)
            {
              mpath = tpath;
              merror = error;
              msize = size;
            }
            iter_c++;
          }
          if (mpath.length() != 0)
          {
            this->_answer->SetFileError(merror);
            this->_answer->SetFileSize(msize);
            delete [] this->_answer->GetPath();
            this->_answer->SetPath(ZiaString::dup(mpath.c_str()));
          }
        }
      }
    }

    bool  Answering::chkRoot(const char *rpath, const char *path)
    {
      char full[_MAX_PATH];

      if(!_fullpath(full, path, _MAX_PATH))
        return (false);
      for (int i = 0; rpath[i] && full[i]; i++)
        if (full[i] != rpath[i] && rpath[i] != '/' && rpath[i] != '\\'
          && full[i] != '/' && full[i] != '\\')
          return (false);
      return (true);
    }

    bool		  Answering::getStatus()
    {
      string	path;
      __int64 size = 0;

      if (!strcmp(this->_request->GetMethod(), "head") ||
        !strcmp(this->_request->GetMethod(), "options"))
        this->_answer->SetType(HEAD);
      this->_answer->SetVersion(this->_request->GetVersion());
      if (this->_request->GetUrl() == NULL ||
        this->_request->GetHost() == NULL||
        this->_request->GetVersion() == NULL)
      {
        this->_answer->SetStatus(400);
        this->_answer->SetMsg(this->getMsg(400));
        this->_answer->SetType(REG_FILE);
        return (false);
      }
      if (strcmp(this->_request->GetVersion(), "HTTP/1.0") &&
        strcmp(this->_request->GetVersion(), "HTTP/1.1"))
      {
        this->_answer->SetVersion("HTTP/1.1");
        this->_answer->SetStatus(505);
        this->_answer->SetMsg(this->getMsg(505));
        this->_answer->SetType(REG_FILE);
        return (false);
      }
      if (!strcmp(this->_request->GetUrl(), "*") &&
        strcmp(this->_request->GetMethod(), "options"))
      {
        this->_answer->SetStatus(400);
        this->_answer->SetMsg(this->getMsg(400));
        this->_answer->SetType(REG_FILE);
        return (false);
      }
      if ((*this->arg->xmlconf)["virtualhosts"].Exists(this->_request->GetHost()) == false)
      {
        this->_answer->SetStatus(404);
        this->_answer->SetMsg(this->getMsg(404));
        this->_answer->SetType(REG_FILE);
        return (false);
      }
      // test si c'est un cgi
      if ((*this->arg->xmlconf)["virtualhosts"][this->_request->GetHost()].Get("cgi-alias") &&
        !strncmp((*this->arg->xmlconf)["virtualhosts"][this->_request->GetHost()].Get("cgi-alias")->c_str(),
        this->_answer->GetRequest()->GetUrl(),
        (*this->arg->xmlconf)["virtualhosts"][this->_request->GetHost()].Get("cgi-alias")->size()))
      {
        this->_answer->SetType(CGI_BIN);
        path = (*this->arg->xmlconf)["virtualhosts"][this->_request->GetHost()].Get("cgi-root")->c_str();
        path.append(this->_request->GetUrl() +
          (*this->arg->xmlconf)["virtualhosts"][this->_request->GetHost()].Get("cgi-alias")->size());
        this->_answer->SetRoot((char *)(*this->arg->xmlconf)["virtualhosts"][this->_request->GetHost()].Get("cgi-root")->c_str());
      }
      else
      {
        path = (*this->arg->xmlconf)["virtualhosts"][this->_request->GetHost()].Get("documents-root")->c_str();
        path.append(this->_request->GetUrl());
        this->_answer->SetRoot((char *)(*this->arg->xmlconf)["virtualhosts"][this->_request->GetHost()].Get("documents-root")->c_str());
      }

      if ((*this->arg->xmlconf)["virtualhosts"][this->_request->GetHost()].Get("cgi-bin"))
        this->_answer->SetCGIPath((char *)(*this->arg->xmlconf)["virtualhosts"][this->_request->GetHost()].Get("cgi-bin")->c_str());
      this->_answer->SetPath(ZiaString::dup(path.c_str()));

      this->_answer->SetFileError(Reader::getFileStat(path, size, this->_answer->MDate));
      this->_answer->SetFileSize(size);
      switch (this->_answer->GetFileError())
      {
      case READER_ERROR_ISDIR :
        this->_answer->SetType(LIST_DIR);
        break ;
      case READER_ERROR_ACCESS :
        this->_answer->SetType(REG_FILE);
        this->_answer->SetStatus(403);
        this->_answer->SetMsg(this->getMsg(403));
        return (false);
        break ;
      case READER_ERROR_NOFILE :
        this->_answer->SetType(REG_FILE);
        this->_answer->SetStatus(404);
        this->_answer->SetMsg(this->getMsg(404));
        return (false);
        break ;
      }
      if (!chkRoot(this->_answer->GetRoot(), path.c_str()))
      {
        this->_answer->SetType(REG_FILE);
        this->_answer->SetStatus(403);
        this->_answer->SetPath(NULL);
        this->_answer->SetMsg(this->getMsg(403));
        return (false);
      }
      this->chkDefaultPage();
      if ((*this->_answer->GetRequest())["Range"])
        if (!this->setRanges())
        {
          this->_answer->SetStatus(416);
          this->_answer->SetMsg(this->getMsg(416));
          return (false);
        }
        else
        {
          this->_answer->SetStatus(206);
          this->_answer->SetMsg(this->getMsg(206));
          return (true);
        }
      this->_answer->SetStatus(200);
      this->_answer->SetMsg(this->getMsg(200));
      return (true);
    }

    void	  Answering::SwitchMethod()
    {
      int   i;
      int		j;
      char  *method = this->_request->GetMethod();

      for (i = 0; i < sizeof(glMethodTab) / sizeof(glMethodTab[i]); i++)
        if (!ZiaString::casecmp(glMethodTab[i].method, method))
        {
          this->_answer->SetValid(this->getStatus());
          for (j = 0; glMethodTab[i].key[j] != RespNone; j++)
            this->SwitchHeader(glMethodTab[i].key[j]);
          break ;
        }
    }

    void  Answering::DecodeContent(void)
    {
      UrlEnc UrlEncode;

      if (this->_request->GetUrl())
      {
        char *tmp = UrlEncode.Decode(this->_request->GetUrl());
        delete [] this->_request->GetUrl();
        this->_request->SetUrl(tmp);
      }
    }

    void	Answering::Start(TicketsHandler *th)
    {
      if (th->CurTicket->CData == NULL)
      {
        th->Drop("/dev/null", CORE_DELETE_TICKET);
        return ;
      }
      if (th->CurTicket->CData->GetType() == "ClientRequest")
        this->_request = (ClientRequest*) th->CurTicket->CData;
      else
      {
        th->Drop("/dev/null", CORE_DELETE_TICKET);
        return ;
      }
      this->_answer = (ServerAnswer *) th->CurTicket->GetMyData();
      if (!this->_answer)
      {
        this->_answer = new ServerAnswer();
        this->_answer->SetRequest(this->_request);
        th->CurTicket->SetMyData(this->_answer);
        this->DecodeContent();
        this->SwitchMethod();
        th->CurTicket->CData = this->_answer;
        switch (this->_answer->GetType())
        {
        case LIST_DIR :
          th->Drop("LISTDIR", LISTDIR_BAZOU);
          break ;
        case CGI_BIN :
          th->Drop("CGI", CGI_PROCESS);
          break ;
        case HEAD :
          th->CurTicket->CData = this->_answer->Render();
          th->Drop("NETWORK", NETWORK_SENDDATA);
          break ;
        case REG_FILE :
          th->Drop("FILEREG", FILEREG_SENDFILE);
          break ;
        }
        return ;
      }
      th->Drop("/dev/null", CORE_DELETE_TICKET);
    }
  }
}
