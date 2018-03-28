// Mozia - $Id: ModNetwork.cpp 402 2005-12-12 13:25:45Z nico $

#include "NetworkSSL.h"
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
  mozia::network::NetworkSSL networkssl(arg->th);

  networkssl.mainLoop();
}
