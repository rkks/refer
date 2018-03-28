#ifndef __REQUEST_
# define __REQUEST_

# include <iostream>

# include <map>
# include <string>

# include "utils/cmpfunc.h"
# include "IModuleData.h"
# include "DBuffer.h"
# include "ICData.h"

using namespace std;
using namespace mozia::ticket;
using namespace mozia::modules;
using namespace mozia::modules::utils;

namespace mozia
{
	namespace protocol
	{
	  class				ClientRequest : public IModuleData, public ICData
		{

		private:
		  char*	_method;
		  char*	_host;
		  char*	_url;
		  char* _args;
		  char*	_version;
		  Buffer  _buff;
		  map<const char*, char*, mozia::utils::eqistr> _headers;
		  map<const char*, char*, mozia::utils::eqistr>::iterator  _headersiter;

		public:

			// Constructor / Destructor
			ClientRequest(void);
      ~ClientRequest(void);

			// Set fields
			void		SetMethod(char* method);
      Buffer  &GetBuff();
			void		SetHost(char* host);
			void		SetUrl(char* url);
			void		SetVersion(char* query);
      void    SetArgs(char* args);

			// Get fields value
      char* GetArgs();
			char*	GetMethod();
			char*	GetHost();
			char*	GetUrl();
			char*	GetVersion();
      const string&  GetType() const;
			char*	operator[](char* key);	// returns the value corresponding to given key
      map<const char*, char*, mozia::utils::eqistr>::iterator  *getNextHeader(void); // get the next header value

			// Check if fields are set
			bool		isSetUrl();
			bool		isSetHost();
			bool		isSetVersion();
			bool		isSetMethod();

			// List management
			void		addField(char* key, char* value);
			bool		delField(char* key);
		};
	}
}

# endif // __REQUEST_
