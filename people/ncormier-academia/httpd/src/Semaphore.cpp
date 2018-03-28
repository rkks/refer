/*
**
**	$HeadURL: svn://svn.mozia.info/repo/zia/src/Semaphore.cpp $
**	$Author: nico $
**	$Revision: 687 $
**	$Date: 2006-04-19 23:11:07 +0200 (Wed, 19 Apr 2006) $
**
*/

#include <iostream>
#include <sstream>

#include "Semaphore.h"
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
        // un wait ou un post est considere comme fatale.
        // Les valeurs de retour sont dans notre cas inutile.
	    //
		Semaphore::Semaphore()
		{
        #ifdef _WIN32
			this->_hSemaphore = CreateSemaphore(NULL, 0, SEM_MAX_COUNT,NULL);
			if (!this->_hSemaphore)
			{
                ostringstream oss;
                oss << "Semaphore::Semaphore()  GetLastError=" << ::GetLastError();
				throw new SemaphoreCreationException(oss.str());
			}
        #else
            if (sem_init(&this->_semaphore, 0, 0) == -1)
            {
                ostringstream oss;
                oss << "Semaphore::Semaphore()  errno=" << ::strerror(errno);
            	throw new SemaphoreCreationException(oss.str());
            }
        #endif
		};

        //
        // FIX: Post bloquant plutot qu'une erreur fatale.
        //
		bool	Semaphore::Post()
		{
        #ifdef _WIN32
			while (!ReleaseSemaphore(this->_hSemaphore, 1, NULL))
			{
                //ostringstream oss;
                //oss << "Semaphore::Post()  GetLastError=" << ::GetLastError();
                //throw new SemaphorePostException(oss.str());
                ::Sleep(100);
			}
        #else
            while (sem_post(&this->_semaphore) == -1)
            {
                //ostringstream oss;
                //oss << "Semaphore::Post()  errno=" << ::strerror(errno);
                //throw new SemaphorePostException(oss.str());
                usleep(100);
            }
        #endif
			return (true);
		}

		bool	Semaphore::Wait()
		{
        #ifdef _WIN32
			DWORD dwWaitResult;

			dwWaitResult = WaitForSingleObject(this->_hSemaphore, INFINITE);
			if (dwWaitResult == WAIT_OBJECT_0)
				return (true);
            ostringstream oss;
            oss << "Semaphore::Wait()  GetLastError=" << ::GetLastError();
            throw new SemaphoreWaitException(oss.str());
        #else
            if (sem_wait(&this->_semaphore) != -1)
                return (true);
            ostringstream oss;
            oss << "Semaphore::Wait()  errno=" << ::strerror(errno);
            throw new SemaphoreWaitException(oss.str());
        #endif
			return (false);
		}

		bool	Semaphore::TryWait()
		{
        #ifdef _WIN32
			DWORD dwWaitResult;

			dwWaitResult = WaitForSingleObject(this->_hSemaphore, 0L);
			if (dwWaitResult == WAIT_OBJECT_0)
				return (true);
        #else
            if (sem_trywait(&this->_semaphore) != -1)
                return (true);
        #endif
			return (false);
		}

		Semaphore::~Semaphore()
		{
        #ifdef _WIN32
			if (CloseHandle(this->_hSemaphore))
				return;
            ostringstream oss;
            oss << "Semaphore::~Semaphore()  GetLastError=" << ::GetLastError();
            throw new SemaphoreDestructionException(oss.str());
        #else
            if (sem_destroy(&this->_semaphore) != -1)
                return;
            ostringstream oss;
            oss << "Semaphore::Post()  errno=" << ::strerror(errno);
            throw new SemaphoreDestructionException(oss.str());
        #endif
		}

	}
}
