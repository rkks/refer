
#include <sstream>
#include "DBuffer.h"
#include "ServerAnswer.h"



using namespace	std;

namespace	mozia
{
  namespace	answering
  {
    ServerAnswer::ServerAnswer(void)
    {
      this->MDate[0] = 0;
      this->_version = 0;
      this->_msg = 0;
      this->_status = 400;
      this->_fileSize = 0;
      this->_fileError = 0;
      this->_path = 0;
      this->_request = 0;
      this->_getRange = 0;
      this->_cgi = 0;
      this->_root = 0;
      this->_atype = REG_FILE;
      this->_nbRange = 0;
      this->_isRange = false;
      this->_valid = false;
    }

    ServerAnswer::~ServerAnswer(void)
    {
      map<const char *, char *, mozia::utils::eqstr>::iterator iter;
      map<const char *, char *, mozia::utils::eqstr>::iterator back;

      iter = this->_headers.begin();
      while (iter != this->_headers.end())
      {
        delete [] iter->second;
	back = iter++;
        this->_headers.erase(back);
      }
      if (this->_path)
        delete [] this->_path;
      if (this->_getRange)
        free (this->_getRange);
    }

    const string&  ServerAnswer::GetType() const
    {
      static  string type = "ServerAnswer";
      return (type);
    }

    void	ServerAnswer::SetVersion(char *version)
    {
      this->_version = version;
    }

    void	ServerAnswer::SetType(AnswerType atype)
    {
      this->_atype = atype;
    }

    void	ServerAnswer::SetFileError(int error)
    {
      this->_fileError = error;
    }

    void	ServerAnswer::SetFileSize(long long size)
    {
      this->_fileSize = size;
    }

    void	ServerAnswer::SetStatus(int status)
    {
      if (status > 0 && status < 1000)
        this->_status = status;
    }

    void	ServerAnswer::SetMsg(char *msg)
    {
      this->_msg = msg;
    }

    void	ServerAnswer::SetCGIPath(char *path)
    {
      for (int i = 0; path[i]; i++)
        if (path[i] == '\\')
          path[i] = '/';
      this->_cgi = path;
    }

    void	ServerAnswer::SetRoot(char *path)
    {
      this->_root = path;
    }

    void	ServerAnswer::SetPath(char *path)
    {
      for (int i = 0; path && path[i]; i++)
        if (path[i] == '\\')
          path[i] = '/';
      this->_path = path;
    }

    void  ServerAnswer::SetValid(bool validity)
    {
      this->_valid = validity;
    }

    void	ServerAnswer::SetRequest(ClientRequest *req)
    {
      this->_request = req;
    }

    void	ServerAnswer::addField(char *key, char *value)
    {
      map<const char*, char*, mozia::utils::eqstr>::iterator  iter;

      char  *v = ZiaString::dup(value);
      iter = this->_headers.find(key);
      if (iter == this->_headers.end())
        this->_headers[key] = v;
      else
	{
	  delete [] iter->second;
	  iter->second = v;
	}
    }

    void	ServerAnswer::addField(char *key, int value)
    {
      ostringstream	oss;
      map<const char*, char*, mozia::utils::eqstr>::iterator  iter;

      oss << value;
      char  *v = ZiaString::dup(oss.str().c_str());

      iter = this->_headers.find(key);
      if (iter == this->_headers.end())
        this->_headers[key] = v;
      else
	{
	  delete [] iter->second;
	  iter->second = v;
	}
    }

    void	ServerAnswer::addField(char *key, unsigned long long value)
    {
      ostringstream	oss;
      map<const char*, char*, mozia::utils::eqstr>::iterator  iter;

      oss << value;
      char  *v = ZiaString::dup(oss.str().c_str());

      iter = this->_headers.find(key);
      if (iter == this->_headers.end())
        this->_headers[key] = v;
      else
	{
	  delete [] iter->second;
	  iter->second = v;
	}
    }

    bool	ServerAnswer::addRange(long long begin, long long end)
    {
      if (begin < 0 || end < 0)
        return (false);
      if ((unsigned long long) begin > this->_fileSize || (unsigned long long) end > this->_fileSize)
        return (false);
      this->_getRange = (unsigned long long *) realloc(this->_getRange,
						       sizeof(*this->_getRange) * (this->_nbRange + 1) * 2);
      this->_getRange[this->_nbRange * 2] = (unsigned long long) begin;
      this->_getRange[this->_nbRange * 2 + 1] = (unsigned long long) end;
      return (true);
    }

    bool	ServerAnswer::addRange(const char *cbegin, const char *cend)
    {
//       long long begin = (cbegin ? _atoi64(cbegin) : 0);
//       long long end = (cend ? _atoi64(cend) : this->_fileSize);
      long long begin = (cbegin ? atoi(cbegin) : 0);
      long long end = (cend ? atoi(cend) : this->_fileSize);
      if (begin < 0 || end < 0)
        return (false);
      if ((unsigned long long) begin > this->_fileSize ||
	  (unsigned long long) end > this->_fileSize)
        return (false);
      this->_getRange = (unsigned long long *)
	realloc(this->_getRange,
		sizeof(*this->_getRange) * (this->_nbRange + 1) * 2);
      this->_getRange[this->_nbRange * 2] = (unsigned long long) begin;
      this->_getRange[this->_nbRange * 2 + 1] = (unsigned long long) end;
      this->_nbRange++;
      return (true);
    }

    void  ServerAnswer::changeRange(unsigned long long begin,
				    unsigned long long end, int nb)
    {
      this->_getRange[nb * 2] = (unsigned long long) begin;
      this->_getRange[nb * 2 + 1] = (unsigned long long) end;
    }

    void  ServerAnswer::delRange(int nb)
    {
      unsigned long long *tmpRange;

      if (this->_nbRange == 1)
	{
	  this->_nbRange = 0;
	  free(this->_getRange);
	  return ;
	}
      tmpRange = (unsigned long long *) malloc(sizeof(*this->_getRange) *
					       (this->_nbRange + 1) * 2);
      memcpy(tmpRange, this->_getRange, nb * 2 * sizeof(*this->_getRange));
      memcpy(tmpRange, this->_getRange + nb + 1, (this->_nbRange - nb - 1) * 2
	     * sizeof(*this->_getRange));
      free(this->_getRange);
      this->_getRange = tmpRange;
      this->_nbRange--;
    }

    void  ServerAnswer::IsRange(bool range)
    {
      this->_isRange = range;
    }

    bool  ServerAnswer::IsRange(void)
    {
      return (this->_isRange);
    }

    unsigned long long	  *ServerAnswer::GetRanges()
    {
      return (this->_getRange);
    }

    bool	ServerAnswer::GetRange(long long &begin, long long &end,
				       int nb)
    {
      if (nb < 0 || nb >= this->_nbRange)
        return (false);
      begin = this->_getRange[nb * 2];
      end = this->_getRange[nb * 2 + 1];
      return (true);
    }

    char	*ServerAnswer::GetVersion(void)
    {
      return (this->_version);
    }

    bool  ServerAnswer::GetValid(void)
    {
      return (this->_valid);
    }

    AnswerType  ServerAnswer::GetType(void)
    {
      return (this->_atype);
    }

    char	*ServerAnswer::GetRoot(void)
    {
      return (this->_root);
    }

    char	*ServerAnswer::GetCGIPath(void)
    {
      return (this->_cgi);
    }

    int		ServerAnswer::GetStatus(void)
    {
      return (this->_status);
    }

    char	*ServerAnswer::GetMsg(void)
    {
      return (this->_msg);
    }

    char	*ServerAnswer::GetPath(void)
    {
      return (this->_path);
    }

    int	ServerAnswer::GetFileError(void)
    {
      return (this->_fileError);
    }

    unsigned long long	ServerAnswer::GetFileSize(void)
    {
      return (this->_fileSize);
    }

    unsigned int	ServerAnswer::GetNbRanges()
    {
      return (this->_nbRange);
    }

    ClientRequest	*ServerAnswer::GetRequest(void)
    {
      return (this->_request);
    }

    char*	ServerAnswer::operator[](char* key)
    {
      return (this->_headers[key]);
    }

    Buffer	*ServerAnswer::Render(void)
    {
      char	status[4];

      this->_version = (this->_version ? this->_version : (char *)"HTTP/1.1");
      this->_buff.setData(this->_version, strlen(this->_version));
      this->_buff.addData(" ", 1);
      snprintf(status, 4, "%d", this->_status);
      this->_buff.addData(status, strlen(status));
      this->_buff.addData(" ", 1);
      this->_msg = (this->_msg ? this->_msg : (char *)"Bad Request");
      this->_buff.addData(this->_msg, strlen(this->_msg));
      this->_buff.addData("\r\n", 2);
      map<const char *, char *, mozia::utils::eqstr>::iterator iter;
      iter = this->_headers.begin();
      while (iter != this->_headers.end())
	{
	  this->_buff.addData((char *) iter->first, strlen(iter->first));
	  this->_buff.addData(":", 1);
	  this->_buff.addData((char *) iter->second, strlen(iter->second));
	  this->_buff.addData("\r\n", 2);
	  iter++;
	}
      this->_buff.addData("\r\n", 2);
      return (&(this->_buff));
    }
  }
}
