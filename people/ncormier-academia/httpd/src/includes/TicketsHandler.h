/*
**
**	$HeadURL: svn://epizia.homeip.net/repo/zia/src_common/includes/TicketsHandler.h $
**	$Author: nico $
**	$Revision: 512 $
**	$Date: 2006-03-20 18:52:32 +0100 (Mon, 20 Mar 2006) $
**
*/

#ifndef __TICKETSHANDLER_H__
#define __TICKETSHANDLER_H__

#include <stack>
#include <string>

#include "Ticket.h"
#include "Mutex.h"
#include "Module.h"
#include "Semaphore.h"

using namespace std;
using namespace mozia::core;
using namespace mozia::ticket;


namespace mozia
{
	namespace network
	{
		class Network;
		class NetworkSSL;
	}

	namespace ticket
	{
		class TicketsHandler
		{
			friend class mozia::network::Network;
			friend class mozia::network::NetworkSSL;
		public:
			TicketsHandler(Semaphore*, Mutex*, stack<Ticket*>*, Semaphore*, Mutex*, stack<Ticket*>*, const string&);
			bool			Get();
			bool			TryGet();
			bool			Drop();
			bool			Drop(Ticket*);
			bool			Drop(string, int);
			Ticket*			CurTicket;
		private:
			bool			forceGet();
			Mutex*			_outMutex;
			Semaphore*		_outSem;
			stack<Ticket*>*	_outStack;
			Mutex*			_inMutex;
			Semaphore*		_inSem;
			stack<Ticket*>*	_inStack;
			string			_modId;
			bool			_sleepin;
		};
	}
}

#endif /*__TICKETSHANDLER_H__*/
