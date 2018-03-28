#ifndef __URL_ENC_
# define __URL_ENC_

#include "ITransform.h"

using namespace mozia::Transform;

#define BSIZE 4096

namespace mozia
{
  namespace Transform
    {
      class UrlEnc : ITransform
	{
	public:
	  char *Encode(char *data);
	  char *Decode(char *data);
	  UrlEnc(void);
	  ~UrlEnc(void);

	private:
	  int  DecToHex(char dec, char *dest);
	  unsigned char   HexToDec(char *hex);
	  bool  mychrcmp(char a, char b);
	};
    }
}

#endif /* __URL_ENC */
