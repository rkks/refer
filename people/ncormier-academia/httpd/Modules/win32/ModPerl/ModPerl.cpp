// Mozia - $Id$

//#ifdef _DEBUG
//#include <stdlib.h>
//#define _CRTDBG_MAP_ALLOC
//#include <crtdbg.h>
//#endif

#include "ModuleArg.h"
#include "MPerl.h"

#include <windows.h>
#include <iostream>

#include "LeakWatcher.h"
#ifdef _DEBUG
# define new DEBUG_NEW
# undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace std;
using namespace mozia;
using namespace mozia::modules::mperl;

// Export this function
extern "C" __declspec(dllexport) void	entryPoint(void *);

// DLL initialization function
BOOL APIENTRY DllMain (HANDLE hModule, DWORD dwReason, LPVOID lpReserved) {
  return TRUE;
}

void	entryPoint(void *ptr)
{
  s_ModuleArg* arg = (s_ModuleArg*) ptr;
  MPerl mperl(arg->th, arg->xmlconf);

  mperl.MainLoop();
}
