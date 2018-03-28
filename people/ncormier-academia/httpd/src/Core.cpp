/*
**
**	$HeadURL: svn://svn.mozia.info/repo/zia/src/Core.cpp $
**	$Author: nico $
**	$Revision: 689 $
**	$Date: 2006-04-19 23:35:10 +0200 (Wed, 19 Apr 2006) $
**
*/

#include <iostream>

#include "Core.h"
#include "Exceptions.h"

#include "XmlParser.h"
#include "ModuleConf.h"

#include "Actions.h"

using namespace std;

using namespace mozia::xmlconf;
using namespace mozia::exceptions;

namespace mozia
{
	namespace core
	{

		Core::Core(CmdParser *args)
		{
			XmlParser xmlparser;

			this->_conf_file = args->GetOpt('c')->at(0)->c_str();
			this->_xmlConf = xmlparser.parse(this->_conf_file);
			xmlparser.append(this->_xmlConf, "config/codes.xml");
			xmlparser.append(this->_xmlConf, "config/mimetypes.xml");
			xmlparser.append(this->_xmlConf, "config/icons.xml");

			//
			// Create our Semaphore & Mutex
			//
			try
			{
				this->_sem = new Semaphore();
			}
			catch (const SemaphoreCreationException* ex)
			{
				throw new CoreException("Error during construction of a Semaphore object," + ex->Error);
			}
			try
			{
				this->_mutex = new Mutex();
			}
			catch (const MutexCreationException* ex)
			{
				throw new CoreException("Error during construction of a Mutex object, " + ex->Error);
			}
			//
			// Load Modules.
			//
			try
			{
				this->loadMods(this->_conf_file);
			}
			catch (const ModuleLoadException* ex)
			{
				throw new CoreException("Error during loading of a mandatory module (" + ex->Mod + "), " + ex->Error);
			}
			//
			// Start Modules.
			//
			try
			{
				this->startMods();
			}
			catch (const ModuleStartException* ex)
			{
				throw new CoreException("Error during starting of a mandatory module (" + ex->Mod + "), " + ex->Error);
			}
			//
			// Core Loop.
			//
			this->loop();
		}

		Core::~Core()
		{
			this->freeMods();
		}

		void	Core::loadMods(const char *conf_file)
		{
			vector<ModuleConf *> *mods_conf = ModuleConf::GetModulesConf(this->_xmlConf);
			for (int i = 0; i < (int) mods_conf->size(); i++)
			{
				Module *mod = NULL;

				try
				{
					mod = new Module(this->_sem, this->_mutex, &(this->_stack), mods_conf->at(i), this->_xmlConf, conf_file);
					this->_modList.push_back(mod);
				}
				catch (const DllLoadException* ex)
				{
					if (! ModuleConf::IsOptionalModule( mods_conf->at(i)) )
						throw new ModuleLoadException(mods_conf->at(i)->GetId(), "DllLoadException: " + ex->Error);
					else
					{
						cerr	<< "WARNING: Can't load: " << mods_conf->at(i)->GetId() << endl
								<< "DllLoadException: " + ex->Error << endl;
					}
				}
				catch (const DllEntryPointException* ex)
				{
					if (! ModuleConf::IsOptionalModule( mods_conf->at(i) ) )
						throw new ModuleLoadException(mods_conf->at(i)->GetId(), "DllEntryPointException: " + ex->Error);
					else
					{
						cerr	<< "WARNING: Can't load: " << mods_conf->at(i)->GetId() << endl
								<< "DllEntryPointException: " + ex->Error << endl;
					}
				}

			}
		}

		void	Core::startMods()
		{
			for (list<Module *>::iterator iter = this->_modList.begin();
				iter != this->_modList.end(); iter++)
			{
				try {
					(*iter)->Start();
					this->_modHash[(*iter)->_modConf->GetId()] = *iter;
				}
				catch (const ThreadJoiningException* ex) {
					if (! ModuleConf::IsOptionalModule((*iter)->_modConf) )
						throw new ModuleStartException((*iter)->_modConf->GetId(), "ThreadJoiningException: " + ex->Error);
					else
					{
						cerr	<< "WARNING: Can't start: " << (*iter)->_modConf->GetId() << endl
								<< "DllEntryPointException: " + ex->Error << endl;
						this->_modList.erase(iter);
					}
				}
			}
		}

		void  Core::stopMods()
		{
			bool  testHalt = true;

			for (map<string, Module *>::iterator iter = this->_modHash.begin();
				iter != this->_modHash.end(); iter++)
			{
				iter->second->_status = SUSPENDED;
				if (iter->second->_ticketStack.size() == 0)
				{
					Ticket  *send = new Ticket("CORE", iter->first, iter->first, CORE_HALT_MODULE);
					cerr << "Halting " << iter->first << endl;
					//
					// Waiting for system (WSA...).
					//
#ifndef _WIN32
					sleep(500);
#else
					Sleep(500);
#endif
					iter->second->PushTicket(send);
				}
			}
		}

		void  Core::reloadMods()
		{
			XmlParser xmlparser;
			map<const string, stack<Ticket *> > tmpStacks;
			stack<Ticket *> tmpStack;
			map<string, Module*>::iterator back;

			delete this->_xmlConf;
			this->_xmlConf = xmlparser.parse(this->_conf_file);
			xmlparser.append(this->_xmlConf, "config/codes.xml");
			xmlparser.append(this->_xmlConf, "config/mimetypes.xml");
			xmlparser.append(this->_xmlConf, "config/icons.xml");
			//
			// Waiting for system.
			//
#ifndef _WIN32
			sleep(5000);
#else
			Sleep(5000);
#endif

			for (map<string, Module*>::iterator iter = this->_modHash.begin();
				iter != this->_modHash.end();)
			{
				tmpStacks[iter->first] = iter->second->_tmpTicketStack;
				back = iter++;
				delete back->second;
				this->_modHash.erase(back);
			}
			this->_modHash.clear();
			this->_modList.clear();
			this->loadMods(this->_conf_file);
			this->startMods();
			for (map<string, Module*>::iterator iter = this->_modHash.begin();
				iter != this->_modHash.end(); iter++)
			{
				if (tmpStacks.find(iter->first) != tmpStacks.end())
				{
					tmpStack = tmpStacks[iter->first];
					while (tmpStack.size())
					{
						Ticket* tmp = tmpStack.top();
						tmpStack.pop();
						iter->second->PushTicket(tmp);
					}
				}
			}
		}

		void	Core::freeMods()
		{
			for (list<Module *>::const_iterator iter = this->_modList.begin();
				iter != this->_modList.end(); iter++)
			{
				(*iter)->Halt();
				delete *iter;
			}
		}

		bool  Core::getTicket(Ticket **cur)
		{
			if (!this->_sem->Wait())
				return (false);
			if (!this->_mutex->Lock())
				return (false);
			*cur = this->_stack.top();
			this->_stack.pop();
			if (!this->_mutex->UnLock())
				return (false);
			return (true);
		}
		//
		//	Treat Ticket.
		//
		bool  Core::treatTickets(Ticket* cur)
		{
			Module* ptr;

			//
			// Check reload notification.
			//
			if (cur->_action == CORE_RELOAD_ALL)
			{
				this->stopMods();
				return (false);
			}
			if (cur->_action == CORE_HALTED_MODULE)
			{
				if (this->_modHash.find(cur->_me) != this->_modHash.end())
				{
					ptr = this->_modHash[cur->_me];
					if (ptr->_status == SUSPENDED)
					{
						ptr->_lastOut = cur;
						ptr->_status = READY_TO_HALT;
						if (ptr->Halt())
						{
							bool  testHalt = true;
							for (list<Module *>::iterator iter = this->_modList.begin();
								iter != this->_modList.end(); iter++)
								if ((*iter)->_status != HALTED)
									testHalt = false;
							if (testHalt)
								this->reloadMods();
						}
						cur->_drop = false;
					}
				}
			}
			if (this->_modHash.find(cur->_me) != this->_modHash.end())
			{
				ptr = this->_modHash[cur->_me];
				if (ptr->_status == SUSPENDED)
				{
					ptr->_lastOut = cur;
					if (ptr->_ticketStack.size() == 0)
					{
						Ticket  *send = new Ticket("CORE", cur->_me, cur->_me, CORE_HALT_MODULE);
						ptr->PushTicket(send);
					}
					else if (cur->_from != "CORE")
						ptr->_tmpTicketStack.push(cur);
				}
			}
			//
			// Module want to get again the ticket.
			//
			if (cur->_drop)
				cur->_to = cur->_me;
			//
			// Check HTTPS
			//
			if (cur->_to == "NETWORK")
			{
				ICData* cData = cur->CData;

				if (cData && cData->GetType() == "ClientSocketSSL")
				{
					cur->_to = "NETWORKSSL";
				}
				else if (cur->_dataHash["NETWORKSSL"])
				{
					cur->_to = "NETWORKSSL";
				}
			}
			//
			// Push ticket to module which id is cur->_to.
			//
			if (this->_modHash.find(cur->_to) != this->_modHash.end())
			{
				if (cur->_to == "")
					return (false);
				ptr = this->_modHash[cur->_to];
				ptr->_lastIn = cur;

				cur->_from = cur->_me;
				cur->_me = cur->_to;
				cur->_to = "";
				cur->_drop = false;
				ptr->PushTicket(cur);
				return (true);
			}
			return (false);
		}
		//
		//	Clean Ticket.
		//
		void	Core::cleanTicket(Ticket* to_clean, string* keep, int nbr)
		{
			std::map<string, IModuleData*>::iterator back;
			for (std::map<string, IModuleData*>::iterator iter = to_clean->_dataHash.begin();
				iter != to_clean->_dataHash.end();)
			{
				int i = 0;
				for (; i < nbr; i++)
					if (keep[i] == iter->first)
						break;
				if (i < nbr)
				{
					iter++;
					continue;
				}
				IModuleData* ptr = iter->second;
				if (ptr)
					delete ptr;
				//TOFIX
				back = iter++;
				to_clean->_dataHash.erase(back);
			}
			to_clean->CData = NULL;
		}
		//
		// Delete Ticket.
		//
		bool	Core::delTicket(Ticket *to_del)
		{
			//return (true);
			if (to_del->_action == RELEASE_TICKET)
			{
				//
				// Clean ticket and give it to network.
				//
				string keep[2];
				keep[0] = "NETWORK";
				this->cleanTicket(to_del, keep, 1);
				to_del->_to = "NETWORK";
				to_del->_me = "PROTOCOL";
				to_del->_action = NETWORK_CLEAN;
				if (this->treatTickets(to_del))
					return (true);
			}

			if (to_del->_action == CORE_DELETE_TICKET)
			{
				to_del->_to = "NETWORK";
				to_del->_me = "CORE";
				if (this->treatTickets(to_del))
					return (true);
			}
			//
			// Delete ticket after cleaning it.
			//
			this->cleanTicket(to_del, NULL, 0);
			delete to_del;

			return (true);
		}
		//
		// Loop.
		//
		void	Core::loop()
		{
			for (Ticket* cur; true; )
			{
				if (!this->getTicket(&cur))
					throw new CoreException("Get Ticket Exception");
				if (!this->treatTickets(cur))
					this->delTicket(cur);
			}
		}

	}
}

