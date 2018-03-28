#ifdef _DEBUG
#define CRTDBG_MAP_ALLOC
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#endif


#include <windows.h>
#include <iostream>

#include "ModuleArg.h"
#include "FileReq.h"

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
		file->Start(file->arg->th);
  }
}