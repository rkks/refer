// Mozia - $Id: ModNetwork.cpp 505 2006-03-20 11:07:13Z nico $


#include "NetworkSSL.h"
#include "ModuleArg.h"

#include <iostream>

using namespace std;
using namespace mozia;
using namespace mozia::network;

// Export this function
extern "C" void   entryPoint(void *toto);

void	entryPoint(void *ptr)
{
  s_ModuleArg* arg = (s_ModuleArg*) ptr;
  mozia::network::NetworkSSL networkSSL(arg->th);

  networkSSL.mainLoop();
}
