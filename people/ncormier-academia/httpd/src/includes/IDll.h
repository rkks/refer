/*
**
**	$HeadURL: svn://svn.mozia.info/repo/zia/src/includes/IDll.h $
**	$Author: nico $
**	$Revision: 687 $
**	$Date: 2006-04-19 23:11:07 +0200 (Wed, 19 Apr 2006) $
**
*/

#ifndef __IDLL_H__
# define __IDLL_H__

namespace mozia
{
	namespace core
	{

		struct s_procPt
		{
			void* (*entryPoint)(void *);
		};

		class IDll
		{
		public:
			virtual s_procPt*		GetProcPoint() = 0;
		};

	}
}

#endif //__IDLL_H__
