/*
**
**	$HeadURL: svn://epizia.homeip.net/repo/zia/src/includes/CoreMsg.h $
**	$Author: nico $
**	$Revision: 361 $
**	$Date: 2005-12-06 15:52:07 +0100 (Tue, 06 Dec 2005) $
**
*/

#ifndef __COREMSG_H__
#define __COREMSG_H__
# include "Ticket.h"

using namespace mozia::ticket;

namespace mozia
{
	namespace ticket
	{

		class CoreMsg : public Ticket
		{
		public:
			CoreMsg(void);
			const string&	GetType();
		public:
			virtual ~CoreMsg(void);
		};

	}

}

#endif
