
#include <iostream>

#include "ModuleArg.h"
#include "Answering.h"
#include "Actions.h"



using namespace std;
using namespace mozia;
using namespace mozia::answering;

// Export this function
extern "C" void   entryPoint(void *toto);

void	entryPoint(void *ptr)
{
  Answering*		ans = new Answering((s_ModuleArg*) ptr);

  while (1)
    {
      ans->arg->th->Get();
      if (ans->arg->th->CurTicket->GetAction() == CORE_HALT_MODULE)
	{
	  ans->arg->th->Drop("CORE", CORE_HALTED_MODULE);
	  return ;
	}
      ans->Start(ans->arg->th);
    }
}
