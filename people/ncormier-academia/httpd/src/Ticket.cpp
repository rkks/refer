/*
**
**	$HeadURL: svn://epizia.homeip.net/repo/zia/src_common/Ticket.cpp $
**	$Author: nico $
**	$Revision: 513 $
**	$Date: 2006-03-20 19:05:12 +0100 (Mon, 20 Mar 2006) $
**
*/

#include "Ticket.h"
#include "Actions.h"

namespace mozia
{
	namespace ticket
	{
		Ticket::Ticket(string from, string to, string me, int action)
		{
			this->_from = from;
			this->_to = to;
			this->_me = me;
			this->_action = action;
			this->CData = NULL;
			this->_ttl = 0;
		}

		Ticket::Ticket()
		{
			this->_from = "";
			this->_to = "PROTOCOL";
			this->_drop = false;
			this->_me = "LISTENER";
			this->_action = NETWORK_NEWCLIENT;
			this->CData = NULL;
			this->_ttl = 0;
		}

		const string&	Ticket::GetFrom()
		{
			return (this->_from);
		}

		int Ticket::GetAction()
		{
			return (this->_action);
		}

		void*	Ticket::GetMyData()
		{
			return (this->_dataHash[this->_me]);
		}

		const string&	Ticket::GetType()
		{
			static string ret = "Ticket";

			return (ret);
		}

		void	Ticket::SetMyData(IModuleData *data)
		{
			if (this->_dataHash[this->_me] != NULL)
				delete this->_dataHash[this->_me];
			this->_dataHash[this->_me] = data;
		}
	}
}

