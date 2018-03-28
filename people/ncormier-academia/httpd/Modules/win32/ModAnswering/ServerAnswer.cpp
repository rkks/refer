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
      iter = this->_headers.begin();
      while (iter != this->_headers.end())
      {
        delete [] iter->second;
        iter = this->_headers.erase(iter);
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

    void	ServerAnswer::SetFileSize(__int64 size)
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

    void	ServerAnswer::addField(char *key, unsigned __int64 value)
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

    bool	ServerAnswer::addRange(__int64 begin, __int64 end)
    {
      if (begin < 0 || end < 0)
        return (false);
      if ((unsigned __int64) begin > this->_fileSize || (unsigned __int64) end > this->_fileSize)
        return (false);
      this->_getRange = (unsigned __int64 *) realloc(this->_getRange,
          sizeof(*this->_getRange) * (this->_nbRange + 1) * 2);
      this->_getRange[this->_nbRange * 2] = (unsigned __int64) begin;
      this->_getRange[this->_nbRange * 2 + 1] = (unsigned __int64) end;
      return (true);
    }

    bool	ServerAnswer::addRange(const char *cbegin, const char *cend)
    {
      __int64 begin = (!cbegin || !(*cbegin) ? 0 : _atoi64(cbegin));
      __int64 end = (!cend || !(*cend) ?  this->_fileSize : _atoi64(cend));
      if (begin < 0 || end < 0)
        return (false);
      if ((unsigned __int64) begin > this->_fileSize || (unsigned __int64) end > this->_fileSize)
        return (false);
      this->_getRange = (unsigned __int64 *) realloc(this->_getRange,
          sizeof(*this->_getRange) * (this->_nbRange + 1) * 2);
      this->_getRange[this->_nbRange * 2] = (unsigned __int64) begin;
      this->_getRange[this->_nbRange * 2 + 1] = (unsigned __int64) end;
      this->_nbRange++;
      return (true);
    }

    void  ServerAnswer::changeRange(unsigned __int64 begin, unsigned __int64 end, int nb)
    {
      this->_getRange[nb * 2] = (unsigned __int64) begin;
      this->_getRange[nb * 2 + 1] = (unsigned __int64) end;
    }

    void  ServerAnswer::delRange(int nb)
    {
      unsigned __int64 *tmpRange;

      if (this->_nbRange == 1)
      {
        this->_nbRange = 0;
        free(this->_getRange);
        return ;
      }
      tmpRange = (unsigned __int64 *) malloc(sizeof(*this->_getRange) * (this->_nbRange + 1) * 2);
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

    unsigned __int64	  *ServerAnswer::GetRanges()
    {
      return (this->_getRange);
    }

    bool	ServerAnswer::GetRange(__int64 &begin, __int64 &end, int nb)
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

    unsigned __int64	ServerAnswer::GetFileSize(void)
    {
      return (this->_fileSize);
    }

    unsigned __int64	ServerAnswer::GetResponseSize(void)
    {
      unsigned __int64  respsize = 0;

      if (!this->_nbRange)
        return (this->_fileSize);
      for (int i = 0; i < this->_nbRange; i++)
        respsize += (this->_getRange[i * 2 + 1] - this->_getRange[i * 2]) + 1;
      return (respsize);
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

      this->_version = (this->_version ? this->_version : "HTTP/1.1");
      this->_buff.setData(this->_version, (int) strlen(this->_version));
      this->_buff.addData(" ", 1);
      sprintf_s(status, 4, "%d", this->_status);
      this->_buff.addData(status, (int) strlen(status));
      this->_buff.addData(" ", 1);
      this->_msg = (this->_msg ? this->_msg : "Bad Request");
      this->_buff.addData(this->_msg, (int) strlen(this->_msg));
      this->_buff.addData("\r\n", 2);
      map<const char *, char *, mozia::utils::eqstr>::iterator iter;
      iter = this->_headers.begin();
      while (iter != this->_headers.end())
      {
        this->_buff.addData((char *) iter->first, (int) strlen(iter->first));
        this->_buff.addData(":", 1);
        this->_buff.addData((char *) iter->second, (int) strlen(iter->second));
        this->_buff.addData("\r\n", 2);
        iter++;
      }
      this->_buff.addData("\r\n", 2);
      return (&(this->_buff));
    }
  }
}
