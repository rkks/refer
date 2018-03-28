#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <io.h>
#include <stdio.h>
#include <windows.h>
#include <iostream>
#include <Winsock.h>

#include "ModuleArg.h"
#include "Actions.h"

using namespace std;
using namespace mozia;

// Export this function
extern "C" __declspec(dllexport) void	entryPoint(void *toto);

// DLL initialization function
/*BOOL APIENTRY DllMain (HANDLE hModule, DWORD dwReason, LPVOID lpReserved) {
        return TRUE;
}*/

void	entryPoint(void *ptr)
{
  struct stat  cur;
  struct stat  prev;
  bool          bReload = false;
  s_ModuleArg *args = (s_ModuleArg*) ptr;

  Sleep(100);
  if (stat(args->config_file, &prev) == -1)
    return ;
  while (1)
  {
    if (args->th->TryGet())
      if (args->th->CurTicket->GetAction() == CORE_HALT_MODULE && bReload)
      {
        args->th->Drop("CORE", CORE_HALTED_MODULE);
        return ;
      }
    if (stat(args->config_file, &cur) == -1)
      return ;
    if (memcmp(&prev, &cur, sizeof(cur)) && !bReload)
    {
      Ticket  *send = new Ticket("CONFIG", "CORE", "CONFIG", CORE_RELOAD_ALL);
      args->th->CurTicket = send;
      args->th->Drop();
      stat(args->config_file, &prev);
      bReload = true;
    }
    Sleep(10);
  }
}