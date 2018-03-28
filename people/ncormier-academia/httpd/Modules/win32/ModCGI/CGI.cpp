// Mozia - $Id$

#include <windows.h>
#include <iostream>

#include <vector>
#include <map>
#include <sstream>

#include "Actions.h"
#include "TicketsHandler.h"
#include "Reader.h"
#include "CGI.h"
#include "ServerAnswer.h"
#include "UrlEnc.h"
#include "CgiSave.h"

using namespace std;
using namespace mozia::answering;
using namespace mozia::Transform;

namespace mozia
{
  namespace modules
  {
    namespace cgi
    {
      //
      // Constructeur
      //

      CGI::CGI(TicketsHandler *th, XmlContainer *xmlconf)
      {
        this->_th = th;
        this->_xml = xmlconf;
      }

      //
      // Destructeur
      //

      CGI::~CGI(void)
      {
      }

      //
      // Recupere le shebang s'il existe.
      //

      string  CGI::_isAScript(const string &s)
      {
        string  shebang(s.substr(0, s.find("\n") + 1));

        if ((memcmp(shebang.c_str(), "#!", 2) != 0) || (shebang[shebang.size() - 1] != '\n'))
          return ("");
        string execline(shebang.substr(2, shebang.size() - 2 - 2));
        return (execline);
      }

      //
      // Cree un nouvel environnement d'execution
      //

      LPVOID  CGI::_createNewEnv(const string &args, const string &additional)
      {
        vector<string>  v;
        size_t pos = 0;
        size_t lpos = 0;
        size_t totalsize = 0;
        size_t written = 0;
        unsigned int i;
        LPVOID  ptr;

        while ((pos = args.find_first_of("&", lpos)) != -1)
        {
          string tmp(args.substr(lpos, pos - lpos));
          v.push_back(tmp);
          totalsize += pos - lpos + 1;
          lpos = pos + 1;
        }
        if ((pos == -1) && (v.size() == 0) && (args.size() != 0))
        {
          v.push_back(args);
          totalsize += args.size();
        }
        if (additional != "")
        {
          v.push_back(additional);
          totalsize += additional.size();
        }
        ptr = new char[totalsize + 2];
        memset(ptr, 0, totalsize + 2);
        for (i = 0, written = 0; i < v.size(); i++)
        {
          memcpy((char *) ptr + written, v.at(i).c_str(), v.at(i).size());
          written += v.at(i).size() + 1;
        }
        return (ptr);
      }

      //
      // Lance un CGI
      //

      int  CGI::_runProcess(CgiSave *save, const string &path, const string &arguments, const string &additional, const Buffer *topipe = NULL)
      {
        STARTUPINFO si;
        PROCESS_INFORMATION pi;
        SECURITY_ATTRIBUTES saAttr; 
        HANDLE  hChildStdoutRd, hChildStdoutWr, hChildStdinRd, hChildStdinWr, hChildStdoutRdDup, hChildStdinWrDup;
        DWORD dwRead, dwWritten; 
        char  chBuf[4096];
        string output;

        saAttr.nLength = sizeof(SECURITY_ATTRIBUTES); 
        saAttr.bInheritHandle = TRUE; 
        saAttr.lpSecurityDescriptor = NULL; 

        if (CreatePipe(&hChildStdoutRd, &hChildStdoutWr, &saAttr, 0) == 0)
          return (0);
        if (DuplicateHandle(GetCurrentProcess(), hChildStdoutRd, GetCurrentProcess(), &hChildStdoutRdDup , 0, FALSE, DUPLICATE_SAME_ACCESS) == 0)
          return (0);
        if (CloseHandle(hChildStdoutRd) == 0)
          return (0);
        if (CreatePipe(&hChildStdinRd, &hChildStdinWr, &saAttr, 0) == 0)
          return (0);
        if (DuplicateHandle(GetCurrentProcess(), hChildStdinWr, GetCurrentProcess(), &hChildStdinWrDup, 0, FALSE, DUPLICATE_SAME_ACCESS) == 0)
          return (0);
        if (CloseHandle(hChildStdinWr) == 0)
          return (0);

        ZeroMemory(&pi, sizeof(pi));
        ZeroMemory(&si, sizeof(si));
        si.cb = sizeof(si);
        si.hStdError = hChildStdoutWr;
        si.hStdOutput = hChildStdoutWr;
        si.hStdInput = hChildStdinRd;
        si.dwFlags |= STARTF_USESTDHANDLES;
        LPVOID  environment = this->_createNewEnv(arguments, additional);

        if (CreateProcess(NULL, (LPSTR) path.c_str(), NULL, NULL, TRUE, 0, environment,  NULL, &si, &pi) == 0)
        {
          delete [] environment;
          return (0);
        }
        delete [] environment;
        if ((topipe != NULL) && (topipe->getSize() != 0))
          if (WriteFile(hChildStdinWrDup, topipe->getData(), topipe->getSize(), &dwWritten, NULL) == 0)
            goto end;
        if (CloseHandle(hChildStdinWrDup) == 0)
          goto end;
        if (CloseHandle(hChildStdoutWr) == 0)
          goto end;
        while ((ReadFile(hChildStdoutRdDup, chBuf, 4096, &dwRead, NULL) != 0) && (dwRead != 0))
          output.append(chBuf, dwRead);
        save->getBuffer()->setData(output);
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
        return (WaitForSingleObject(pi.hProcess, INFINITE));
end:
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
        delete [] environment;
        return (0);
      }

      //
      // Ajoute les parametres necessaires au CGI
      //

      string  CGI::_forgeEnv(ServerAnswer *sanswer)
      {
        string  arguments;

        // REQUEST_METHOD
        string  method(sanswer->GetRequest()->GetMethod());
        for (unsigned int i = 0; i < method.size(); i++)
          method[i] = toupper(method[i]);
        arguments += "REQUEST_METHOD=" + method + "&";
        // SCRIPT_FILENAME
        arguments += "SCRIPT_FILENAME=" + string(sanswer->GetPath()) + "&";
        // SERVER_SOFTWARE
        arguments += "SERVER_SOFTWARE=" + string((*sanswer)["Server"]) + "&";
        // SERVER_PROTOCOL
        arguments += "SERVER_PROTOCOL=" + string(sanswer->GetVersion()) + "&";
        // REQUEST_URI
        arguments += "REQUEST_URI=" + string(sanswer->GetRequest()->GetUrl()) + "&";
        // GATEWAY_INTERFACE
        arguments += "GATEWAY_INTERFACE=CGI/1.1&";
        // SERVER_PORT
        arguments += "SERVER_PORT=" + (*(*this->_xml)["general"].Get("port")) + "&";
        map<const char*, char*, mozia::utils::eqistr>::iterator  *iter;
        string  envname;
        string  name;
        while ((iter = sanswer->GetRequest()->getNextHeader()))
        {
          envname = "HTTP_";
          name = (*iter)->first;
          for (unsigned int i = 0; i < name.size(); i++)
          {
            name[i] = toupper(name[i]);
            if (name[i] == '-')
              name[i] = '_';
          }
          envname += name + "=" + string((*iter)->second) + "&";
          arguments += envname;
        }
        if (method == "POST")
        {
          // CONTENT_LENGTH
          arguments += "CONTENT_LENGTH=" + string((*sanswer->GetRequest())["Content-Length"]) + "&";
          // CONTENT_TYPE
          if ((*sanswer->GetRequest())["Content-type"] != NULL)
            arguments += "CONTENT_TYPE=" + string((*sanswer->GetRequest())["Content-type"]) + "&";
        }
        return (arguments);
      }

      //
      // Formate un Internal Server Error
      //

      void  CGI::_internalServerError(CgiSave *save)
      {
        string  out = "<!DOCTYPE HTML PUBLIC \"-//IETF//DTD HTML 2.0//EN\">\n"
          "<html><head>\n"
          "<title>500 Internal Server Error</title>\n"
          "</head><body>\n"
          "<h1>Internal Server Error</h1>\n"
          "<p>The server encountered an internal error or\n"
          "misconfiguration and was unable to complete\n"
          "your request.</p>\n"
          "<p>Please contact the server administrator,\n"
          " you@example.com and inform them of the time the error occurred,\n"
          "and anything you might have done that may have\n"
          "caused the error.</p>\n"
          "<p>More information about this error may be available\n"
          "in the server error log.</p>\n"
          "</body></html>\n";

        save->getServerAnswer()->addField("Content-Length", (int) out.size());
        save->getServerAnswer()->addField("Content-type", "text/html");
        save->getServerAnswer()->SetStatus(500);
        save->getServerAnswer()->SetMsg("Internal Server Error");
        Buffer  *buf = save->getServerAnswer()->Render();
        save->getBuffer()->Clear();
        save->getBuffer()->setData(*buf);
        save->getBuffer()->addData(out);
        save->setDelete();
      }

      //
      // Lance le CGI en detectant si c'est un binaire ou un script
      //

      void  CGI::_runCGI(CgiSave *save)
      {
        Reader  reader(save->getServerAnswer()->GetPath());
        reader.setChunkSize(256);
        Buffer  *buf = reader.getData();

        string env = this->_forgeEnv(save->getServerAnswer());
        if (env != "")
        {
          string  execline = this->_isAScript(buf->toString());
          if (execline == "")
            execline = save->getServerAnswer()->GetPath();
          else
            execline += " " + string(save->getServerAnswer()->GetPath());
          Buffer  *topipe = save->getContent();
          string querystring;
          if (save->getServerAnswer()->GetRequest()->GetArgs() == NULL)
            querystring = "";
          else
            querystring = "QUERY_STRING=" + string(save->getServerAnswer()->GetRequest()->GetArgs());
          this->_runProcess(save, execline, env, querystring, topipe);
        }
      }

      //
      // Bufferize l'ecriture
      //

      void  CGI::_bufferize(CgiSave *save)
      {
        int l = save->getBuffer()->getSize() > 4096 ? 4096 : save->getBuffer()->getSize();
        save->getTBuffer()->setData(save->getBuffer()->getData(), l);
        save->getBuffer()->Trunc(l);
        this->_th->CurTicket->CData = save->getTBuffer();
        if (save->getBuffer()->getSize() == 0)
          save->setDelete();
        this->_th->Drop("NETWORK", NETWORK_SENDDATA);
      }

      //
      // Post processing de l'output des CGI
      //

      void  CGI::_postProcess(CgiSave *save)
      {
        //string  output(this->_buf.toString());
        string  output(save->getBuffer()->toString());

        // verif de la presence de content-type, location et status
        size_t pos = output.find("\r\n\r\n", 0);
        if (pos == -1)
        {
          this->_internalServerError(save);
          return ;
        }
        output = output.substr(0, pos + 2);
        save->getBuffer()->Trunc((int) pos + 4);
        string contenttype;
        string location;
        string status;
        while ((pos = output.find_first_of("\r\n", 0)) != -1)
        {
          string tmp(output.substr(0, pos));
          if (ZiaString::casencmp(tmp.c_str(), "Content-type:", 13) == 0)
            contenttype = tmp;
          else
            if (ZiaString::casencmp(tmp.c_str(), "Status:", 7) == 0)
              status = tmp;
            else
              if (ZiaString::casencmp(tmp.c_str(), "Location:", 9) == 0)
                location = tmp;
          output = output.substr(pos + 2);
        }

        if ((contenttype == "") && (location == "") && (status == ""))
        {
          this->_internalServerError(save);
          return ;
        }

        save->getServerAnswer()->addField("Content-Length", save->getBuffer()->getSize());
        if (contenttype != "")
          save->getServerAnswer()->addField("Content-type", (char *) contenttype.substr(contenttype.find(":") + 1).c_str());
        if (status != "")
          save->getServerAnswer()->addField("Status", (char *) status.substr(status.find(":") + 1).c_str());
        if (location != "")
          save->getServerAnswer()->addField("Location", (char *) location.substr(location.find(":") + 1).c_str());

        save->getBuffer()->preprendData(*(save->getServerAnswer()->Render()));
      }

      //
      // Boucle principale
      //

      void  CGI::mainLoop(void)
      {
        Buffer  tbuf;

        while (this->_th->Get())
        {
          if (this->_th->CurTicket->GetAction() == CORE_HALT_MODULE)
          {
            this->_th->Drop("CORE", CORE_HALTED_MODULE);
            return ;
          }
          CgiSave *mydata = (CgiSave *) this->_th->CurTicket->GetMyData();
          if (mydata != NULL)
          {
            if (mydata->getDelete() == true)
            {
              this->_th->Drop("/dev/null", CORE_DELETE_TICKET);
              continue;
            }
            else
              if (mydata->getBuffer()->getSize() != 0)
              {
                this->_bufferize(mydata);
                continue;
              }
          }

          if ((this->_th->CurTicket->CData != NULL) && (this->_th->CurTicket->CData->GetType() == "ServerAnswer"))
          {
            ServerAnswer  *sanswer = (ServerAnswer *) this->_th->CurTicket->CData;
            CgiSave *save = new CgiSave(sanswer);
            this->_th->CurTicket->SetMyData(save);
            if (string(sanswer->GetRequest()->GetMethod()) == "post")
            {
              if ((*sanswer->GetRequest())["Content-Length"] == NULL)
              {
                this->_internalServerError(save);
                this->_bufferize(save);
              }
              else
              {
                this->_th->CurTicket->CData = NULL;
                this->_th->Drop("NETWORK", NETWORK_READ);
              }
            }
            else
            {
              this->_runCGI(save);
              this->_postProcess(save);
              this->_bufferize(save);
            }
          }
          else
            if ((this->_th->CurTicket->CData != NULL) && (this->_th->CurTicket->CData->GetType() == "Buffer"))
            {
              CgiSave *save = (CgiSave *) this->_th->CurTicket->GetMyData();
              Buffer  *buf = (Buffer *) this->_th->CurTicket->CData;
              save->addContent(*buf);
              buf->Clear();

              if (save->getContent()->getSize() == atoi((*save->getServerAnswer()->GetRequest())["Content-Length"]))
              {
                this->_runCGI(save);
                this->_postProcess(save);
                this->_bufferize(save);
              }
              else
              {
                this->_th->CurTicket->CData = NULL;
                this->_th->Drop("NETWORK", NETWORK_READ);
              }
            }
        }
      }
    }
  }
}
