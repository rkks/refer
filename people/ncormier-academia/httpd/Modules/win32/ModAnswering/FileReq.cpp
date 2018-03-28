#ifdef _DEBUG
#define CRTDBG_MAP_ALLOC
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#endif


#include <iostream>
#include <fstream>

#include <time.h>
#include <errno.h>

#include "Reader.h"
#include "FileReq.h"
#include "XmlContainer.h"
#include "ServerAnswer.h"
#include "DBuffer.h"
#include "Actions.h"

using namespace	std;
using namespace mozia::answering;

namespace	mozia
{
  namespace	filereq
  {
    FileReq::FileReq(s_ModuleArg *arg)
    {
      this->arg = arg;
    }


    bool  FileReq::OpenDoc(TicketsHandler &th)
    {
      Reader  *file;

      file = (Reader *) th.CurTicket->GetMyData();
      if (!file)
      {
        if (this->_answer->GetPath() == NULL)
        {
          th.CurTicket->CData = this->_answer->Render();
          return (true);
        }
        file = new Reader(this->_answer->GetPath());
        th.CurTicket->SetMyData(file);
        th.CurTicket->CData = this->_answer->Render();
        return (true);
      }
      if (file->getError() != READER_ERROR_NOERROR)
        th.CurTicket->CData = NULL;
      else
        th.CurTicket->CData = file->getData();
      return (true);
    }

    void	FileReq::Start(TicketsHandler *th)
    {
      if (!th->CurTicket->CData)
      {
        th->Drop("/dev/null", CORE_DELETE_TICKET);
        return ;
      }
      if (th->CurTicket->CData->GetType() == "ServerAnswer")
        this->_answer = (ServerAnswer *) th->CurTicket->CData;
      if (!this->OpenDoc(*th))
      {
         th->Drop("/dev/null", CORE_DELETE_TICKET);
         return ;
      }
			if (!th->CurTicket->CData)
			{
				th->CurTicket->SetMyData(NULL);
				//TODO NOT BUTTED
				th->Drop("/dev/null", CORE_DELETE_TICKET);
				//th->Drop("PROTOCOL", PROTOCOL_NEWDATA);
			}
			else
				th->Drop("NETWORK", NETWORK_SENDDATA);
		}
	}
}