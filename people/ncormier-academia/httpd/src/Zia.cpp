/*
**
**	$HeadURL: svn://svn.mozia.info/repo/zia/src/Zia.cpp $
**	$Author: nico $
**	$Revision: 687 $
**	$Date: 2006-04-19 23:11:07 +0200 (Wed, 19 Apr 2006) $
**
*/

#include "Core.h"
#include "Exceptions.h"

using namespace mozia::exceptions;

using namespace mozia::core;

int main(int argc, char **argv)
{
	try
	{
		mozia::core::Core ziaCore(new CmdParser(argc, argv));
	}
	catch (ZiaException* ziaEx)
	{
		cerr << ziaEx->ToString() << endl;
		throw;
	}
	return (0);
}
