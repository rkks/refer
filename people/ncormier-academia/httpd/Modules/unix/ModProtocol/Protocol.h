# ifndef __PROTOCOL_H__
# define __PROTOCOL_H__

# include "Ticket.h"
# include "TicketsHandler.h"
# include "ClientRequest.h"
# include "DBuffer.h"

using namespace mozia::ticket;
using namespace mozia::modules::utils;

namespace mozia
{
	namespace protocol
	{
    class Protocol
		{
		public:
			void	ParseHeader();
			void	Start(TicketsHandler *tick);
			bool	IsSetTicket();
			Protocol();
			~Protocol();

		private:
			ClientRequest*  _request;
			Ticket*         _ticket;
			TicketsHandler* _hticket;

      void    rmBeforeEnd(Buffer &buf);
      bool    HeaderIsComplete(Buffer &buf);
			void		getOtherInfo(char *key, char *arg);
			char*		getLine(Buffer &buf);
			void		getFirstInfo(char *line);
		};
	}
}

#endif // __PROTOCOL_H__
