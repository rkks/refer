// ModuleArg.h

#ifndef __MODULE_ARG_H__
#define __MODULE_ARG_H__

#include "XmlContainer.h"
#include "TicketsHandler.h"

using namespace mozia::ticket;
using namespace mozia::xmlconf;

namespace mozia
{
	struct s_ModuleArg
	{
    TicketsHandler* th;
    XmlContainer*   xmlconf;
    const char  *   config_file;
	};
}
#endif //__MODULE_ARG_H__
