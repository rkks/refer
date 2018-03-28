// Mozia - $Id: Listener.h 238 2005-11-14 22:07:27Z dak $

#ifndef __LISTENER_H__
# define __LISTENER_H__

# include <map>

# include "TicketsHandler.h"
# include "utils/cmpfunc.h"

# include <openssl/ssl.h>

using namespace std;
using namespace mozia::ticket;

namespace mozia
{
  namespace network
  {
    class ListenerSSL
    {
    public:
      ListenerSSL(unsigned short port, TicketsHandler *th);
      ~ListenerSSL(void);
      void  mainLoop(void);

    private:
      map<const unsigned short, SOCKET, mozia::utils::eqint>  _ports;
      fd_set  _rfd;
      TicketsHandler *_th;
      SSL_CTX *_ctx;

      bool  _checkHalt(void);
      bool  _openPort(unsigned short port);
      bool  _closePort(unsigned short port);
      void  _selectPorts(void);
      void  _closeAll(void);
      void  _createClient(SOCKET socket, SSL *);
      void  _checkOpenClose(void);
      void  _LoadSSLData(void);
    };
  }
}

#endif // !__LISTENER_H__
