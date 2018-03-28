/*
**
**	$HeadURL: svn://svn.mozia.info/repo/zia/src/includes/CmdParser.h $
**	$Author: nico $
**	$Revision: 687 $
**	$Date: 2006-04-19 23:11:07 +0200 (Wed, 19 Apr 2006) $
**
*/

#ifndef __CMDPARSER_H__
# define __CMDPARSER_H__

# include <map>
# include <vector>
# include <string>
# include "utils/cmpfunc.h"

using namespace std;

namespace mozia
{
	namespace cmdparser
	{
		class CmdParser
		{
		public:
			CmdParser(int argc, char **argv);
			~CmdParser(void);
			vector <string *> *GetOpt(char opt);
		private:
			map<const char, vector<string *> *, mozia::utils::eqchr> _optHash;
			char*	_optarg;
			int		_optind;
			int		_getopt(int argc, char **argv, char *optstring);
			void	_usage(void);
			void	_addOpt(const char opt, char *value);
		};
	}
}
#endif //!__CMDPARSER_H__
