/*
**
**	$HeadURL: svn://epizia.homeip.net/repo/zia/src/includes/CharT.h $
**	$Author: nico $
**	$Revision: 241 $
**	$Date: 2005-11-15 08:44:39 -0800 (Tue, 15 Nov 2005) $
**
*/

#pragma once


namespace mozia
{
	namespace utils
	{
		class CharT
		{
		public:
			CharT(const char*);
			~CharT(void);
			void Set(const char*);
			void Cat(const char*);
		private:
			char*			buf;
			unsigned int	len;
			unsigned int	buflen;
		};
	}
}
