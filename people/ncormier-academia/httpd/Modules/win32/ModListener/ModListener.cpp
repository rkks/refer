// Mozia - $Id: ModListener.cpp 682 2006-04-19 19:17:14Z dak $

#include <windows.h>
#include <iostream>

#include "ModuleArg.h"
#include "Listener.h"

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
  Listener  listener(atoi((*arg->xmlconf)["general"].Get("port")->c_str()), arg->th);

  listener.mainLoop();
}