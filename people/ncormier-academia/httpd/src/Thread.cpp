/*
**
**	$HeadURL: svn://svn.mozia.info/repo/zia/src/Thread.cpp $
**	$Author: nico $
**	$Revision: 687 $
**	$Date: 2006-04-19 23:11:07 +0200 (Wed, 19 Apr 2006) $
**
*/

#include <iostream>
#include <sstream>

#include "Thread.h"
#include "Exceptions.h"

#ifndef _WIN32
# include <errno.h>
# include <string.h>
#endif

using namespace std;
using namespace mozia::exceptions;

namespace mozia
{
	namespace core
	{
		Thread::Thread(s_procPt* procPt, void *args)
		{
        #ifdef _WIN32
			this->_hThread = CreateThread( NULL, 0,
										  (LPTHREAD_START_ROUTINE) procPt->entryPoint,
										   args, CREATE_SUSPENDED, &(this->_hThreadId)
										  );
			if (this->_hThread == NULL)
			{
                ostringstream oss;
                oss << "Thread::Thread()  GetLastError=" << ::GetLastError();
				throw new ThreadCreationException(oss.str());
			}
        #else
            this->_procPt = procPt;
            this->_args = args;
        #endif
		}

		void	Thread::Join()
		{
        #ifdef _WIN32
			if (ResumeThread(this->_hThread) == -1)
			{
                ostringstream oss;
                oss << "Thread::Join()  GetLastError=" << ::GetLastError();
				throw new ThreadJoiningException(oss.str());
			}
        #else
            if (pthread_create(&this->_thread, NULL, this->_procPt->entryPoint, this->_args))
            {
                ostringstream oss;
                oss << "Thread::Join()  errno=" << ::strerror(errno);
                throw new ThreadJoiningException(oss.str());
            }
        #endif
		}

		Thread::~Thread()
		{
        #ifdef _WIN32
			if (!TerminateThread(this->_hThread, 0))
			{
				ostringstream oss;
                oss << "Thread::~Thread()  GetLastError=" << ::GetLastError();
				throw new ThreadDestructionException(oss.str());
			}
        #else
			//
			// FIX: Module could already have quit.
			//
			if (pthread_cancel(this->_thread))
            {
                ostringstream oss;
                oss << "Thread::~Thread()  errno=" << ::strerror(errno);
 				//throw new ThreadDestructionException(oss.str());
            }
        #endif
		}

	}
}
