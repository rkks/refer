/*
**
**	$HeadURL: svn://svn.mozia.info/repo/zia/src/includes/ModuleConf.h $
**	$Author: nico $
**	$Revision: 241 $
**	$Date: 2005-11-15 17:44:39 +0100 (Tue, 15 Nov 2005) $
**
*/

#ifndef __MODULECONF_H__
# define __MODULECONF_H__

# include <string>
# include <vector>

# include "XmlContainer.h"

using namespace std;

namespace mozia
{
	namespace xmlconf
	{
		class ModuleConf
		{
		public:
			ModuleConf(const string &id, const string &path, const string &catch_c, const string &to, const string &prio);
			~ModuleConf();
			const string&	GetId();
			const string&	GetPath();
			const string&	GetEntryPoint();
			const string&	GetCatch();
			const string&	GetTo();
			const int&		GetPrio();
			static vector<ModuleConf *>*	ModuleConf::GetModulesConf(XmlContainer* _xmlConf);
			static bool		IsOptionalModule(ModuleConf *);
		private:
			string	      _id;
			string	      _path;
			string	      _entryPoint;
			string	      _catch;
			string	      _to;
			int	          _prio;
		};

		
	}
}
#endif //__MODULECONF_H__
