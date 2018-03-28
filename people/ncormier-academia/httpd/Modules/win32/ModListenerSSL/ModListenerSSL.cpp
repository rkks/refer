// Mozia - $Id: ModListener.cpp 408 2005-12-13 00:05:20Z dak $

#include <windows.h>
#include <iostream>

#include "ModuleArg.h"
#include "ListenerSSL.h"

using namespace std;
using namespace mozia;
using namespace mozia::network;

// Export this function
extern "C" __declspec(dllexport) void	entryPoint(void *toto);

// DLL initialization function
BOOL APIENTRY DllMain (HANDLE hModule, DWORD dwReason, LPVOID lpReserved) {
  return TRUE;
}

void	entryPoint(void *ptr)
{
  s_ModuleArg* arg = (s_ModuleArg*) ptr;
  ListenerSSL  listenerssl(atoi((*arg->xmlconf)["general"].Get("sslport")->c_str()), arg->th);

  listenerssl.mainLoop();
}