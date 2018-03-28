/*
**
**	$HeadURL: svn://svn.mozia.info/repo/zia/src/includes/Module.h $
**	$Author: nico $
**	$Revision: 687 $
**	$Date: 2006-04-19 23:11:07 +0200 (Wed, 19 Apr 2006) $
**
*/

#ifndef __MODULE_H__
# define __MODULE_H__


# include "Exceptions.h"

# include "Dll.h"
# include "Thread.h"
# include "Mutex.h"
# include "Semaphore.h"

# include "Ticket.h"
# include "TicketsHandler.h"

# include "XmlContainer.h"
# include "ModuleConf.h"

# include <iostream>
# include <stack>

using namespace std;
using namespace mozia;
using namespace mozia::ticket;
using namespace mozia::xmlconf;
using namespace mozia::exceptions;

namespace mozia
{
	namespace core
	{
		enum {
			HALTED,
			RUNNING,
			SUSPENDED,
			READY_TO_HALT
		};

		struct ModuleArg
		{
			TicketsHandler* th;
			XmlContainer*   xmlconf;
			const char*		config_file;
		};

		class Core;

		class Module
		{
			friend class Core;
		public:
			Module(Semaphore*, Mutex*, stack<Ticket*>*, ModuleConf*, XmlContainer*, const char*);
			~Module();
			void	Init();
			bool	Halt();
			bool	Restart();
			void	Start();
			bool	IsRunning();
			bool	PushTicket(Ticket *to_push);

		private:
			ModuleConf*			_modConf;
			Semaphore*			_coreSem;
			Mutex*				_coreMutex;
			stack<Ticket*>*		_coreStack;
			XmlContainer*		_xmlConf;
			unsigned int		_status;
			const char*			_conf_file;
			//
			// Acces au module
			//
			stack<Ticket *>		_ticketStack;
			Dll*				_dll;
			Thread*				_thread;
			Semaphore*			_sem;
			Mutex*				_mutex;
			TicketsHandler*		_curTH;
			ModuleArg*			_modArg;
			//
			// Suspend module
			//
			stack<Ticket *>		_tmpTicketStack;
			Ticket*				_lastIn;
			Ticket*				_lastOut;
		};

	}
}
#endif //__MODULE_H__
