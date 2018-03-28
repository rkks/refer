/*
**
**	$HeadURL: svn://svn.mozia.info/repo/zia/src/includes/Thread.h $
**	$Author: nico $
**	$Revision: 687 $
**	$Date: 2006-04-19 23:11:07 +0200 (Wed, 19 Apr 2006) $
**
*/

#ifndef __THREAD_H__
# define __THREAD_H__

# ifdef _WIN32
#  include <windows.h>
# else
#  include <pthread.h>
#  include <signal.h>
# endif

# include "IThread.h"

namespace mozia
{
	namespace core
	{
		class Thread : public IThread
		{
		public:
			Thread(s_procPt* procPt, void *arg);
			~Thread();
			void		Join();

		private:
#ifdef _WIN32
			HANDLE		_hThread;
			DWORD		_hThreadId;
#else
			pthread_t   _thread;
			s_procPt*   _procPt;
			void*       _args;
#endif
		};
	}
}

#endif // !__THREAD_H__
