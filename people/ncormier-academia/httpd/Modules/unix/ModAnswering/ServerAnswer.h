#ifndef __SERVER_ANSWER_
#define __SERVER_ANSWER_

#include <iostream>

#include <map>
#include <string>

#include	"utils/cmpfunc.h"
#include	"DBuffer.h"
#include	"Reader.h"
#include	"IModuleData.h"
#include	"ClientRequest.h"

using namespace std;
using namespace mozia::protocol;
using namespace mozia::modules;
using namespace mozia::modules::utils;

namespace	mozia
{
  namespace	answering
    {
      enum  AnswerType
	{
	  REG_FILE,
	  CGI_BIN,
	  LIST_DIR,
	  HEAD
	};

      class ServerAnswer : public IModuleData, public ICData
	{
	private:
	  map<const char*, char*, mozia::utils::eqstr> _headers;
	  int     _status;
	  int     _fileError;
	  unsigned long long     _fileSize;
	  char*   _version;
	  char*   _cgi;
	  char*   _root;
	  char*   _msg;
	  char*   _path;
	  AnswerType  _atype;
	  Buffer	_buff;
	  bool    _valid;
	  bool    _isRange;
	  unsigned long long*    _getRange;
	  int     _nbRange;
	  ClientRequest *_request;

	public:
	  // Constructor
	  ServerAnswer(void);
	  ~ServerAnswer(void);

	  // Set variable
	  void  SetValid(bool isvalid);
	  void  SetType(AnswerType atype);
	  void	SetVersion(char *version);
	  void	SetPath(char *path);
	  void	SetRoot(char *path);
	  void	SetCGIPath(char *path);
	  void	SetStatus(int status);
	  void	SetFileSize(long long size);
	  void	SetFileError(int error);
	  void	SetRequest(ClientRequest *request);
	  void	SetMsg(char *msg);
	  void	addField(char *value, char *field);
	  void	addField(char *value, int field);
	  void	addField(char *value, unsigned long long field);
	  void  IsRange(bool is);
	  const string&  GetType() const;
	  bool	addRange(long long begin, long long end);
	  bool	addRange(const char *begin, const char *end);
	  void  changeRange(unsigned long long begin, unsigned long long end,
			    int nb);
	  void  delRange(int nb);

	  // Get variable
	  AnswerType  GetType();
	  char  *GetPath();
	  char  *GetRoot();
	  char  *GetCGIPath();
	  char	*GetVersion(void);
	  int		GetStatus(void);
	  bool  IsRange(void);
	  bool  GetValid(void);
	  bool	GetRange(long long &begin, long long &end, int nb);
	  unsigned long long*GetRanges();
	  unsigned int	GetNbRanges();
	  unsigned long long		GetFileSize(void);
	  int		GetFileError(void);
	  char	*GetMsg(void);
	  ClientRequest *GetRequest(void);
	  char*	operator[](char* key);	// returns the value corresponding to given key

	  char    MDate[32];
	  // functions
	  Buffer	*Render(void);
	};
    }
}

#endif // __SERVER_ANSWER_
