#include  "UrlEnc.h"
#include  "ZiaString.h"
#include  <string.h>

using namespace mozia;

namespace mozia
{
  namespace Transform
  {
    UrlEnc::UrlEnc(void)
    {
    }

    UrlEnc::~UrlEnc(void)
    {
    }

    bool  UrlEnc::mychrcmp(char a, char b)
    {
      if (a >= 'a' && a <= 'f')
        a -= 'z' - 'Z';
      return (b == a);
    }

    unsigned char   UrlEnc::HexToDec(char *hex)
    {
      static char tabconv[] = "0123456789ABCDEF";
      char a1 = -1;
      char a2 = -1;

      for (int i = 0; i < 16; i++)
      {
        if (this->mychrcmp(hex[0], tabconv[i]))
          a1 = (i << 4);
        if (this->mychrcmp(hex[1], tabconv[i]))
          a2 = i;
      }
      if (a1 == -1 || a2 == -1)
        return (-1);
      return (a1 + a2);
    }

    int  UrlEnc::DecToHex(char dec, char *dest)
    {
      static char tabconv[] = "0123456789ABCDEF";

      if ((dec <= ',' && dec != '*') ||
        dec == '+' || dec == ',' || dec == '/'
        || (dec >= ':' && dec <= '@') ||
        (dec >= '[' && dec <= '^') || dec == '`'
        ||dec >= '{')
      {
          dest[0] = '%';
          dest[1] = tabconv[dec >> 4];
          dest[2] = tabconv[dec & 0x0F];
          return (3);
      }
      *dest = dec;
      return (1);
    }

    char  *UrlEnc::Encode(char *data)
    {
        unsigned int  i;
        int total = BSIZE;
        size_t  len;
        int  buflen;
        char  *out;
        
        out = new char[total];
        buflen = 0;
        len = strlen(data);
        for (i = 0; i < len; i++)
        {
          int s = this->DecToHex(data[i], out + buflen);
          buflen += s;
          if (buflen >= (total - 4))
          {
            total += BSIZE;
            char *tmp = new char[total];
            strncpy_s(tmp, total, out, buflen);
            delete [] out;
            out = tmp;
          }
        }
        out[buflen] = 0;
        return (out);
    }

    char  *UrlEnc::Decode(char *data)
    {
        char    *str;
        size_t  len;
        int     res;

        data = ZiaString::dup(data);
        len = strlen(data);
        str = data;
        while ((str = strchr(str, '+')))
          *str = ' ';
        str = data;
        while ((str = strchr(str, '%')))
        {
          str++;
          if ((unsigned int)(str - data) + 2 < len)
          {
            res = this->HexToDec(str);
            if (res != -1)
            {
              *(str - 1) = res;
#pragma warning (disable : 4996)
              strcpy(str, str + 2);
            }
          }
        }
        return (data);
    }
  }
}