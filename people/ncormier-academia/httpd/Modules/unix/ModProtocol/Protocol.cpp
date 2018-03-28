
#include <iostream>

#include "Protocol.h"
#include "DBuffer.h"
#include "Actions.h"

using namespace std;
using namespace mozia;
using namespace mozia::ticket;
using namespace mozia::utils;

namespace mozia
{
  namespace protocol
  {
    Protocol::Protocol()
    {
      this->_ticket = 0x0;
    }

    Protocol::~Protocol()
    {
    }

    bool	Protocol::IsSetTicket()
    {
      return (this->_ticket != 0x0);
    }

    bool		Protocol::HeaderIsComplete(Buffer &buf)
    {
      int		idx;
      bool  isother;
      int		bsize;
      int		count;
      char*	header;

      bsize = buf.getSize();
      isother = false;
      if (this->IsSetTicket() == true)
	{
	  header = (char *) buf.getData();
	  for (idx = 0, count = 0; idx < bsize; idx++)
	    {
	      if (header[idx] == '\n')
		count++;
	      else if (header[idx] != '\r')
		{
		  count = 0;
		  isother = true;
		}
	      // ICI ISOTHER JAMAIS A TRUE LORS KEEP ALIVE
	      // YANN !!!!
	      if (count == 2 && isother)
		return (true);
	    }
	  return (false);
	}
      return (false);
    }

    void		Protocol::getFirstInfo(char *line)
    {
      char*	str;

      if ((str = strchr(line, ' ')))
        *str = '\0';
      this->_request->SetMethod(line);
      if (str)
        line = ++str;
      if (*line && (str = strchr(line, ' ')))
	{
	  *str = '\0';
	  this->_request->SetUrl(line);
	  line = ++str;
	}
      if (*line)
        this->_request->SetVersion(line);
    }

    void	Protocol::getOtherInfo(char *key, char *arg)
    {
      this->_request = (ClientRequest *) this->_ticket->GetMyData();
      if (!this->_request)
	{
	  this->_hticket->Drop(this->_ticket);
	}
      this->_request->addField(ZiaString::dup(key), ZiaString::dup(arg));
    }

    char*				Protocol::getLine(Buffer &buf)
    {
      int				i = 0;
      int				bsize = buf.getSize();
      char*				header;
      static char*			line = 0;

      if (line)
        delete [] line;
      line = 0;
      header = (char*)buf.getData();
      while (i <= bsize && header[i] != '\n' && header[i] != '\r')
        i++;
      if (i > bsize)
        return (0);
      if (i <= 0)
	{
	  buf.Trunc(1);
	  return (0);
	}
      line = new char[i + 1];
      memcpy(line, header, i);
      line[i] = '\0';
      if (strstr(header + i, "\r\n"))
        buf.Trunc(i + 2);
      else
        buf.Trunc(i + 1);
      return (line);
    }

    void    Protocol::rmBeforeEnd(Buffer &buf)
    {
      char  *ret = buf.getData();
      int   count;
      int   i;

      count = 0;
      for (i = 0; i < buf.getSize() && count < 2; i++)
	{
	  if (ret[i] == '\n')
	    count++;
	  else if (ret[i] != '\r')
	    count = 0;
	}
      buf.Trunc(i);
    }

    void      Protocol::ParseHeader()
    {
      char*   field;
      char*   value;

      if (this->_ticket->CData &&
	  this->_ticket->CData->GetType() == "Buffer")
	{
	  this->_request = (ClientRequest *) this->_ticket->GetMyData();
	  if (!this->_request)
	    {
	      this->_request = new ClientRequest();
	      this->_ticket->SetMyData(this->_request);
	    }
	  Buffer &buf = this->_request->GetBuff();
	  buf.addData(*((Buffer *) this->_ticket->CData));
	  if ((this->IsSetTicket() == true) && (buf.getSize() != 0))
	    {
	      if (this->HeaderIsComplete(buf) == true)
		{
		  while (!(field = this->getLine(buf)))
		    ;
		  getFirstInfo(field);
		  while ((field = this->getLine(buf)) != 0)
		    {
		      if (!(*field))
			break ;
		      value = strchr(field, ':');
		      if (value)
			{
			  *value = '\0';
			  while (*(++value) == ' ' || *value == '\t')
			    ;
			  getOtherInfo(field, value);
			}
		    }
		  rmBeforeEnd(buf);
		  rmBeforeEnd(*((Buffer *) this->_ticket->CData));
		  this->_request->SetHost((*this->_request)["host"]);
		  /*printf("Method : %s\n", this->_request->GetMethod());
		    printf("Url : %.40s\n", this->_request->GetUrl());
		    printf("Version : %s\n", this->_request->GetVersion());*/
		  this->_ticket->CData = this->_request;
		  this->_hticket->Drop("ANSWERING", ANSWERING_IDROPTOYOU);
		}
	      else
		{
		  ((Buffer *) this->_ticket->CData)->Clear();
		  this->_hticket->Drop(this->_ticket->GetFrom(), NETWORK_READ);
		}
	    }
	  return ;
	}
      this->_hticket->Drop("NETWORK", NETWORK_READ);
    }

    void	Protocol::Start(TicketsHandler *tick)
    {
      this->_ticket = tick->CurTicket;
      this->_hticket = tick;
      this->ParseHeader();
    }
  }
}
