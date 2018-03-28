/*
**
**	$HeadURL: svn://epizia.homeip.net/repo/zia/src/includes/Network.h $
**	$Author: nico $
**	$Revision: 300 $
**	$Date: 2005-11-24 12:06:19 +0100 (Thu, 24 Nov 2005) $
**
*/

#ifndef _NETWORK_H_
# define _NETWORK_H_

# include <map>
# include <list>
# include "utils/cmpfunc.h"
# include "ClientSocket.h"
# include "TicketsHandler.h"

using namespace std;
using namespace mozia::ticket;

# define BUFSIZE  4096
# define WM_WSAASYNC (WM_USER +5)

struct eqsock
{
	bool operator()(const SOCKET a, const SOCKET b) const
	{
		return (a < b);
	}
};

namespace mozia
{
	namespace network
	{
		typedef map<const SOCKET, ClientSocket *, eqsock> Sock_CltSock_map;

		class Network
		{
		public:
			Network(TicketsHandler *th);
			~Network(void);
			unsigned int				getNbClients(void);
			void						mainLoop(void);

		private:
			HANDLE						_hEvent;
			int							_nbrSocketSet;
			char						_ibuf[BUFSIZE];
			TicketsHandler*				_th;
			Sock_CltSock_map			_clients;
			list<Ticket*>				_tdelList;
			list<ClientSocket*>			_toDelete;
			stack<ClientSocket*>		_toSend;

			int							receiveFromClient(ClientSocket*);
			int							sendToClient(ClientSocket*);

			void						treatTickets(void);

			void						dropTicketRead(ClientSocket*);
			void						dropTicketWrite(ClientSocket*);

			void						closeClient(ClientSocket*);
			void						closeAndDropClient(ClientSocket*);
			bool						isBadClient(ClientSocket*);
			
			void						treatFD(void);
			void						checkTimeout(void);
			void						sendStack(void);
		};
	}
}

#endif /* !_NETWORK_H_ */
