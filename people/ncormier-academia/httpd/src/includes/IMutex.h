/*
**
**	$HeadURL: svn://epizia.homeip.net/repo/zia/src/includes/IMutex.h $
**	$Author: nico $
**	$Revision: 318 $
**	$Date: 2005-11-27 13:02:37 -0800 (Sun, 27 Nov 2005) $
**
*/

#ifndef __IMUTEX_H__
# define __IMUTEX_H__

namespace mozia
{
	namespace core
	{

		class IMutex
		{
		public:
			virtual bool	Lock() = 0;
			virtual bool	UnLock() = 0;
		};

	}

}

#endif
