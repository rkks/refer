#include <string>

using namespace std;

namespace mozia
{
  namespace modules
  {
    namespace utils
    {
      class Url
      {
      public:
        Url(string);
        string  UrlEncode();
        string  UrlDecode();
        void    SetUrl(string);
        bool    EncodingHasFailed;
        bool    DecodingHasFailed;

      private:
        string  _url;
        string  _encoded;
        string  _decoded;
        bool    _isEncoded;
        bool    _isDecoded;
        char     getASCIIFromHex(char *buffer, int count);
      };
    }
  }
}
