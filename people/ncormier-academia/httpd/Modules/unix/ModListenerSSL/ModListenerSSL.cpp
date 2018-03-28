// Mozia - $Id: ModListener.cpp 505 2006-03-20 11:07:13Z nico $

#include <iostream>

#include "ModuleArg.h"
#include "ListenerSSL.h"

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
        ListenerSSL  listenerSSL(atoi((*arg->xmlconf)["general"].Get("sslport")->c_str()), arg->th);

        listenerSSL.mainLoop();
    }
    catch (ZiaException* ziaEx)
    {
        cout << ziaEx->ToString() << endl;
        throw;
    }

}
