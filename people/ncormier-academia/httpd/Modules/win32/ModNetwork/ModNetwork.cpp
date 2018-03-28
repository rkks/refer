// Mozia - $Id: ModNetwork.cpp 687 2006-04-19 21:11:07Z nico $

#include "Network.h"
#include "ModuleArg.h"

#include <windows.h>
#include <iostream>

using namespace std;
using namespace mozia;
using namespace mozia::network;

// Export this function
extern "C" __declspec(dllexport) void	entryPoint(void *);

// DLL initialization function
BOOL APIENTRY DllMain (HANDLE hModule, DWORD dwReason, LPVOID lpReserved) {
	return TRUE;
}

void	entryPoint(void *ptr)
{
	s_ModuleArg* arg = (s_ModuleArg*) ptr;
	mozia::network::Network network(arg->th);

	network.mainLoop();
}
