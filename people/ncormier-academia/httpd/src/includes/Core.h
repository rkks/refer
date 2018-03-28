/*
**
**	$HeadURL: svn://svn.mozia.info/repo/zia/src/includes/Core.h $
**	$Author: nico $
**	$Revision: 687 $
**	$Date: 2006-04-19 23:11:07 +0200 (Wed, 19 Apr 2006) $
**
*/

#ifndef __Core_H__
#define __Core_H__

#include <list>
#include <map>
#include <stack>

#include "Ticket.h"
#include "Module.h"
#include "CmdParser.h"
#include "IModuleData.h"
#include "XmlContainer.h"

using namespace std;
using namespace mozia::ticket;
using namespace mozia::xmlconf;
using namespace mozia::cmdparser;
using namespace mozia::modules;

namespace mozia
{
	namespace core
	{

		class Core
		{
		public:
			Core(CmdParser *args);
			~Core();
		private:
			//
			// Tickets.
			//
			bool	getTicket(Ticket**);
			bool	delTicket(Ticket*);
			void	cleanTicket(Ticket*, string*, int);
			bool	treatTickets(Ticket*);
			//
			// Core loop.
			//
			void	loop();
			//
			// Modules.
			//
			void	loadMods(const char *);
			void	startMods();
			void	freeMods();
			void	stopMods();
			void	reloadMods();

			XmlContainer*				_xmlConf;
			list<Module *>				_modList;
			std::map <string, Module*>	_modHash;
			Semaphore*					_sem;
			Mutex*						_mutex;
			stack<Ticket*>				_stack;
			const char*					_conf_file;
		};

	}
}
#endif //__Core_H__
