// Mozia - $Id: Listener.cpp 685 2006-04-19 19:55:08Z dak $

#include <iostream>

#include "ClientSocket.h"
#include "Listener.h"
#include "Ticket.h"
#include "Actions.h"

using namespace mozia::ticket;

namespace mozia
{
  namespace network
  {
    Listener::Listener(unsigned short port, TicketsHandler *th)
    {
      WSADATA WSAData;

      if (WSAStartup(MAKEWORD(2, 2), &WSAData) != 0)
        throw "Winsock error";
      // checker le retour
      if (this->_openPort(port) == false)
        throw "Can't open port";
      this->_th = th;
    }

    Listener::~Listener(void)
    {
      this->_closeAll();
    }

    bool  Listener::_openPort(unsigned short port)
    {
      SOCKET		sock;
      SOCKADDR_IN	sin;

      sock = socket(AF_INET, SOCK_STREAM, 0);
      if (sock == INVALID_SOCKET)
        return (false);
      sin.sin_addr.s_addr = INADDR_ANY;
      sin.sin_family = AF_INET;
      sin.sin_port = htons(port);
      if (bind(sock, (SOCKADDR *) &sin, sizeof(sin)) == SOCKET_ERROR)
      {
        closesocket(sock);
        return (false);
      }
      if (listen(sock, SOMAXCONN) == SOCKET_ERROR)
      {
        closesocket(sock);
        return (false);
      }
      this->_ports[port] = sock;
      return (true);
    }

    bool  Listener::_closePort(unsigned short port)
    {
      closesocket(this->_ports[port]);
      this->_ports.erase(port);
      return (true);
    }

    void  Listener::_closeAll(void)
    {
      map<const unsigned short, SOCKET, mozia::utils::eqint>::iterator iter;
      iter = this->_ports.begin();
      while (iter != this->_ports.end())
      {
        closesocket(iter->second);
        iter = this->_ports.erase(iter);
      }
      WSACleanup();
    }

    inline void Listener::_selectPorts(void)
    {
      struct timeval  t;

      FD_ZERO(&this->_rfd);
      map<const unsigned short, SOCKET, mozia::utils::eqint>::iterator iter;
      iter = this->_ports.begin();
      while (iter != this->_ports.end())
      {
        FD_SET(iter->second, &this->_rfd);
        iter++;
      }
      t.tv_sec = 0;
      t.tv_usec = 50000;
      int r = select(0, &this->_rfd, NULL, NULL, &t);
      if (r == -1)
        throw "Select error";
      // check de 'r' + voir pour le timeout (interruption ?)
      iter = this->_ports.begin();
      while (iter != this->_ports.end())
      {
        if (FD_ISSET(iter->second, &this->_rfd))
        {
          SOCKADDR_IN addr;
          int addrlen = sizeof(addr);
          SOCKET s = accept(iter->second, (SOCKADDR *) &addr, &addrlen);
          // check valeur de retour
          if (s == -1)
            throw "Accept error\n";
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

      c->setTicket(t);
      t->CData = c;
      this->_th->Drop(t);
    }
  }
}
