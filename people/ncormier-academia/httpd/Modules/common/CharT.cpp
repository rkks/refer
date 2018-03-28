/*
**
**	$HeadURL: svn://epizia.homeip.net/repo/zia/src/CharT.cpp $
**	$Author: nico $
**	$Revision: 241 $
**	$Date: 2005-11-15 08:44:39 -0800 (Tue, 15 Nov 2005) $
**
*/

//#ifdef _DEBUG
//#include <stdlib.h>
//#define _CRTDBG_MAP_ALLOC
//#include <crtdbg.h>
//#endif

#include <string>
#include "CharT.h"

#include "LeakWatcher.h"
#ifdef _DEBUG
# define new DEBUG_NEW
# undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


namespace mozia
{
	namespace utils
	{

		CharT::CharT(const char *str)
		{
			unsigned int mod = strlen(str) / 64;
			if (mod * 64 < strlen(str))
				mod++;
			this->buf = new char[mod * 64];
			this->buflen = 64;
			memcpy(this->buf, str, strlen(str));
			this->len = strlen(str);
		}

		CharT::~CharT(void)
		{
			delete[] this->buf;
		}

		void	CharT::Set(const char *str)
		{
			memcpy(this->buf, str, strlen(str));
			this->len = strlen(str);
		}

		void	CharT::Cat(const char *str)
		{
			if (this->len + strlen(str) > this->buflen)
			{
				unsigned int mod = strlen(str) / 64;
				if (mod * 64 < strlen(str))
					mod++;
				char* tmp = new char[this->buflen + (64 * mod)];
				this->buflen = this->buflen + (64 * mod);
				memcpy(this->buf + this->len - 1, str, strlen(str));
				this->len = strlen(this->buf);
			}
		}

	}

}
