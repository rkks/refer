/*
	Full-screen application (ANSI/VT100)
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
#include "defs.h"
#include "tc_setraw.h"

/*
	Only two VT100 escape sequences are used:

	CUP -- Cursor Position
		ESC [ Pn ; Pn H

	ED -- Erase In Display
		ESC [ Ps J
*/

/*[mvaddstr-clear]*/
#define ESC "\033"

bool mvaddstr(int y, int x, const char *str)
{
	return printf(ESC "[%d;%dH%s", y, x, str) >= 0;
}

bool clear(void)
{
	return printf(ESC "[2J") >= 0 &&
	  mvaddstr(0, 0, "");
}
/*[getch]*/
int getch(void)
{
	char c;

	switch(read(STDIN_FILENO, &c, 1)) {
	default:
		errno = 0;
		/* fall through */
	case -1:
		return -1;
	case 1:
		break;
	}
	return c;
}
/*[beep]*/
#define BEL "\007"

int beep(void)
{
	return printf(BEL) >= 0;
}
/*[main]*/
int main(void)
{
	int c;
	char s[100];
	bool ok = false;

	ec_false( tc_setraw() )
	setbuf(stdout, NULL);
	while (true) {
		ec_false( clear() )
		ec_false( mvaddstr( 2, 9, "What do you want to do?") )
		ec_false( mvaddstr( 3, 9, "1. Check out tape/DVD") )
		ec_false( mvaddstr( 4, 9, "2. Reserve tape/DVD") )
		ec_false( mvaddstr( 5, 9, "3. Register new member") )
		ec_false( mvaddstr( 6, 9, "4. Search for title/actor") )
		ec_false( mvaddstr( 7, 9, "5. Quit") )
		ec_false( mvaddstr( 9, 9, "(Type item number to continue)") )
		ec_neg1( c = getch() )
		switch (c) {
		case '1':
		case '2':
		case '3':
		case '4':
			ec_false( clear() )
			snprintf(s, sizeof(s), "You typed %c", c);
			ec_false( mvaddstr( 4, 9, s) )
			ec_false( mvaddstr( 9, 9, "(Press any key to continue)") )
			ec_neg1( getch() )
			break;
		case '5':
			ok = true;
			EC_CLEANUP
		default:
			ec_false( beep() )
		}
	}


EC_CLEANUP_BGN
	(void)tc_restore();
	(void)clear();
	exit(ok ? EXIT_SUCCESS : EXIT_FAILURE);
EC_CLEANUP_END
}
/*[]*/
