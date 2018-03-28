/*
**
**	$HeadURL: svn://epizia.homeip.net/repo/zia/src/Mutex.cpp $
**	$Author: nico $
**	$Revision: 318 $
**	$Date: 2005-11-27 13:02:37 -0800 (Sun, 27 Nov 2005) $
**
*/

#include <iostream>
#include <sstream>

#include "Mutex.h"
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
	    //
	    // Dans notre implementation l'echec sur
        // un lock ou un unlock est considere comme fatale.
        // Les valeurs de retour sont dans notre cas inutile.
	    //
		Mutex::Mutex()
		{
        #ifdef _WIN32
			this->hMutex = CreateMutex(NULL, FALSE, NULL);
			if (this->hMutex == NULL)
			{
                ostringstream oss;
                oss << "Mutex::Mutex()  GetLastError=" << ::GetLastError();
				throw new MutexCreationException(oss.str());
			}
        #else
            if (pthread_mutex_init(&this->mutex, NULL))
            {
                ostringstream oss;
                oss << "Mutex::Mutex()  errno=" << ::strerror(errno);
                throw new MutexCreationException(oss.str());
            }
        #endif
		}

		Mutex::~Mutex()
		{
        #ifdef _WIN32
            if (CloseHandle(this->hMutex) == FALSE)
            {
                ostringstream oss;
                oss << "Mutex::~Mutex()  GetLastError=" << ::GetLastError();
                throw new MutexDestructionException(oss.str());
            }
        #else
            if (pthread_mutex_destroy(&this->mutex))
            {
                ostringstream oss;
                oss << "Mutex::~Mutex()  errno=" << ::strerror(errno);
                throw new MutexDestructionException(oss.str());
            }
        #endif
		}

		bool	Mutex::Lock()
		{
        #ifdef _WIN32
			DWORD dwWaitResult = WaitForSingleObject(this->hMutex, INFINITE);
			if (dwWaitResult == WAIT_OBJECT_0)
				return (true);
            ostringstream oss;
            oss << "Mutex::Lock()  GetLastError=" << ::GetLastError();
            throw new MutexLockException(oss.str());
        #else
            if (pthread_mutex_lock(&this->mutex) == 0)
                return (true);
            ostringstream oss;
            oss << "Mutex::Lock()  errno=" << ::strerror(errno);
            throw new MutexLockException(oss.str());
		#endif
			return (false);
		}

		bool	Mutex::UnLock()
		{
        #ifdef _WIN32
			if (ReleaseMutex(this->hMutex))
				return (true);
            ostringstream oss;
            oss << "Mutex::UnLock()  GetLastError=" << ::GetLastError();
            throw new MutexUnlockException(oss.str());
        #else
            if (pthread_mutex_unlock(&this->mutex) == 0)
                return (true);
            ostringstream oss;
            oss << "Mutex::~UnLock()  errno=" << ::strerror(errno);
            throw new MutexUnlockException(oss.str());
        #endif
			return (false);
		}


	}

}
