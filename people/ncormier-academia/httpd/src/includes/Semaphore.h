/*
**
**	$HeadURL: svn://svn.mozia.info/repo/zia/src/includes/Semaphore.h $
**	$Author: nico $
**	$Revision: 687 $
**	$Date: 2006-04-19 23:11:07 +0200 (Wed, 19 Apr 2006) $
**
*/

#ifndef __SEMAPHORE_H__
# define __SEMAPHORE_H__

# ifdef _WIN32
#  include <windows.h>
# else
#  include <semaphore.h>
# endif

# include "ISemaphore.h"

# define SEM_MAX_COUNT	50

namespace mozia
{
	namespace network
	{
		class Network;
		class NetworkSSL;
	}

	namespace core
	{

		class Semaphore : public ISemaphore
		{
			friend class mozia::network::Network;
			friend class mozia::network::NetworkSSL;

		public:
			Semaphore();
			~Semaphore();
			bool	Post();
			bool	Wait();
			bool	TryWait();
		private:
#ifdef _WIN32
			HANDLE	_hSemaphore;
#else
			sem_t   _semaphore;
#endif
		};

	}
}

#endif //__SEMAPHORE_H__
