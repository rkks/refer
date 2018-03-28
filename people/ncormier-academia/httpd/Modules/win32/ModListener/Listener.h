// Mozia - $Id: Listener.h 639 2006-04-18 15:57:43Z yann $

#ifndef __LISTENER_H__
# define __LISTENER_H__

# include <map>

# include "TicketsHandler.h"
# include "utils/cmpfunc.h"

using namespace std;
using namespace mozia::ticket;

namespace mozia
{
  namespace network
  {
    class Listener
    {
    public:
      Listener(unsigned short port, TicketsHandler *th);
      ~Listener(void);
      void  mainLoop(void);

    private:
      map<const unsigned short, SOCKET, mozia::utils::eqint>  _ports;
      fd_set  _rfd;
      TicketsHandler *_th;

      bool  _checkHalt(void);
      bool  _openPort(unsigned short port);
      bool  _closePort(unsigned short port);
      void  _selectPorts(void);
      void  _closeAll(void);
      void  _createClient(SOCKET socket);
      void  _checkOpenClose(void);
    };
  }
}

#endif // !__LISTENER_H__
