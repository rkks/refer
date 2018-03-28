#ifndef __FILEREQ_H__
#define __FILEREQ_H__

#include <iostream>
#include <map>

#include "Reader.h"
#include "ClientRequest.h"
#include "ServerAnswer.h"
#include "Ticket.h"
#include "ModuleArg.h"
#include "Reader.h"
#include "ServerAnswer.h"
#include "DBuffer.h"

using namespace mozia::ticket;
using namespace mozia::protocol;
using namespace mozia::answering;
using namespace	std;

namespace	mozia
{
  namespace	filereq
    {
      class FileReq
	{
	private:
	  ServerAnswer*   _answer;

	  void  OpenDoc(TicketsHandler &th, bool isanswer);

	public:
	  FileReq(s_ModuleArg * arg);
	  ~FileReq(void);
	  s_ModuleArg*	arg;
	  void	Start(TicketsHandler *th);
	};
    }
}

#endif //__FILEREQ_H__
