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
      unsigned __int64     _fileSize;
			char*   _version;
      char*   _cgi;
      char*   _root;
			char*   _msg;
      char*   _path;
      AnswerType  _atype;
			Buffer	_buff;
			bool    _valid;
      bool    _isRange;
      unsigned __int64*    _getRange;
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
      void	SetFileSize(__int64 size);
      void	SetFileError(int error);
      void	SetRequest(ClientRequest *request);
			void	SetMsg(char *msg);
			void	addField(char *value, char *field);
			void	addField(char *value, int field);
      void	addField(char *value, unsigned __int64 field);
      void  IsRange(bool is);
      const string&  GetType() const;
      bool	addRange(__int64 begin, __int64 end);
      bool	addRange(const char *begin, const char *end);
      void  changeRange(unsigned __int64 begin, unsigned __int64 end, int nb);
      void  delRange(int nb);
      unsigned __int64	GetResponseSize(void);

			// Get variable
      AnswerType  GetType();
      char  *GetPath();
      char  *GetRoot();
      char  *GetCGIPath();
			char	*GetVersion(void);
			int		GetStatus(void);
      bool  IsRange(void);
      bool  GetValid(void);
      bool	GetRange(__int64 &begin, __int64 &end, int nb);
      unsigned __int64	  *GetRanges();
      unsigned int	GetNbRanges();
      unsigned __int64		GetFileSize(void);
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
