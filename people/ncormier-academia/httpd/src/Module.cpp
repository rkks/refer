/*
**
**	$HeadURL: svn://svn.mozia.info/repo/zia/src/Module.cpp $
**	$Author: nico $
**	$Revision: 687 $
**	$Date: 2006-04-19 23:11:07 +0200 (Wed, 19 Apr 2006) $
**
*/

#include "Module.h"

namespace mozia
{
	namespace core
	{

		Module::Module(	Semaphore *core_sem, Mutex* core_mutex, stack<Ticket*>* core_stack,
			ModuleConf *mod_conf, XmlContainer *xmlconf, const char *conf_file)
		{
			this->_thread = NULL;
			this->_dll = NULL;
			this->_curTH = NULL;
			this->_modArg = NULL;
			this->_sem = NULL;
			this->_lastOut = NULL;
			this->_lastIn = NULL;
			this->_coreSem = core_sem;
			this->_coreMutex = core_mutex;
			this->_coreStack = core_stack;
			this->_modConf = mod_conf;
			this->_xmlConf = xmlconf;
			this->_sem = new Semaphore();
			this->_mutex = new Mutex();
			this->_status = HALTED;
			this->_conf_file = conf_file;
			this->Init();
		}

		void  Module::Init()
		{
			if (this->_status != HALTED)
				throw new ModuleBadStatusException("Module isn't halted.");

			this->_dll = new Dll(this->_modConf);

			this->_curTH = new TicketsHandler(
				this->_coreSem,
				this->_coreMutex,
				this->_coreStack,
				this->_sem,
				this->_mutex,
				&(this->_ticketStack),
				this->_modConf->GetId()
				);
			this->_modArg = new ModuleArg;
			this->_modArg->th = this->_curTH;
			this->_modArg->xmlconf = this->_xmlConf;
			this->_modArg->config_file = this->_conf_file;

			this->_thread = new Thread(this->_dll->GetProcPoint(), (void*) this->_modArg);
		}

		Module::~Module()
		{
			if (this->_thread)
				delete this->_thread;
			if (this->_dll)
				delete this->_dll;
			if (this->_sem)
				delete this->_sem;
			if (this->_mutex)
				delete this->_mutex;
			delete this->_curTH;
			delete this->_modArg;
		}

		bool	Module::Halt()
		{
			if (this->_status == RUNNING)
			{
				this->_status = SUSPENDED;
				if (this->_lastIn != this->_lastOut)
					return (true);
				this->_status = READY_TO_HALT;
			}

			if (this->_status != READY_TO_HALT)
				return (false);

			if (this->_thread)
			{
				delete this->_thread;
				this->_thread = NULL;
			}

			if (this->_dll)
			{
				delete this->_dll;
				this->_dll = NULL;
			}

			delete this->_curTH;
			delete this->_modArg;
			this->_curTH = NULL;
			this->_modArg = NULL;

			this->_status = HALTED;
			return (true);
		}

		bool	Module::Restart()
		{
			/*
			if (!this->halt())
			return (false);
			try {
			this->init();
			}
			catch all... on fait quoi si ca marche pas ...
			catch () {
			return (false);
			}
			*/
			return (true);
		}

		void	Module::Start()
		{
			if (this->_thread == NULL)
				throw new ModuleBadStatusException("Module has not be inited.");
			if (this->_status != HALTED)
				throw new ModuleBadStatusException("Module isn't halted.");
			this->_thread->Join();
			this->_status = RUNNING;
		}

		bool	Module::IsRunning()
		{
			return ((this->_status == RUNNING));
		}

		bool	Module::PushTicket(Ticket *to_push)
		{
			if (this->_status == SUSPENDED && to_push->GetFrom() != "CORE")
			{
				this->_tmpTicketStack.push(to_push);
				return (true);
			}
			if (this->_status != RUNNING && to_push->GetFrom() != "CORE")
				return (false);

			if (!this->_mutex->Lock())
				return (false);
			this->_ticketStack.push(to_push);
			if (!this->_mutex->UnLock())
				return (false);

			if (!this->_sem->Post())
				return (false);
			return (true);
		}


	}
}
