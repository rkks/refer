#include <fcntl.h>
#include <sys/stat.h>

#include "ListDir.h"
#include "ClientRequest.h"
#include "ServerAnswer.h"
#include "TicketsHandler.h"
#include "ModuleArg.h"
#include "XmlContainer.h"
#include "DirHandling.h"
#include "Actions.h"

using namespace std;
using namespace mozia::listdir;
using namespace mozia::protocol;
using namespace mozia::answering;

namespace mozia
{
  namespace listdir
  {
    static char*  ToLower(char *str)
    {
      int           len;
      static char*  buffer = NULL;
      static int    length = 0;

      if (str == NULL)
        return (NULL);
      len = strlen(str);
      if (length < len)
      {
        if (buffer == NULL)
          buffer = new char[len + 1];
        else
        {
          delete[] buffer;
          buffer = new char[len + 1];
        }
        length = len;
      }
      int i;
      for (i = 0; i < len; i++)
        if (str[i] >= 'A' && str[i] < 'Z')
          buffer[i] = str[i] + 'a' - 'A';
        else
          buffer[i] = str[i];
      buffer[i] = 0;
      return (buffer);
    }


    ListDir::ListDir()
    {
      this->_isCurrentDir = false;
      this->_isParentDir = false;
      this->_url = 0x0;
      this->_host = 0x0;
      this->_filename = 0x0;
      this->_cacheLeft = 10;
    }

    ListDir::~ListDir()
    {
      if (this->_url != 0x0)
        delete [] this->_url;
      this->_url = NULL;
      if (this->_host != 0x0)
        delete [] this->_host;
      this->_host = NULL;
      if (this->_filename != 0x0)
        delete [] this->_filename;
      this->_filename = NULL;
      for (int i = 0; i < (10 - this->_cacheLeft); i++)
        this->_cached[i]->~DirCache();
    }

	  void              ListDir::Start(s_ModuleArg* arg)
	  {
      string          target;
		  ClientRequest*	req = 0x0;
      DirHandling*    dir;

      this->_arg = arg;
      if (!arg->th->CurTicket->CData)
        return ;
      if (arg->th->CurTicket->CData->GetType() == "ServerAnswer")
	      req = (ClientRequest*) ((ServerAnswer *) arg->th->CurTicket->CData)->GetRequest();
      if (!(dir = (DirHandling*)arg->th->CurTicket->GetMyData()))
      {
        
        bool ret = ((*this->_arg->xmlconf).Exists("virtualhosts") && (*arg->xmlconf)["virtualhosts"].Exists(req->GetHost()));
        if (ret)
          target = *(*arg->xmlconf)["virtualhosts"][req->GetHost()].Get("documents-root");
        else
          target = "C:\\WINDOWS\\";
        dir = new DirHandling(target);
        dir->SetPath(target.c_str(), req->GetUrl());
        arg->th->CurTicket->SetMyData(dir);
      }
      if (req != 0x0 && this->RenderFile(dir, req))
      {
        arg->th->CurTicket->CData = dir->GetBuffer();
        arg->th->Drop("NETWORK", NETWORK_SENDDATA);
      }
      else
        arg->th->Drop("/dev/null", CORE_DELETE_TICKET);
	  }

    char*   ListDir::removeTrailingSlashes(const char *str)
    {
      char* newStr = ZiaString::dup(str);

      int idx = strlen(newStr) - 1;
      if (newStr[idx] == '/')
        newStr[idx--] = 0;
      return (newStr);
    }

    void    ListDir::setUrl(const char *url)
    {
      if (this->_url != 0x0)
        delete [] this->_url;
      this->_url = this->removeTrailingSlashes(url);
    }

    void    ListDir::setHost(const char* host)
    {
      if (this->_host != 0x0)
        delete [] this->_host;
      this->_host = this->removeTrailingSlashes(host);
    }

    void    ListDir::setFileName(const char* fileName)
    {
      int   i;
      int   count = 0;

      this->_isCurrentDir = false;
      this->_isParentDir = false;
      if (this->_filename != 0x0)
        delete [] this->_filename;
      this->_filename = this->removeTrailingSlashes(fileName);
      for (i = 0; this->_filename[i]; i++)
        if (this->_filename[i] == '.')
          count++;
        else
          break;
      if (this->_filename[i] == 0)
      {
        if (count == 1)
          this->_isCurrentDir = true;
        else if (count == 2)
          this->_isParentDir = true;
      }
    }

    string ListDir::addSpaces(int limit)
    {
      string  spaces = "";

      for (int i = 0; i < limit; i++)
        spaces += " ";
      return (spaces);
    }

    string  ListDir::includeFile(string filename)
    {
      string line;
      string file = "";
      ifstream include;

      include.open(filename.c_str());
      if (include.is_open())
      {
        while (!include.eof())
        {
          getline(include, line);
          file += line;
        }
        include.close();
      }
      return (file);
    }

    string  ListDir::GenerateListDir(DirCache* dir, char* args)
    {
      string          buffer;
      char            length[256];
      char*           icon;


      if (args != 0x0)
        dir->Sort(args);
      string DirName = "Index of " + (dir->Url()[0] ? dir->Url() : "/");
      buffer += "<html>\r\n <head>\r\n  <title>" + DirName + "</title>\r\n";
      buffer += "  <style>" + this->includeFile("config/ListDir.css") + "</style>";
      buffer += "</head>\r\n <body>\r\n  <pre>\n";
      buffer += " <h1>" + DirName + "</h1>";
      buffer += "  <a href='?name'>Filename</a>     " + this->addSpaces(dir->MaxFileLength() - 8);
      buffer += this->addSpaces(dir->MaxSizeLength() - 4) + "  <a href='?size'>Size</a>     ";
      buffer += "  <a href='?date'>Modification date</a>                      \n<hr>";
      if (dir->Url()[0])
      {
        buffer += "<img src='/icons/back.png' alt=' ' /> ";
        buffer += "<a class='parenLink' href='" + string(dir->Url()) +  "/..' style='margin-bottom:5px;'>Parent directory</a>\n\n";
      }

      int i = 1;
      for (list<class FileInfo*>::iterator ite = dir->Files.begin(); ite != dir->Files.end(); ite++)
      {
        FileInfo* FI = (*ite);
        if (strcmp(FI->File, ".") == 0)
          continue;
        if (strcmp(FI->File, "..") == 0)
          continue;
        if (FI->Type == 0)
          icon = "folder.png";
        else
        { 
          string sname = string(FI->File);
          int pos = sname.rfind('.', -1);
          icon = NULL;
          if (pos >= 0)
          {
            string ext = ToLower(FI->File + pos + 1);
            if ((*this->_arg->xmlconf).Exists("icons") && (*this->_arg->xmlconf)["icons"].Get(ext.c_str()) != NULL)
              icon = (char *)(*this->_arg->xmlconf)["icons"].Get(ext.c_str())->c_str();
          }
          if (icon == NULL)
              icon = "unknown.png";
        }
        string cssClass = (i++ % 2 ? "greyEntry" : "whiteEntry");
        string img  = "<div class='" + cssClass + "'><img align='middle' src='/icons/" + string(icon) + "' alt=' ' /> ";
        string link = "<a class='Entry' href='" + string(dir->Url()) + "/" + string(FI->File) + "'>" + FI->File + "</a>     " + this->addSpaces(dir->MaxFileLength() - strlen(FI->File));
        buffer += img;
        buffer += link;
        buffer += this->addSpaces(dir->MaxSizeLength() - FI->GetSize().length()) + FI->GetSize() + "     ";
        buffer += "  " + FI->FormatDate(FI->Mdate) + "</div>";
      }
      buffer += "</pre>\n";
      buffer += "</body>\n</html>\n\n";
      sprintf_s(length, "%d", buffer.length());
      buffer = "Content-Length: " + string(length) + "\r\n\r\n" + buffer;
      buffer = "HTTP/1.1 200 OK\r\n" + buffer;
      return (buffer);
    }

    void      ListDir::AddCache(DirCache* elem)
    {
      if (this->_cacheLeft == 0)
      {
        int i;

        this->_cached[0]->~DirCache();
        for (i = 0; i < 9; i++)
          this->_cached[i] = this->_cached[i + 1];
        this->_cacheLeft++;
      }
      this->_cached[10 - this->_cacheLeft] = elem;
      this->_cacheLeft--;
    }

    bool      ListDir::RenderFile(DirHandling *dir, ClientRequest* req)
    {
      bool          rendered = false;
      string        buffer;

      if (dir->End)
        return (false);
      this->setUrl(req->GetUrl());
      this->setHost(req->GetHost());

      // Pour verifier si le contenu du dossier a change
      struct __stat64 stats;
      if(_stat64(dir->GetPath().c_str(), &stats) == -1)
        cout << "_stat64 failed" << endl;

      if (this->FindCachedDir() && stats.st_mtime == this->GetCacheDir()->GetMDate())
      {
        DirCache* cachedDir = this->GetCacheDir();

        buffer = this->GenerateListDir(cachedDir, req->GetArgs());
        rendered = true;
      }
      else
      {
        DirCache* currentDir = new DirCache(this->_host, this->_url);
        while (dir->NextFile())
        {
          currentDir->Add(dir->GetFileInfo());
          rendered = true;
        }
        this->AddCache(currentDir);
        buffer = this->GenerateListDir(currentDir, req->GetArgs());
      }
      dir->SetBuffer(buffer);
      return (rendered);
    }
  
    bool  ListDir::FindCachedDir()
    {
      int i;

      for (i = 0; i < (10 - this->_cacheLeft); i++)
        if (this->_cached[i]->Host() == this->_host && this->_cached[i]->Url() == this->_url)
          break;
      if (i == (10 - this->_cacheLeft))
        return (false);
      this->_cachedDir = i;
      return (true);
    }

    DirCache* ListDir::GetCacheDir()
    {
      return (this->_cached[this->_cachedDir]);
    }
  }
}
