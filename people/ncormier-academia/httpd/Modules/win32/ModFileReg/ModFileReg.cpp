#include <windows.h>
#include <iostream>

#include "ModuleArg.h"
#include "FileReq.h"

#include "Actions.h"

using namespace std;
using namespace mozia;
using namespace mozia::filereq;

// Export this function
extern "C" __declspec(dllexport) void	entryPoint(void *toto);

// DLL initialization function
BOOL APIENTRY DllMain (HANDLE hModule, DWORD dwReason, LPVOID lpReserved) {
        return TRUE;
}

void	entryPoint(void *ptr)
{
	FileReq*		file = new FileReq((s_ModuleArg*) ptr);

	while (1)
	{
		file->arg->th->Get();
    if (file->arg->th->CurTicket->GetAction() == CORE_HALT_MODULE)
    {
      file->arg->th->Drop("CORE", CORE_HALTED_MODULE);
      return ;
    }
		file->Start(file->arg->th);
  }
}