#include <windows.h>
#include <iostream>
#include <string>

#include "XmlContainer.h"
#include "ModuleArg.h"
#include "Protocol.h"

#include "Actions.h"

using namespace std;
using namespace mozia;
using namespace mozia::xmlconf;
using namespace mozia::protocol;

// Export this function
extern "C" __declspec(dllexport) void	entryPoint(void *toto);

// DLL initialization function
BOOL APIENTRY DllMain (HANDLE hModule, DWORD dwReason, LPVOID lpReserved) {
        return TRUE;
}

void	entryPoint(void *ptr)
{
	s_ModuleArg* arg = (s_ModuleArg*) ptr;
	Protocol prot;

	while (1)
	{
		arg->th->Get();
    if (arg->th->CurTicket->GetAction() == CORE_HALT_MODULE)
    {
      arg->th->Drop("CORE", CORE_HALTED_MODULE);
      return ;
    }
		prot.Start(arg->th);
	}
}