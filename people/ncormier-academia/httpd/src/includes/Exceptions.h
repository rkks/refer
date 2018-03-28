/*
**
**	$HeadURL: svn://svn.mozia.info/repo/zia/src/includes/Exceptions.h $
**	$Author: nico $
**	$Revision: 687 $
**	$Date: 2006-04-19 23:11:07 +0200 (Wed, 19 Apr 2006) $
**
*/

#ifndef __EXCEPTIONS_H__
# define __EXCEPTIONS_H__

# include <string>
# include <sstream>

using namespace std;

namespace mozia
{
	namespace exceptions
	{

		class ZiaException
		{
		public:
			virtual string  ToString(void) = 0;
		};

		//
		// Core.cpp Exceptions
		//
		class CoreException : public ZiaException
		{
		public:
			CoreException(string err)
			{
				this->Error = err;
			}
			string  ToString(void)
			{
				return (this->Error);
			}
			string Error;
		};

		//
		// Module.cpp Exceptions
		//
		class ModuleBadStatusException : public ZiaException
		{
		public:
			ModuleBadStatusException(string err)
			{
				this->Error = err;
			}
			string  ToString(void)
			{
				return (this->Error);
			}
			string Error;
		};

		class ModuleLoadException : public ZiaException
		{
		public:
			ModuleLoadException(string mod, string err)
			{
				this->Error = err;
				this->Mod = mod;
			}
			string  ToString(void)
			{
				return (this->Error);
			}
			string Error;
			string Mod;
		};

		class ModuleStartException : public ZiaException
		{
		public:
			ModuleStartException(string mod, string err)
			{
				this->Error = err;
				this->Mod = mod;
			}
			string  ToString(void)
			{
				return (this->Error);
			}
			string Error;
			string Mod;
		};

		class ModuleLoopException : public ZiaException
		{
		public:
			ModuleLoopException(string err)
			{
				this->Error = err;
			}
			string  ToString(void)
			{
				return (this->Error);
			}
			string Error;
		};

		//
		// Thread.cpp Exceptions
		//
		class ThreadCreationException : public ZiaException
		{
		public:
			ThreadCreationException(string err)
			{
				this->Error = err;
			}
			string  ToString(void)
			{
				return (this->Error);
			}
			string Error;
		};

		class ThreadJoiningException : public ZiaException
		{
		public:
			ThreadJoiningException(string err)
			{
				this->Error = err;
			}
			string  ToString(void)
			{
				return (this->Error);
			}
			string Error;
		};

		class ThreadDestructionException : public ZiaException
		{
		public:
			ThreadDestructionException(string err)
			{
				this->Error = err;
			}
			string  ToString(void)
			{
				return (this->Error);
			}
			string Error;
		};
		//
		// Dll.cpp Exceptions
		//
		class DllLoadException : public ZiaException
		{
		public:
			DllLoadException(string err)
			{
				this->Error = err;
			}
			string  ToString(void)
			{
				return (this->Error);
			}
			string Error;
		};

		class DllEntryPointException : public ZiaException
		{
		public:
			DllEntryPointException(string err)
			{
				this->Error = err;
			}
			string  ToString(void)
			{
				return (this->Error);
			}
			string Error;
		};

		class DllUnloadException : public ZiaException
		{
		public:
			DllUnloadException(string err)
			{
				this->Error = err;
			}
			string  ToString(void)
			{
				return (this->Error);
			}
			string Error;
		};


		//
		// Semaphore.cpp Exceptions
		//
		class SemaphoreCreationException : public ZiaException
		{
		public:
			SemaphoreCreationException(string err)
			{
				this->Error = err;
			}
			string  ToString(void)
			{
				return (this->Error);
			}
			string Error;
		};

		class SemaphoreWaitException : public ZiaException
		{
		public:
			SemaphoreWaitException(string err)
			{
				this->Error = err;
			}
			string  ToString(void)
			{
				return (this->Error);
			}
			string Error;
		};

		class SemaphorePostException : public ZiaException
		{
		public:
			SemaphorePostException(string err)
			{
				this->Error = err;
			}
			string  ToString(void)
			{
				return (this->Error);
			}
			string Error;
		};

		class SemaphoreDestructionException : public ZiaException
		{
		public:
			SemaphoreDestructionException(string err)
			{
				this->Error = err;
			}
			string  ToString(void)
			{
				return (this->Error);
			}
			string Error;
		};

		//
		// Mutex.cpp Exceptions
		//
		class MutexCreationException : public ZiaException
		{
		public:
			MutexCreationException(string err)
			{
				this->Error = err;
			}
			string  ToString(void)
			{
				return (this->Error);
			}
			string Error;
		};

		class MutexDestructionException : public ZiaException
		{
		public:
			MutexDestructionException(string err)
			{
				this->Error = err;
			}
			string  ToString(void)
			{
				return (this->Error);
			}
			string Error;
		};

		class MutexLockException : public ZiaException
		{
		public:
			MutexLockException(string err)
			{
				this->Error = err;
			}
			string  ToString(void)
			{
				return (this->Error);
			}
			string Error;
		};

		class MutexUnlockException : public ZiaException
		{
		public:
			MutexUnlockException(string err)
			{
				this->Error = err;
			}
			string  ToString(void)
			{
				return (this->Error);
			}
			string Error;
		};

		//
		// Network.cpp Exceptions
		//
		class NetworkException : public ZiaException
		{
		public:
			NetworkException(string err, int err_code)
			{
				this->Error = err;
				this->ErrCode = err_code;
			}
			string  ToString(void)
			{
				std::ostringstream oss;
				oss << this->Error << ", code = " << this->ErrCode;
				return (oss.str());
			}
			string	Error;
			int		ErrCode;
		};

		//
		// XmlParser.cpp Exceptions
		//
		class XmlParserException : public ZiaException
		{
		public:
			XmlParserException(string err)
			{
				this->Error = err;
			}
			string  ToString(void)
			{
				return (this->Error);
			}
			string Error;
		};

	}
}
#endif /*__EXCEPTIONS_H__*/
