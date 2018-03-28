
#include <iostream>

#include "ModuleArg.h"
#include "ClientRequest.h"
#include "ListDir.h"
#include "UrlEncoding.h"
#include "Actions.h"


using namespace std;
using namespace mozia;
using namespace mozia::protocol;
using namespace mozia::listdir;
using namespace mozia::modules::utils;

// Export this function
extern "C" void   entryPoint(void *toto);

void	entryPoint(void *ptr)
{
  TicketsHandler  *th;
  ListDir         *list = new ListDir();

  th = ((s_ModuleArg*) ptr)->th;
  while (1)
    {
      th->Get();
      if (th->CurTicket->GetAction() == CORE_HALT_MODULE)
	{
	  th->Drop("CORE", CORE_HALTED_MODULE);
	  return ;
	}
      list->Start((s_ModuleArg*) ptr);
    }
}
