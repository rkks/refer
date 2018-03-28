/*
**
**	$HeadURL: svn://epizia.homeip.net/repo/zia/src/includes/NetworkSSL.h $
**	$Author: nico $
**	$Revision: 300 $
**	$Date: 2005-11-24 12:06:19 +0100 (Thu, 24 Nov 2005) $
**
*/

#ifndef _NETWORKSSL_H_
# define _NETWORKSSL_H_

# include <map>
# include <list>
# include "utils/cmpfunc.h"
# include "ClientSocketSSL.h"
# include "TicketsHandler.h"

# include <openssl/ssl.h>

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
		typedef map<const SOCKET, ClientSocketSSL *, eqsock> Sock_CltSock_map;

		class NetworkSSL
		{
		public:
			NetworkSSL(TicketsHandler *th);
			~NetworkSSL(void);
			unsigned int				getNbClients(void);
			void						mainLoop(void);

		private:
			HANDLE						_hEvent;
			int							_nbrSocketSet;
			char						_ibuf[BUFSIZE];
			TicketsHandler*				_th;
			Sock_CltSock_map			_clients;
			list<Ticket*>				_tdelList;
			list<ClientSocketSSL*>			_toDelete;
			stack<ClientSocketSSL*>		_toSend;

			int							receiveFromClient(ClientSocketSSL*);
			int							sendToClient(ClientSocketSSL*);

			void						treatTickets(void);

			void						dropTicketRead(ClientSocketSSL*);
			void						dropTicketWrite(ClientSocketSSL*);

			void						closeClient(ClientSocketSSL*);
			void						closeAndDropClient(ClientSocketSSL*);
			bool						isBadClient(ClientSocketSSL*);
			
			void						treatFD(void);
			void						sendStack(void);
		};
	}
}

#endif /* !_NETWORKSSL_H_ */
