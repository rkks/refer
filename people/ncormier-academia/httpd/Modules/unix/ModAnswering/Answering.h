#ifndef __ANSWERING_H__
#define __ANSWERING_H__

#include <iostream>
#include <map>

#include  "Reader.h"
#include  "ClientRequest.h"
#include  "ServerAnswer.h"
#include  "Ticket.h"
#include  "ModuleArg.h"
#include  "Reader.h"
#include  "ServerAnswer.h"
#include  "DBuffer.h"

using namespace mozia::ticket;
using namespace mozia::protocol;
using namespace mozia::answering;
using namespace	std;

namespace	mozia
{
  namespace	answering
    {
    enum  HeaderResponse
      {
	RespNone,
	Date,
	Server,
	Last_Modified,
	ETag,
	Accept_Ranges,
	Connection,
	Content_type,
	Content_Length,
	Content_Range,
	Allow
      };

    class Answering
      {
      private:
	ClientRequest*	_request;
	ServerAnswer*	_answer;

	void		SwitchHeader(HeaderResponse field);
	void		SwitchMethod();
	char*		content;
	char*		position;
	char*		getExtension(const char *name);
	bool		getStatus();
	void		DecodeContent(void);
	bool		chkRoot(const char *rpath, const char *path);
	void		chkDefaultPage(void);
	char		*getMsg(int status);
	bool		setRanges();
	void		Split(const string& str,
			      vector<string>& tokens,
			      const string& delimiters);
	map<const char *, char *, mozia::utils::eqstr>	*_mimeType;

      public:
	Answering(s_ModuleArg * arg);
	~Answering(void);
	s_ModuleArg*	arg;

      void	Start(TicketsHandler *th);
      };
    }
}

#endif //__ANSWERING_H__
