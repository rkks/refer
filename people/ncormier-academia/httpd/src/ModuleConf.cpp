/*
**
**	$HeadURL: svn://svn.mozia.info/repo/zia/src/ModuleConf.cpp $
**	$Author: nico $
**	$Revision: 544 $
**	$Date: 2006-04-11 17:23:05 +0200 (Tue, 11 Apr 2006) $
**
*/

#include <iostream>

#include "ModuleConf.h"


using namespace std;

namespace mozia
{
	namespace xmlconf
	{
		ModuleConf::ModuleConf(const string &id, const string &path, const string &catch_c, const string &to, const string &prio)
		{
			this->_id = id;
			this->_path = path;
			this->_catch = catch_c;
			this->_to = to;
			this->_prio = atoi(prio.c_str());
		}

		ModuleConf::~ModuleConf()
		{
		}

		const string&	ModuleConf::GetId()
		{
			return (this->_id);
		}

		const string&	ModuleConf::GetPath()
		{
			return (this->_path);
		}

		const string&	ModuleConf::GetCatch()
		{
			return (this->_catch);
		}

		const string&	ModuleConf::GetTo()
		{
			return (this->_to);
		}

		const int&	ModuleConf::GetPrio()
		{
			return (this->_prio);
		}

		vector<ModuleConf *>*	ModuleConf::GetModulesConf(XmlContainer* _xmlConf)
		{
			vector<ModuleConf *>* v = new vector<ModuleConf *>;
			XmlContainer &modules = (*_xmlConf)["modules"];

			map_next::iterator iter = modules.ChildBegin();
			while (iter != modules.ChildEnd())
			{
				ModuleConf *mod = new ModuleConf(iter->first,
					*iter->second->Get("path"),
					*iter->second->Get("catch"),
					*iter->second->Get("to"),
					*iter->second->Get("prio"));
				v->insert(v->end(), mod);
				iter++;
			}
			return (v);
		}

		bool	ModuleConf::IsOptionalModule(ModuleConf *mod)
		{
			if (mod->GetId() == "PROTOCOL"	||
				mod->GetId() == "LISTENER"	||
				mod->GetId() == "NETWORK"	||
				mod->GetId() == "ANSWERING"	)
				return (false);
			return (true);
		}


	}
}
