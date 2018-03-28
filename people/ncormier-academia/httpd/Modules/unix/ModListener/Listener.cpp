// Mozia - $Id: Listener.cpp 505 2006-03-20 11:07:13Z nico $

#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>

#include "ClientSocket.h"
#include "Listener.h"
#include "Ticket.h"
#include "Actions.h"

using namespace mozia::ticket;
typedef struct sockaddr_in    t_sockaddr_in;

namespace mozia
{
  namespace network
  {
    Listener::Listener(unsigned short port, TicketsHandler *th)
    {
      // checker le retour
      if (this->_openPort(port) == false)
        throw new NetworkException("Can't open port", errno);
      this->_th = th;
    }

    Listener::~Listener(void)
    {
      this->_closeAll();
    }

    bool  Listener::_openPort(unsigned short port)
    {
      SOCKET		sock;
      t_sockaddr_in	sin;

      sock = socket(AF_INET, SOCK_STREAM, 0);
      if (sock == -1)
        return (false);
      sin.sin_addr.s_addr = INADDR_ANY;
      sin.sin_family = AF_INET;
      sin.sin_port = htons(port);
      int enable = 1;
      if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &enable,
		     sizeof(enable)) == -1)
	{
	  close(sock);
	  return (false);
	}
      if (bind(sock, (const struct sockaddr *) &sin, sizeof(sin)) == -1)
      {
        close(sock);
        return (false);
      }
      if (listen(sock, SOMAXCONN) == -1)
      {
        close(sock);
        return (false);
      }
      this->_ports[port] = sock;
      return (true);
    }

    bool  Listener::_closePort(unsigned short port)
    {
      close(this->_ports[port]);
      this->_ports.erase(port);
      return (true);
    }

    void  Listener::_closeAll(void)
    {
      map<const unsigned short, SOCKET, mozia::utils::eqint>::iterator iter;
      map<const unsigned short, SOCKET, mozia::utils::eqint>::iterator back;
      iter = this->_ports.begin();
      while (iter != this->_ports.end())
      {
        close(iter->second);
	back = iter++;
        this->_ports.erase(back);
      }
    }

    inline void Listener::_selectPorts(void)
    {
      struct timeval  t;
      int nfds = -1;

      FD_ZERO(&this->_rfd);
      map<const unsigned short, SOCKET, mozia::utils::eqint>::iterator iter;
      iter = this->_ports.begin();
      while (iter != this->_ports.end())
      {
        FD_SET(iter->second, &this->_rfd);
	if (nfds < iter->second)
	  nfds = iter->second;
        iter++;
      }
      t.tv_sec = 0;
      t.tv_usec = 50000;
      int r = select(nfds + 1, &this->_rfd, NULL, NULL, &t);
      if (r == -1)
        throw new NetworkException("Select error", errno);
      // check de 'r' + voir pour le timeout (interruption ?)
      iter = this->_ports.begin();
      while (iter != this->_ports.end())
      {
        if (FD_ISSET(iter->second, &this->_rfd))
        {
          t_sockaddr_in addr;
          socklen_t addrlen = sizeof(addr);
          SOCKET s = accept(iter->second, (struct sockaddr *) &addr, &addrlen);
          // check valeur de retour
          if (s == -1)
            throw new NetworkException("Accept error", errno);
          this->_createClient(s);
        }
        iter++;
      }
    }

    void  Listener::_checkOpenClose(void)
    {
      while (this->_th->TryGet())
      {
        Ticket  *t = this->_th->CurTicket;
        if (t->GetAction() == LISTENER_OPENPORT)
        {
          if (t->CData->GetType() == "Buffer")
            this->_openPort(((Buffer *)t->CData)->getDataToInt());
          this->_th->Drop("", CORE_DELETE_TICKET);
          // checker retour
        }
        else
          if (t->GetAction() == LISTENER_CLOSEPORT)
          {
            if (t->CData->GetType() == "Buffer")
              this->_closePort(((Buffer *)t->CData)->getDataToInt());
            this->_th->Drop("", CORE_DELETE_TICKET);
            // checker retour
          }
      }
    }

    bool  Listener::_checkHalt(void)
    {
      while (this->_th->TryGet())
	{
	  Ticket  *t = this->_th->CurTicket;
	  if (t->GetAction() == CORE_HALT_MODULE)
	    {
	      this->_th->Drop("CORE", CORE_HALTED_MODULE);
	      return (true);
	    }
	}
      return (false);
    }

    void  Listener::mainLoop(void)
    {
      while (1) // crado
      {
	if (this->_checkHalt())
	  return ;
        this->_checkOpenClose();
        this->_selectPorts();
      }
    }

    void  Listener::_createClient(SOCKET socket)
    {
      ClientSocket  *c = new ClientSocket(socket);
      Ticket        *t = new Ticket;

	  //cout << t << endl;
      //t->SetMyData(c);
      c->setTicket(t);
      t->CData = c;
      this->_th->Drop(t);
    }
  }
}
