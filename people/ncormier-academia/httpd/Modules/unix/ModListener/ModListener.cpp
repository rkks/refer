// Mozia - $Id: ModListener.cpp 505 2006-03-20 11:07:13Z nico $

#include <iostream>

#include "ModuleArg.h"
#include "Listener.h"

using namespace std;
using namespace mozia;
using namespace mozia::network;

// Export this function
extern "C" void   entryPoint(void *toto);

void	entryPoint(void *ptr)
{
    try
    {
        s_ModuleArg* arg = (s_ModuleArg*) ptr;
        Listener  listener(atoi((*arg->xmlconf)["general"].Get("port")->c_str()), arg->th);

        listener.mainLoop();
    }
    catch (ZiaException* ziaEx)
    {
        cout << ziaEx->ToString() << endl;
        throw;
    }

}
