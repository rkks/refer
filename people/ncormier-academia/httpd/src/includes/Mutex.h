/*
**
**	$HeadURL: svn://epizia.homeip.net/repo/zia/src/includes/Mutex.h $
**	$Author: nico $
**	$Revision: 318 $
**	$Date: 2005-11-27 13:02:37 -0800 (Sun, 27 Nov 2005) $
**
*/

#ifndef __MUTEX_H__
# define __MUTEX_H__

# ifdef _WIN32
#  include <windows.h>
# else
#  include <pthread.h>
# endif

# include "IMutex.h"

namespace mozia
{
	namespace core
	{

		class Mutex : public IMutex
		{
		public:
			Mutex();
			~Mutex();
			bool	Lock();
			bool	UnLock();
		private:
#ifdef _WIN32
			HANDLE			hMutex;
#else
			pthread_mutex_t	mutex;
#endif
		};

	}

}

#endif

