// Mozia - $Id: ClientSocket.h 527 2006-04-04 12:54:18Z dak $

#ifndef _CLIENTSOCKET_H_
# define _CLIENTSOCKET_H_

# include <string>

# include "DBuffer.h"
# include "Ticket.h"

# include "ICData.h"
# include "IModuleData.h"

# define SOCKET int

using namespace std;
using namespace mozia::modules;
using namespace mozia::modules::utils;
using namespace mozia::ticket;

namespace mozia
{
	namespace network
	{
		class ClientSocket : public IModuleData, public ICData
		{
			friend class Network;
		public:
			ClientSocket(SOCKET socket);
			~ClientSocket(void);
			SOCKET  getSocket(void);
			Buffer  &getBuf(void);
			Buffer  &getWBuf(void);
			Buffer  &getSBuf(void);
			void  setTicket(Ticket *t);
			Ticket  *getTicket(void);
			const string&  GetType() const;
      void  *_sslsocket;

		private:
			SOCKET  _socket;	// socket du client
			Buffer  _buf;		// buffer des donnees a traiter
			Buffer  _wbuf;		// donnees recues en attente
			Buffer  _sbuf;		// buffer a envoyer
			Ticket  *_ticket;	// ticket du client (jamais NULL)
# define CLIENT_HASTICKET           (1 << 0)
//# define CLIENT_HASDATAWAITING      (1 << 1)
//# define CLIENT_KILL				(1 << 2)
			int _flags;

			void  _setFlag(int flag);
			void  _unsetFlag(int flag);
			int _getFlags(void);
			bool _isFlagSet(int flag);
		};
	}
}

#endif /* !_CLIENTSOCKET_H_ */
