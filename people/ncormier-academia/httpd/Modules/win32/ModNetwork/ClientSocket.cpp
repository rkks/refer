// Mozia - $Id: ClientSocket.cpp 687 2006-04-19 21:11:07Z nico $

#include "ClientSocket.h"

namespace mozia
{
	namespace network
	{
		ClientSocket::ClientSocket(SOCKET socket)
		{
			this->_socket = socket;
			this->_ticket = NULL;
			this->_flags = 0;
			this->_sslsocket = NULL;
		}

		ClientSocket::~ClientSocket(void)
		{
			closesocket(this->_socket);
		}

		SOCKET  ClientSocket::getSocket(void)
		{
			return (this->_socket);
		}

		Buffer  &ClientSocket::getBuf(void)
		{
			return (this->_buf);
		}

		Buffer  &ClientSocket::getWBuf(void)
		{
			return (this->_wbuf);
		}

		Buffer  &ClientSocket::getSBuf(void)
		{
			return (this->_sbuf);
		}

		const string&  ClientSocket::GetType() const
		{
			static  string type = "ClientSocket";
			return (type);
		}

		void  ClientSocket::setTicket(Ticket *t)
		{
			this->_ticket = t;
		}

		Ticket  *ClientSocket::getTicket(void)
		{
			return (this->_ticket);
		}

		void  ClientSocket::_setFlag(int flag)
		{
			this->_flags |= flag;
		}

		void  ClientSocket::_unsetFlag(int flag)
		{
			this->_flags &= ~flag;
		}

		int ClientSocket::_getFlags(void)
		{
			return (this->_flags);
		}

		bool  ClientSocket::_isFlagSet(int flag)
		{
			return ((this->_flags & flag) == flag);
		}
	}
}
