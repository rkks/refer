// Mozia - $Id$

#include <windows.h>
#include <iostream>

#include "CGI.h"
#include "ModuleArg.h"

using namespace std;
using namespace mozia;
using namespace mozia::modules::cgi;

// Export this function
extern "C" __declspec(dllexport) void	entryPoint(void *toto);

// DLL initialization function
BOOL APIENTRY DllMain (HANDLE hModule, DWORD dwReason, LPVOID lpReserved) {
  return TRUE;
}

void	entryPoint(void *ptr)
{
  s_ModuleArg* arg = (s_ModuleArg*) ptr;
  CGI cgi(arg->th, arg->xmlconf);

  cgi.mainLoop();
}
