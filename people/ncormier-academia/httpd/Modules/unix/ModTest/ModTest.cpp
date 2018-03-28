#include <iostream>

#include "ModuleArg.h"

using namespace std;
using namespace mozia;

// Export this function
extern "C" void   entryPoint(void *toto);

void	entryPoint(void *ptr)
{
	s_ModuleArg* arg = (s_ModuleArg*) ptr;

	string test = "toto";

	cout << "HELLO :)" << endl;
	while (1)
	{
		arg->th->Get();
		arg->th->Drop("TEST", 0);
	}
}
