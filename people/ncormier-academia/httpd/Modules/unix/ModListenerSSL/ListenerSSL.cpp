// Mozia - $Id: Listener.cpp 505 2006-03-20 11:07:13Z nico $

#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>

#include "ClientSocketSSL.h"
#include "ListenerSSL.h"
#include "Ticket.h"
#include "Actions.h"

using namespace mozia::ticket;
typedef struct sockaddr_in    t_sockaddr_in;

namespace mozia
{
  namespace network
  {
    ListenerSSL::ListenerSSL(unsigned short port, TicketsHandler *th)
    {
      SSL_METHOD *method;

      SSL_library_init();
      SSL_load_error_strings();

      method = SSLv23_server_method();

      {struct sigaction act;
      act.sa_handler = SIG_IGN;
      sigemptyset (&act.sa_mask);
      act.sa_flags = 0;
      sigaction (SIGPIPE, &act, NULL);}

      this->_ctx = SSL_CTX_new(method);
      if (this->_ctx == NULL)
        throw "Can't create SSL context";

      // checker le retour
      if (this->_openPort(port) == false)
        throw new NetworkException("Can't open port", errno);
      this->_th = th;

      this->_LoadSSLData();
    }

    ListenerSSL::~ListenerSSL(void)
    {
      SSL_CTX_free(this->_ctx);
      this->_closeAll();
    }

    bool  ListenerSSL::_openPort(unsigned short port)
    {
      SOCKET		sock;
      t_sockaddr_in	sin;

      sock = socket(AF_INET, SOCK_STREAM, 0);
      if (sock == -1)
        return (false);
      sin.sin_addr.s_addr = INADDR_ANY;
      sin.sin_family = AF_INET;
      sin.sin_port = htons(port);
      // RELOU QUOI
      int relou = 1;
      if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &relou, sizeof(relou)) == -1)
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

    bool  ListenerSSL::_closePort(unsigned short port)
    {
      close(this->_ports[port]);
      this->_ports.erase(port);
      return (true);
    }

    void  ListenerSSL::_closeAll(void)
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

    inline void ListenerSSL::_selectPorts(void)
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

	  SSL *ssl = SSL_new(this->_ctx);
          SSL_set_fd(ssl, s);
          int ret = SSL_accept(ssl);
          if (ret == -1)
          {
            SSL_shutdown(ssl);
            SSL_free(ssl);
            close(s);
            continue;
          }

          this->_createClient(s, ssl);
        }
        iter++;
      }
    }

    void  ListenerSSL::_checkOpenClose(void)
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

    bool  ListenerSSL::_checkHalt(void)
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

    void  ListenerSSL::mainLoop(void)
    {
      while (1) // crado
      {
	if (this->_checkHalt())
          return ;
        this->_checkOpenClose();
        this->_selectPorts();
      }
    }

    void  ListenerSSL::_createClient(SOCKET socket, SSL *ssl)
    {
      ClientSocketSSL  *c = new ClientSocketSSL(socket);
      Ticket        *t = new Ticket;

      c->_sslsocket = ssl;
      c->setTicket(t);
      t->CData = c;
      this->_th->Drop(t);
    }

    void  ListenerSSL::_LoadSSLData(void)
    {
      char  *cert = "config/cert.cert";
      char  *pkey = "config/pkey.key";

      if (SSL_CTX_use_certificate_chain_file(this->_ctx, cert) <= 0)
        throw "Invalid certificate\n";

      if (SSL_CTX_use_PrivateKey_file(this->_ctx, pkey, SSL_FILETYPE_PEM) <= 0)
        throw "Invalid private key\n";

      if (!SSL_CTX_check_private_key(this->_ctx))
        throw "Wrong certificate/key\n";
    }
  }
}
