/*
**
**	$HeadURL: svn://svn.mozia.info/repo/zia/src/includes/Dll.h $
**	$Author: nico $
**	$Revision: 687 $
**	$Date: 2006-04-19 23:11:07 +0200 (Wed, 19 Apr 2006) $
**
*/

#ifndef __DLL_H__
# define __DLL_H__

# ifdef _WIN32
#  include <windows.h>
# else
#  include <dlfcn.h>
# endif

# include "IDll.h"
# include "ModuleConf.h"

using namespace mozia::xmlconf;

namespace mozia
{
	namespace core
	{
		class Dll : public IDll
		{
		public:
			Dll(ModuleConf *mod_conf);
			~Dll();
			s_procPt*	GetProcPoint();
		private:
#ifdef _WIN32
			HINSTANCE	_hinstLib;
#else
			void*       _handle;
#endif
			s_procPt*	_procPoint;
		};
	}
}

#endif //__DLL_H__
