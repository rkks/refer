/*
**
**	$HeadURL: svn://epizia.homeip.net/repo/zia/src_common/TicketsHandler.cpp $
**	$Author: nico $
**	$Revision: 512 $
**	$Date: 2006-03-20 18:52:32 +0100 (Mon, 20 Mar 2006) $
**
*/

#include <iostream>

#include "TicketsHandler.h"

namespace mozia
{
	namespace ticket
	{

		TicketsHandler::TicketsHandler(	Semaphore* Core_sem, Mutex* Core_mutex, stack<Ticket*>* out,
										Semaphore* mod_sem, Mutex* mod_mutex, stack<Ticket*>* in,
										const string& mod_name)
		{
			this->_inSem = mod_sem;
			this->_outSem = Core_sem;
			this->_inMutex = mod_mutex;
			this->_outMutex = Core_mutex;
			this->_outStack = out;
			this->_inStack = in;
			this->_modId = mod_name;
		}

		bool   TicketsHandler::Get()
		{
			if (!this->_inSem->Wait())
				return (false);
			return this->forceGet();
		}

		bool   TicketsHandler::TryGet()
		{
			if (!this->_inSem->TryWait())
				return (false);
			return this->forceGet();
		}

		bool	TicketsHandler::forceGet()
		{
			if (!this->_inMutex->Lock())
				return (false);
			this->CurTicket = this->_inStack->top();
			this->_inStack->pop();
			if (!this->_inMutex->UnLock())
				return (false);
			return  (true);
		}

		bool      TicketsHandler::Drop(Ticket* newT)
		{
			newT->_ttl++;

			if (!this->_outMutex->Lock())
				return (false);
			this->_outStack->push(newT);
			if (!this->_outMutex->UnLock())
				return (false);

			if (!this->_outSem->Post())
				return (false);
			return (true);
		}

		bool      TicketsHandler::Drop()
		{
			this->CurTicket->_ttl++;
			this->CurTicket->_from = this->_modId;
			this->CurTicket->_drop = true;

			if (!this->_outMutex->Lock())
				return (false);
			this->_outStack->push(this->CurTicket);
			if (!this->_outMutex->UnLock())
				return (false);

			if (!this->_outSem->Post())
				return (false);
			return (true);
		}

		bool      TicketsHandler::Drop(string to, int action)
		{
			this->CurTicket->_ttl++;
			this->CurTicket->_to = to;
			this->CurTicket->_action  = action;

			if (!this->_outMutex->Lock())
				return (false);
			this->_outStack->push(this->CurTicket);
			if (!this->_outMutex->UnLock())
				return (false);

			if (!this->_outSem->Post())
				return (false);
			return (true);
		}
	}
}
