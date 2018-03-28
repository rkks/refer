/*
**
**	$HeadURL: svn://epizia.homeip.net/repo/zia/src/includes/ICData.h $
**	$Author: nico $
**	$Revision: 237 $
**	$Date: 2005-11-14 14:03:38 -0800 (Mon, 14 Nov 2005) $
**
*/

# ifndef __ICDATA_H__
#  define __ICDATA_H__
#  include <string>

namespace mozia
{
	namespace ticket
	{
		class ICData
		{
		public:
			virtual							~ICData(){}
			virtual const std::string&		GetType() const = 0;
		};

	}
}


#endif
