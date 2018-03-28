
#include <iostream>
#include <fstream>

#include <time.h>
#include <errno.h>

#include "Reader.h"
#include "FileReq.h"
#include "XmlContainer.h"
#include "ServerAnswer.h"
#include "DBuffer.h"
#include "OnFile.h"
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

    void  FileReq::OpenDoc(TicketsHandler &th, bool isanswer)
    {
      COnFile  *file;

      file = (COnFile *) th.CurTicket->GetMyData();
      if (!file && isanswer)
	{
	  if (this->_answer->GetPath() == NULL)
	    {
	      th.CurTicket->CData = this->_answer->Render();
	      return ;
	    }
	  file = new COnFile(new Reader(this->_answer->GetPath()));
	  file->SetRanges(this->_answer->GetRanges(),
			  this->_answer->GetNbRanges());
	  th.CurTicket->SetMyData(file);
	  th.CurTicket->CData = this->_answer->Render();
	  return ;
	}
      if (!file || file->GetReader().getError() != READER_ERROR_NOERROR)
        th.CurTicket->CData = NULL;
      else
        th.CurTicket->CData = file->GetData();
    }

    void	FileReq::Start(TicketsHandler *th)
    {
      bool  isanswer = false;

      if (!th->CurTicket->CData)
	{
	  th->Drop("/dev/null", CORE_DELETE_TICKET);
	  return ;
	}
      if (th->CurTicket->CData->GetType() == "ServerAnswer")
	{
	  this->_answer = (ServerAnswer *) th->CurTicket->CData;
	  isanswer = true;
	}
      this->OpenDoc(*th, isanswer);
      if (!th->CurTicket->CData)
	{
	  th->CurTicket->SetMyData(NULL);
	  // changer par nico CORE_DELETE_TICKET -> RELEASE_TICKET
	  th->Drop("/dev/null", CORE_DELETE_TICKET/*RELEASE_TICKET*/);
// 	  th->Drop("/dev/null", RELEASE_TICKET);
	}
      else
	th->Drop("NETWORK", NETWORK_SENDDATA);
    }
  }
}
