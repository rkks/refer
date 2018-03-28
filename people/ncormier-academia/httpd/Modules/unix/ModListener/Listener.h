// Mozia - $Id: Listener.h 505 2006-03-20 11:07:13Z nico $

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
      map<const unsigned short, int, mozia::utils::eqint>  _ports;
      fd_set  _rfd;
      TicketsHandler *_th;

      bool  _checkHalt(void);
      bool  _openPort(unsigned short port);
      bool  _closePort(unsigned short port);
      void  _selectPorts(void);
      void  _closeAll(void);
      void  _createClient(int socket);
      void  _checkOpenClose(void);
    };
  }
}

#endif // !__LISTENER_H__
