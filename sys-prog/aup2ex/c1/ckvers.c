/*
	Program to display POSIX and X/Open version info
	AUP2, Sec. 1.5 (not in book)
	Not written by Marc Rochkind.

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
 * (C) Copyright 1996 X/Open Company Limited
 *
 * Permission to use, copy, modify, and distribute this software and its
 * documentation for any purpose and without fee is hereby granted, provided
 * that the above copyright notice appear in all copies and that both that
 * copyright notice and this permission notice appear in supporting
 * documentation, and that the name of X/OPEN not be used in
 * advertising or publicity pertaining to distribution of the software
 * without specific, written prior permission.  X/OPEN make
 * no representations about the suitability of this software for any purpose.
 * It is provided "as is" without express or implied warranty.
 *
 * X/OPEN DISCLAIM ALL WARRANTIES WITH REGARD TO THIS SOFTWARE,
 * INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO
 * EVENT SHALL X/OPEN BE LIABLE FOR ANY SPECIAL, INDIRECT OR
 * CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF
 * USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR
 * OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 * PERFORMANCE OF THIS SOFTWARE.
 *
 * X/Open and the 'X' symbol are trademarks of X/Open Company Limited in
 * the UK and other countries.
 */

/* ckvers.c
 * A simple programme to print out the POSIX version
 * and the X/Open version claimed by the system
 * It also does some simple validity checks on the combination
 * of values returned.
 *
 * Author: Andrew Josey (ajosey@xopen.org)
 * Any suggestions/improvements should be sent to the author.
 *
 * Version: 1.0
 *
 * Date: 23 Sep 1996 - Original version.
 *
 * Version 1.1 8th June 1998
 *
 * Update referenced URL to be  http://www.opengroup.org/
 * and other changes to cope with UNIX 98
 * This program should now be compiled with the -DCKVERS_UNIX98
 * switch when checking for UNIX 98
 *
 * Version 2.0 16th Sept 2002
 *
 * Updated by Marc Rochkind (rochkind@basepath.com) to include
 * the Single UNIX Specification Version 3 (SUSv3).
 * This program should be compiled with -DCKVERS_SUSV3 to
 * enable checking for SUSV3. (Note: Some systems capable
 * of Unix98 support (_XOPEN_VERSION == 500) when _XOPEN_SOURCE is
 * set to 500 revert to _XOPEN_VERSION == 4 if _XOPEN_SOURCE is
 * set to 600. So, if you don't get 600 when you set -DCKVERS_SUSV3,
 * you should go ahead and try -DCKVERS_UNIX98 as well.
 *
 * At the time of writing the brand name for the UNIX mark for
 * the Single UNIX Specification Version 3 is yet to be finalized.
 *
 */

#define _POSIX_SOURCE
#ifdef CKVERS_UNIX98   /* command-line def to switch to UNIX 98 check */
#define _XOPEN_SOURCE 500  /* for UNIX 98 */
#elif CKVERS_SUSV3
#define _XOPEN_SOURCE 600  /* for the Single UNIX Specification Version 3 */
#else
#define _XOPEN_SOURCE
#define _XOPEN_SOURCE_EXTENDED 1
#endif

#include <unistd.h>
#include <stdio.h>
#ifdef _POSIX_VERSION
#include <sys/utsname.h>
#endif

/* define TESTBED to emulate SUSv3 */
#ifdef TESTBED
#undef _XOPEN_XCU_VERSION
#define _XOPEN_VERSION 600
#define _POSIX_VERSION 200112L
#define _POSIX2_VERSION 200112L
#endif

#define CKVERS_VERSION "Ckvers Release 2.0 16-Sept-2002"

int main(void)
{
	int ckvers_posix, ckvers_xsh, ckvers_xcu, ckvers_xopen_unix =0;
	int ckvers_posix2clb, ckvers_posix2 =0;
#ifdef _POSIX_VERSION
	struct utsname ckvers_unames;
#endif

	(void) printf("\t\t%s\n\t\tSUMMARY REPORT\n\n",CKVERS_VERSION);

#ifdef _POSIX_VERSION
	if (uname(&ckvers_unames) != -1)
     	{
         (void) printf("\tSystem: %s %s %s %s %s\n",
              ckvers_unames.sysname, ckvers_unames.nodename,
              ckvers_unames.release, ckvers_unames.version,
              ckvers_unames.machine);
	}

#ifdef CKVERS_UNIX98
	(void) printf("\n\tNotice: Compiled to check for UNIX 98 claims\n");
	(void) printf("\tIf warnings occur, recompile with -DCKVERS_SUSV3\n");
	(void) printf("\tor without any version symbol.\n");
#elif CKVERS_SUSV3
	(void) printf("\n\tNotice: Compiled to check for SUS v3 claims\n");
	(void) printf("\tIf warnings occur, recompile with -DCKVERS_UNIX98\n");
	(void) printf("\tor without any version symbol.\n");
#endif

	(void) printf("\n\tPOSIX SUPPORT: ");

	switch (_POSIX_VERSION) {

	case 199009L: /* classic dot1 - ISO version */
		(void) printf("IEEE Std POSIX.1-1990/ISO 9945-1:1990\n");
		ckvers_posix=1990;
		break;
	case 198808L:  /* classic dot 1 - non ISO version */
		(void) printf("IEEE Std POSIX.1-1988\n");
		ckvers_posix=1988;
		break;
	case 199309L: /* POSIX realtime */
		(void) printf("IEEE Std POSIX.1b-1993 [Realtime]\n");
		ckvers_posix=1993;
		break;
	case 199506L:  /* POSIX threads */
		(void) printf("IEEE Std POSIX.1-1996/ISO 9945-1:1996\n");
		ckvers_posix=1995;
		break;
	case 200112L:  /* the POSIX 1003.1 revision,
		replaces 1003.1-1990 and all its amendments,
		and replaces 1003.2-1992 and all its amendments.
		Developed by the Austin Group.
		ISO Status: Draft International Standard (Sep 2002)
		DIS 9945-1 (Base Definitions),
		DIS 9945-2 (System Interfaces),
		DIS 9945-3 (Shell and Utilities),
		DIS 9945-4 (Rationale) */
		(void) printf("IEEE Std POSIX.1-2001\n");
		ckvers_posix=2001;
		break;
	default:
		(void) printf("Unknown _POSIX_VERSION=%ldL\n", _POSIX_VERSION);
	}

/* POSIX.2 C Language Bindings */
#if (_XOPEN_VERSION - 600 == 0) || (_POSIX_VERSION == 200112L )
	/*
		For 1003.1-2001/SUSv3 _POSIX2_C_VERSION has been removed.
	        since the functionality is now mandatory
	*/

	(void) printf("\tFor POSIX 1003.1-2001:\n\t\tthe former ISO POSIX.2 C Language Binding is required.\n");
#else
	(void) printf("\tPOSIX.2 C Language Binding: ");

#ifdef _POSIX2_C_VERSION
	if ( _POSIX2_C_VERSION == 199209L ) {
		ckvers_posix2clb = 1;
		(void) printf("supported\n");
	}
	else
		(void) printf("not supported\n");
#else
	(void) printf("not supported\n");
#endif
#endif /* _XOPEN_VERSION */
/* POSIX.2 Shell & Utilities */
	(void) printf("\tPOSIX Shell & Utilities: ");
#ifdef _POSIX2_VERSION
	/*
		_POSIX2_VERSION has been incremented for POSIX 1003.1-2001
	*/
	if ( _POSIX2_VERSION == 199209L ) {
		ckvers_posix2 = 1;
		(void) printf("ISO POSIX 1003.2-1993 supported\n");
	}
	else if ( _POSIX2_VERSION == 200112L ) {
		ckvers_posix2 = 2001;
		(void) printf("POSIX 1003.1-2001 Shell & Utilities\n");
	}
	else
		(void) printf("not supported\n");
#else
	(void) printf("not supported\n");
#endif


#else
(void) printf("\n\tPOSIX is not supported\n");
#endif


	(void) printf("\n\tX/OPEN SUPPORT:\n");
#ifdef _XOPEN_VERSION
	(void) printf("\tSYSTEM INTERFACES & HEADERS: ");
	switch (_XOPEN_VERSION) {

	case 3:
		(void) printf("XPG3\n");
		ckvers_xsh=3;
		break;
	case 4:
		(void) printf("XPG4\n");
		ckvers_xsh=4;
		break;
	case 500:
		(void) printf("XSH 5 claimed for UNIX 98\n");
		ckvers_xsh=500;
		break;
	case 600:
		(void) printf("XSH 6 claimed for SUS v.3\n");
		ckvers_xsh=600;
		break;
	default:
		(void) printf("Unknown _XOPEN_VERSION=%d\n", _XOPEN_VERSION);
	}
#else
	(void) printf("\tSYSTEM INTERFACES & HEADERS NOT SUPPORTED\n");
#endif


#if (_XOPEN_VERSION - 600 == 0)
	(void) printf("\tCOMMANDS & UTILITIES: ");
	/*
		For the Single UNIX Specification Version 3 ,
		the value is unspecified
	*/
	ckvers_xcu=6;
	(void) printf("XCU6/IEEE Std POSIX 1003.1-2001\n");

#elif _XOPEN_XCU_VERSION
	(void) printf("\tCOMMANDS & UTILITIES: ");
	switch (_XOPEN_XCU_VERSION) {

	case 3:
		(void) printf("XPG3/ XPG4 Historical Commands\n");
		ckvers_xcu=3;
		break;
	case 4:
		(void) printf("XPG4/IEEE Std POSIX.2-1992/ISO 9945-2:1993\n");
		ckvers_xcu=4;
		break;
	case 5:
		(void) printf("XCU5/IEEE Std POSIX.2-1992/ISO 9945-2:1993\n");
		ckvers_xcu=5;
		break;
	default:
		(void) printf("Unknown _XOPEN_XCU_VERSION=%d\n", _XOPEN_XCU_VERSION);
		break;
	}
#endif
	if (( ckvers_xcu >= 4 ) && (ckvers_posix2 < 1) )
		(void) printf("\tWarning: POSIX Shell & Utilities not supported, but required when XCU 4 or later is claimed (%ld)\n\n", _POSIX_VERSION); /* fmt added by mjr */
	else  if (ckvers_xcu == 0)
		(void) printf("\tCOMMANDS & UTILITIES NOT SUPPORTED\n");


#ifdef _XOPEN_UNIX
	(void) printf("\tX/OPEN UNIX SUPPORT IS CLAIMED\n");
	ckvers_xopen_unix=1;
#else
	(void) printf("\tX/OPEN UNIX is not supported\n");
#endif

/* check valid combinations */

/* for XSH6 _XOPEN_XCU_VERSION is unspecified */

#if (_XOPEN_VERSION - 600 == 0)

	if ( ckvers_xopen_unix == 1 )  {

	if ( ckvers_xsh == 600) {
		if ( ckvers_posix < 2001) {
			(void) printf("\tWarning: Invalid value found for POSIX support (%ld) when XSH6 support (for SUS v.3) is claimed\n", _POSIX_VERSION);
			(void) printf("\tWarning: Expected _POSIX_VERSION equals 200112L , got %ld\n", _POSIX_VERSION);
		}
		if ( (ckvers_posix2 < 2001))
			(void) printf("\tWarning: Unexpected value found for Commands & Utilities (%d) when XCU6 is claimed\n", _POSIX2_VERSION);
			(void) printf("\tWarning: Expected _POSIX2_VERSION equals 200112L , got %ld\n", _POSIX2_VERSION);
	}

	(void) printf("\n\tIf there are no Warnings output above, then");

	(void) printf("\n\tThis could be a UNIX system conforming to the Single UNIX Specification Version 3");
	(void) printf("\n\tPlease check the branded products catalogue for confirmation\n\thttp://www.opengroup.org/regproducts/\n");
	}

#elif (defined(_POSIX_SOURCE) && defined(_XOPEN_SOURCE) && defined(_XOPEN_XCU_VERSION) && defined(_XOPEN_VERSION))

	if ( ckvers_xopen_unix == 1 )  {

		if (  ckvers_xcu != 4 && ckvers_xcu != 5 ) {
			(void) printf("\tWarning: Invalid value found for Commands & Utilities (%d) when X/OPEN UNIX is claimed\n", _XOPEN_XCU_VERSION);
		}
		if ( ckvers_xsh != 4 && ckvers_xsh != 500 && ckvers_xsh != 600)
			(void) printf("\tWarning: Invalid value found for System Interfaces & Headers (%d) when X/OPEN UNIX is claimed\n", _XOPEN_VERSION);
		if ( ckvers_posix < 1990 )
			(void) printf("\tWarning: Invalid value found for POSIX support (%ld) when X/OPEN UNIX is claimed\n", _POSIX_VERSION);
		if ( ckvers_posix2clb < 1 )
			(void) printf("\tWarning: POSIX.2 CLB not supported, but required when X/OPEN UNIX is claimed (%ld)\n\n", _POSIX_VERSION); /* fmt added by mjr */

	}

	if ( ckvers_xsh == 4) {
		if ( ckvers_posix < 1990)
			(void) printf("\tWarning: Invalid value found for POSIX support (%ld) when XPG4 is claimed\n", _POSIX_VERSION);

 	if ( (ckvers_xcu != 3) && (ckvers_xcu != 4))
			(void) printf("\tWarning: Invalid value found for Commands & Utilities (%d) when XPG4 is claimed\n", _XOPEN_XCU_VERSION);

	}
	if ( ckvers_xsh == 500) {
		if ( ckvers_posix < 1995)
			(void) printf("\tWarning: Invalid value found for POSIX support (%ld) when XSH5 support (for UNIX 98) is claimed\n", _POSIX_VERSION);
		if ( (ckvers_xcu != 5)) {
			(void) printf("\tWarning: Unexpected value found for Commands & Utilities (%d) when XSH5 is claimed\n", _XOPEN_XCU_VERSION);
			(void) printf("\tWarning: Expected _XOPEN_XCU_VERSION equals 5 , got %ld\n", (long)_XOPEN_XCU_VERSION);
		}
	}

	if ( ckvers_xopen_unix == 1 )  {

		(void) printf("\n\tIf there are no Warnings output above, then");

		(void) printf("\n\tThis could be a UNIX system conforming to the Single UNIX Specification");
#if _XOPEN_VERSION - 500 == 0
		(void) printf(", Version 2\n");
#else
		(void) printf("\n");
#endif
		(void) printf("\n\tPlease check the branded products catalogue for confirmation\n\thttp://www.opengroup.org/regproducts/\n");
	} else {

		if (ckvers_xsh == 4) {
			if ( ckvers_xcu == 3 )
				(void) printf("\n\tThis could be an XPG4 Base system\n");
			else if (ckvers_xcu == 4)
				(void) printf("\n\tThis could be an XPG4 Base or XPG4 Base 95 system\n");
		(void) printf("\n\tPlease check the branded products catalogue for confirmation\n\thttp://www.opengroup.org/regproducts/\n");
		}
		else if (ckvers_xsh == 3) {
				(void) printf("\n\tThis could be an XPG3 Base system\n");
				(void) printf("\n\tPlease check the branded products catalogue for confirmation\n\thttp://www.opengroup.org/regproducts/\n");
		}
	}
#endif
	return 0;
}

