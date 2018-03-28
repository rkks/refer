
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <iostream>

#include "ModuleArg.h"
#include "Actions.h"

using namespace std;
using namespace mozia;

// Export this function
extern "C" void   entryPoint(void *toto);

void	entryPoint(void *ptr)
{
  struct stat  cur;
  struct stat  prev;
  bool bReload = false;
  s_ModuleArg *args = (s_ModuleArg*) ptr;

  if (stat(args->config_file, &prev) == -1)
    throw "Stat error\n";
  while (1)
  {
    if (args->th->TryGet())
      if (args->th->CurTicket->GetAction() == CORE_HALT_MODULE)
	{
	  args->th->Drop("CORE", CORE_HALTED_MODULE);
	  return ;
	}
    if (stat(args->config_file, &cur) == -1)
      throw "Stat error\n";
    if (memcmp(&prev, &cur, sizeof(cur)) && !bReload)
    {
      Ticket  *send = new Ticket("CONFIG", "CORE", "CONFIG", CORE_RELOAD_ALL);
      args->th->CurTicket = send;
      args->th->Drop();
      stat(args->config_file, &prev);
      bReload = true;
    }
    usleep(10);
  }
}
