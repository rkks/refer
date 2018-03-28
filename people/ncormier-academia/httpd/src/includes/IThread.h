/*
**
**	$HeadURL: svn://svn.mozia.info/repo/zia/src/includes/IThread.h $
**	$Author: nico $
**	$Revision: 687 $
**	$Date: 2006-04-19 23:11:07 +0200 (Wed, 19 Apr 2006) $
**
*/

#ifndef __ITHREAD_H__
# define __ITHREAD_H__

# include "IDll.h"

namespace mozia
{
	namespace core
	{

		class IThread
		{
		public:
			virtual void	Join() = 0;
		};

	}
}

#endif //__IITHREAD_H__
