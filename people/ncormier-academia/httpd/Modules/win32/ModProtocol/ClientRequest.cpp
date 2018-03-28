//
// ClientRequest.cpp for  in /u/ept3/rimbea_n/proj/zia
// 
// Made by nicolas rimbeau
// Login   <rimbea_n@epita.fr>
// 
// Started on  Thu Oct 13 16:26:43 2005 nicolas rimbeau
// Last update Thu Oct 20 14:57:55 2005 Nicolas Rimbeau
//

#include "ClientRequest.h"
#include "ZiaString.h"

using namespace std;
using namespace mozia;

namespace mozia
{
	namespace protocol
	{
		ClientRequest::ClientRequest()
		{
			this->_method = 0x0;
			this->_host = 0x0;
			this->_url = 0x0;
			this->_version = 0x0;
			this->_args = 0x0;
		}

		ClientRequest::~ClientRequest()
		{
			if (this->_method)
				delete [] this->_method;
      this->_method = NULL;
			if (this->_url)
				delete [] this->_url;
      this->_url = NULL;
			if (this->_version)
				delete [] this->_version;
      this->_version = NULL;
      if (this->_host)
        delete [] this->_host;
      this->_host = NULL;
      if (this->_args)
        delete [] this->_args;
      this->_args = NULL;
			map<const char *, char *, mozia::utils::eqistr>::iterator iter;
			iter = this->_headers.begin();
			while (iter != this->_headers.end())
			{
        delete [] iter->first;
				delete [] iter->second;
        iter = this->_headers.erase(iter);
			}
		}

    Buffer  &ClientRequest::GetBuff()
    {
       return (this->_buff);
    }

		void	ClientRequest::SetMethod(char *value)
		{
			int	i;

      if (!value)
        return ;
			for (i = 0; value[i]; i++)
				if (value[i] >= 'A' && value[i] < 'X') // Upcase
					value[i] = value[i] - 'A' + 'a';
			this->_method = ZiaString::dup(value);
		}

		void	ClientRequest::SetUrl(char *value)
		{
      char  *test;
      char  *end;

      if (!value)
        return ;
      if (((test = strstr(value, "http://")) || ((test = strstr(value, "https://")))) && test == value)
      {
        end = strchr(value + 7, '/');
        if (strlen(value + 7))
        {
          this->_host = ZiaString::dup(value + 7);
          this->_host[end - value - 7] = 0;
        }
        if (end)
          *end = 0;
        value = (end ? end + 1 : "/");
      }
      if ((test = strstr(value, "?")))
      {
        *test = 0;
        this->SetArgs(ZiaString::dup(++test));
      }
      this->_url = ZiaString::dup(value);
		}

    void  ClientRequest::SetArgs(char* args)
    {
      this->_args = args;
    }

    char*  ClientRequest::GetArgs()
    {
      return (this->_args);
    }

    void	ClientRequest::SetHost(char *value)
		{
      char  *test;

      if (this->_host || !value)
        return ;
      if (((test = strstr(value, "http://")) || ((test = strstr(value, "https://")))) && test == value)
      {
        value += 7;
      }
			this->_host = ZiaString::dup(value);
		}

		void	ClientRequest::SetVersion(char *value)
		{
      if (!value)
        return ;
			this->_version = ZiaString::dup(value);
		}

		//
		// Methods to retrieve field values
		//

		char *ClientRequest::GetMethod()
		{
			return (this->_method);
		}

		char *ClientRequest::GetUrl()
		{
			return (this->_url);
		}

		char *ClientRequest::GetHost()
		{
			return (this->_host);
		}

		char *ClientRequest::GetVersion()
		{
			return (this->_version);
		}

    const string&  ClientRequest::GetType() const
    {
      static  string type = "ClientRequest";
      return (type);
    }

		//
		// Check field values
		//

		bool	ClientRequest::isSetMethod()
		{
			if (this->_method)
				return (true);
			return (false);
		}

		bool	ClientRequest::isSetUrl()
		{
			if (this->_url)
				return (true);
			return (false);
		}

		bool	ClientRequest::isSetHost()
		{
			if (this->_host)
				return (true);
			return (false);
		}

		bool	ClientRequest::isSetVersion()
		{
			if (this->_version)
				return (true);
			return (false);
		}

		void	ClientRequest::addField(char *key, char *value)
		{
			this->_headers[key] = value;
      this->_headersiter = this->_headers.begin();
		}

		bool	ClientRequest::delField(char	*key)
		{
			_headers.erase(key);
      this->_headersiter = this->_headers.begin();
			return (true);
		}

		char*	ClientRequest::operator[](char* key)
		{
      if (this->_headers.find(key) != this->_headers.end())
			  return (this->_headers[key]);
      return (NULL);
		}

    map<const char*, char*, mozia::utils::eqistr>::iterator  *ClientRequest::getNextHeader(void)
    {
      if (++this->_headersiter == this->_headers.end())
      {
        this->_headersiter = this->_headers.begin();
        return (NULL);
      }
      return (&this->_headersiter);
    }
	}
}
