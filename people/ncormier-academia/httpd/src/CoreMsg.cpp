/*
**
**	$HeadURL: svn://epizia.homeip.net/repo/zia/src/CoreMsg.cpp $
**	$Author: yann $
**	$Revision: 386 $
**	$Date: 2005-12-11 14:34:14 +0100 (Sun, 11 Dec 2005) $
**
*/

#include "CoreMsg.h"

namespace mozia
{
	namespace ticket
	{

		CoreMsg::CoreMsg(void)
		{
		}

		CoreMsg::~CoreMsg(void)
		{
		}

		const string&	CoreMsg::GetType()
		{
			static string ret = "CoreMsg";

			return (ret);
		}
	}
}
