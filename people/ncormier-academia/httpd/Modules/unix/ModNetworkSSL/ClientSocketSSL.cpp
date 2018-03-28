// Mozia - $Id: ClientSocketSSL.cpp 530 2006-04-04 13:34:18Z dak $


#include "ClientSocketSSL.h"


namespace mozia
{
  namespace network
  {
    ClientSocketSSL::ClientSocketSSL(SOCKET socket)
    {
      this->_socket = socket;
      this->_ticket = NULL;
      this->_flags = 0;
      this->_sslsocket = NULL;
    }

    ClientSocketSSL::~ClientSocketSSL(void)
    {
      SSL_shutdown(this->_sslsocket);
      SSL_free(this->_sslsocket);
      close(this->_socket);
    }

    SOCKET  ClientSocketSSL::getSocket(void)
    {
      return (this->_socket);
    }

    Buffer  &ClientSocketSSL::getBuf(void)
    {
      return (this->_buf);
    }

    Buffer  &ClientSocketSSL::getWBuf(void)
    {
      return (this->_wbuf);
    }

    Buffer  &ClientSocketSSL::getSBuf(void)
    {
      return (this->_sbuf);
    }

    const string&  ClientSocketSSL::GetType() const
    {
      static  string type = "ClientSocketSSL";
      return (type);
    }

    void  ClientSocketSSL::setTicket(Ticket *t)
    {
      this->_ticket = t;
    }

    Ticket  *ClientSocketSSL::getTicket(void)
    {
      return (this->_ticket);
    }

    void  ClientSocketSSL::_setFlag(int flag)
    {
      this->_flags |= flag;
    }

    void  ClientSocketSSL::_unsetFlag(int flag)
    {
      this->_flags &= ~flag;
    }

    int ClientSocketSSL::_getFlags(void)
    {
      return (this->_flags);
    }

    bool  ClientSocketSSL::_isFlagSet(int flag)
    {
      return ((this->_flags & flag) == flag);
    }
  }
}
