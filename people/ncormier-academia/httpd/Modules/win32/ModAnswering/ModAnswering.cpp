#include <windows.h>
#include <iostream>

#include "ModuleArg.h"
#include "Answering.h"

#include "Actions.h"

using namespace std;
using namespace mozia;
using namespace mozia::answering;

// Export this function
extern "C" __declspec(dllexport) void	entryPoint(void *toto);

// DLL initialization function
BOOL APIENTRY DllMain (HANDLE hModule, DWORD dwReason, LPVOID lpReserved) {
        return TRUE;
}

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