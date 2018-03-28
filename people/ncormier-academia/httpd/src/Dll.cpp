/*
**
**	$HeadURL: svn://svn.mozia.info/repo/zia/src/Dll.cpp $
**	$Author: nico $
**	$Revision: 687 $
**	$Date: 2006-04-19 23:11:07 +0200 (Wed, 19 Apr 2006) $
**
*/

#include <iostream>
#include <sstream>

#include "Dll.h"
#include "Exceptions.h"

#ifndef _WIN32
# include <errno.h>
# include <string.h>
#endif

using namespace std;
using namespace mozia::exceptions;

namespace mozia
{
	namespace core
	{
		Dll::Dll(ModuleConf *mod_conf)
		{
			cout << "DLL: " << mod_conf->GetPath().c_str() << endl;

#ifdef _WIN32
			this->_hinstLib = LoadLibrary(mod_conf->GetPath().c_str());
			if (this->_hinstLib == NULL)
			{
				ostringstream oss;
				oss << "Dll::Dll() ," << mod_conf->GetPath().c_str() << ", GetLastError=" << ::GetLastError();
				throw new DllLoadException(oss.str());
			}
			this->_procPoint = new s_procPt;
			this->_procPoint->entryPoint = (void* (*)(void *)) GetProcAddress(this->_hinstLib, "entryPoint");
			if (!this->_procPoint->entryPoint)
			{
				ostringstream oss;
				oss << "Dll::Dll() ," << mod_conf->GetPath().c_str() << ", GetLastError=" << ::GetLastError();
				throw new DllEntryPointException(oss.str());
			}
#else
			this->_handle = dlopen(mod_conf->GetPath().c_str(), RTLD_NOW);
			if (this->_handle == NULL)
			{
				ostringstream oss;
				oss << "Dll::Dll() ," << mod_conf->GetPath().c_str() << ", dlerror=" << dlerror();
				throw new DllLoadException(oss.str());
			}
			this->_procPoint = new s_procPt;
			this->_procPoint->entryPoint = (void* (*)(void *)) dlsym(this->_handle, "entryPoint");
			if (!this->_procPoint->entryPoint)
			{
				ostringstream oss;
				oss << "Dll::Dll() ," << mod_conf->GetPath().c_str() << ", dlerror=" << dlerror();
				throw new DllEntryPointException(oss.str());
			}
#endif
		}

		Dll::~Dll()
		{
			delete this->_procPoint;
#ifdef _WIN32
			if (FreeLibrary(this->_hinstLib) == NULL)
			{
				ostringstream oss;
				oss << "Dll::~Dll()  GetLastError=" << ::GetLastError();
				throw new DllUnloadException(oss.str());
			}
#else
			if (dlclose(this->_handle) == -1)
			{
				ostringstream oss;
				oss << "Dll::~Dll()  dlerror=" << dlerror();
				throw new DllUnloadException(oss.str());
			}
#endif
		}

		s_procPt*	Dll::GetProcPoint()
		{
			return (this->_procPoint);
		}

	}
}
