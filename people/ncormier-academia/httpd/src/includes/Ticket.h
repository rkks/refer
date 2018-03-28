/*
**
**	$HeadURL: svn://epizia.homeip.net/repo/zia/src_common/includes/Ticket.h $
**	$Author: nico $
**	$Revision: 513 $
**	$Date: 2006-03-20 19:05:12 +0100 (Mon, 20 Mar 2006) $
**
*/

#ifndef __TICKET_H__
# define __TICKET_H__

# include <string>
# include <map>
# include <iostream>

# include "IModuleData.h"
# include "ICData.h"

using namespace std;
using namespace mozia::modules;

namespace mozia
{

# ifdef __CORE__
	namespace core
	{
		class Core;
	}
# endif

	namespace ticket
	{

		class Ticket
		{
			friend class TicketsHandler;
# ifdef __CORE__
			friend class mozia::core::Core;
# endif

		public:
			Ticket();
			Ticket(string from, string to, string me, int action);

			virtual const string&	GetType();
			const string&			GetFrom();
			int						GetAction();
			void*					GetMyData();
			void					SetMyData(IModuleData *ptr);
			ICData*					CData;

		private:
#ifndef _WIN32
		public:
#endif
			string							_me;		//valeur = le nom du module courant (set par le noyau)
			string							_from;		//set par le noyau
			string							_to;		//set a null par le Core avt de passer le ticket
			int								_action;
			bool							_drop;
			std::map <string, IModuleData*> _dataHash;
			int								_ttl;
		};

	}
}
#endif // __TICKET_H__
