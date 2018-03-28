/*
	Full-screen application (Curses)
	AUP2, Sec. 4.08

	Copyright 2003 by Marc J. Rochkind. All rights reserved.
	May be copied only for purposes and under conditions described
	on the Web page www.basepath.com/aup/copyright.htm.

	The Example Files are provided "as is," without any warranty;
	without even the implied warranty of merchantability or fitness
	for a particular purpose. The author and his publisher are not
	responsible for any damages, direct or incidental, resulting
	from the use or non-use of these Example Files.

	The Example Files may contain defects, and some contain deliberate
	coding mistakes that were included for educational reasons.
	You are responsible for determining if and how the Example Files
	are to be used.

*/
/*
	<stdbool.h> and <curses.h> define bool incompatibly, so the following
	kludge separates the two.
*/
#ifndef FREEBSD
#define bool bool_aup
#endif
#include "defs.h"
#ifndef FREEBSD
#undef bool
#endif

/*[scrappc]*/
#include <curses.h>

/* "ec" macro for ERR (used by curses) */
#define ec_ERR(x) ec_cmp(x, ERR)

int main(void)
{
	int c;
	char s[100];
	bool ok = false;

	(void)initscr();
	ec_ERR( raw() )
	while (true) {
		ec_ERR( clear() )
		ec_ERR( mvaddstr(2, 9, "What do you want to do?") )
		ec_ERR( mvaddstr(3, 9, "1. Check out tape/DVD") )
		ec_ERR( mvaddstr(4, 9, "2. Reserve tape/DVD") )
		ec_ERR( mvaddstr(5, 9, "3. Register new member") )
		ec_ERR( mvaddstr(6, 9, "4. Search for title/actor") )
		ec_ERR( mvaddstr(7, 9, "5. Quit") )
		ec_ERR( mvaddstr(9, 9, "(Type item number to continue)") )
		ec_ERR( c = getch() )
		switch (c) {
		case '1':
		case '2':
		case '3':
		case '4':
			ec_ERR( clear() )
			snprintf(s, sizeof(s), "You typed %c", c);
			ec_ERR( mvaddstr(4, 9, s) )
			ec_ERR( mvaddstr(9, 9, "(Press any key to continue)") )
			ec_ERR( getch() )
			break;
		case '5':
			ok = true;
			EC_CLEANUP
		default:
			ec_ERR( beep() )
		}
	}

EC_CLEANUP_BGN
	(void)clear();
	(void)refresh();
	(void)endwin();
	exit(ok ? EXIT_SUCCESS : EXIT_FAILURE);
EC_CLEANUP_END
}
