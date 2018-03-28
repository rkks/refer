/*
**
**	$HeadURL: svn://svn.mozia.info/repo/zia/src/includes/ISemaphore.h $
**	$Author: nico $
**	$Revision: 539 $
**	$Date: 2006-04-11 00:13:17 +0200 (Tue, 11 Apr 2006) $
**
*/

#ifndef __ISEMAPHORE_H__
# define __ISEMAPHORE_H__

namespace mozia
{
	namespace core
	{

		class ISemaphore
		{
		public:
			virtual bool	Post() = 0;
			virtual bool	Wait() = 0;
			virtual bool	TryWait() = 0;
		};

	}

}


#endif //__ISEMAPHORE_H__
