#ifdef _DEBUG
#include <stdlib.h>
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif

#include <windows.h>
#include <iostream>

#include "ModuleArg.h"

using namespace std;
using namespace mozia;

// Export this function
extern "C" __declspec(dllexport) void	entryPoint(void *toto);

// DLL initialization function
BOOL APIENTRY DllMain (HANDLE hModule, DWORD dwReason, LPVOID lpReserved) {
        return TRUE;
}

void	entryPoint(void *ptr)
{
	s_ModuleArg* arg = (s_ModuleArg*) ptr;
	
	string test = "toto";

	cout << "HELLO :)" << endl;
	while (1)
	{
		arg->th->Get();
		arg->th->Drop("TEST", 0);
	}
}