//#ifdef _DEBUG
//#include <stdlib.h>
//#define _CRTDBG_MAP_ALLOC
//#include <crtdbg.h>
//#endif

#include <string>
#include "UrlEncoding.h"

#include "LeakWatcher.h"
#ifdef _DEBUG
# define new DEBUG_NEW
# undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


using namespace std;

namespace mozia
{
  namespace modules
  {
    namespace utils
    {
      Url::Url(string url)
      {
        this->_url = url;
        this->DecodingHasFailed = false;
        this->EncodingHasFailed = false;
        this->_isDecoded = false;
        this->_isEncoded = false;
      }

      void  Url::SetUrl(string url)
      {
        this->_url = url;
        this->_isEncoded = false;
        this->_isDecoded = false;
        this->EncodingHasFailed = false;
        this->DecodingHasFailed = false;
      }

      char Url::getASCIIFromHex(char *buffer, int count)
      {
        int   i;
        int   j;
        int   power = 16;
        char  value = 0;
        char  upper;
        char* hexStr = "0123456789ABCDEF";

        for (i = 0; buffer[i] && i < count; i++)
        {
          if (buffer[i] >= 'a' && buffer[i] <= 'z')
            upper = buffer[i] - 'a' + 'A';
          else if (buffer[i] >= '0' && buffer[i] <= '9')
          {
            value += (power * (buffer[i] - '0'));
            power /= 16;
            continue;
           }
          else
            upper = buffer[i];
          for (j = 0; hexStr[j]; j++)
            if (upper == hexStr[j])
            {
              value += (power * j);
              break;
            }
          if (!hexStr[j])
            this->DecodingHasFailed = true;
        }
        return (value);
      }

      string Url::UrlEncode()
      {
        return (this->_url);
      }

      string Url::UrlDecode()
      {
        int                 i;
        int                 j;
        static char*        buffer = 0;
        static unsigned int size = 0;

        if (this->_isDecoded)
          return (this->_decoded);
        if (size < this->_url.length())
        {
          delete buffer;
          size = (unsigned int)this->_url.length();
          buffer = new char(size + 1);
        }
        memcpy(buffer, this->_url.c_str(), this->_url.length());
        for (i = 0, j = 0; buffer[i]; i++, j++)
          if (buffer[i] == '%' && buffer[i + 1] && buffer[i + 2])
          {
            buffer[j] = getASCIIFromHex(buffer + i + 1, 2);
            i += 2;
          }
          else
            buffer[j] = buffer[i];
        buffer[j] = '\0';
        this->_decoded = string(buffer);
        this->_isDecoded = true;
        return (this->_decoded);
      }
    }
  }
}
