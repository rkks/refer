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

/* ckstdvers.c
 *
 * A simple test programme to print out the POSIX version
 * and the X/Open version claimed by the system
 * It also does some simple validity checks on the combination
 * of values returned.  A sister programme ckvers does
 * similar things and attempts to put out a pretty report.
 * This program is intended to look at more specific details.
 *
 * Author: Andrew Josey (ajosey@xopen.org)
 * If you have any suggestions or improvements please send
 * them to the author.
 *
 * Version: 0.1
 * Date: 15 Sep 1996
 *
 * Version: 0.2
 * Date: 18 Sep 1996
 * Updated by adding _POSIX_JOB_CONTROL, _POSIX_SAVED_IDS, _POSIX2_C_VERSION
 * and _POSIX2_VERSION
 *
 * Version: 0.3
 * Date: 9 June 1998
 * Add in UNIX 98 support. Checks for _POSIX_THREADS
 */

#define _POSIX_SOURCE
/* Note for that UNIX 98 that _XOPEN_SOURCE should be set to 500 */

#define _XOPEN_SOURCE 500 /* mjr added 500; was blank */
#define _XOPEN_SOURCE_EXTENDED 1

#include <unistd.h>
#include <stdio.h>

int main(void)
{

	int posix, xsh, xcu, xopen_unix =0;
	long user_sc_value;

#ifdef _POSIX_VERSION
	switch (_POSIX_VERSION) {

	case 199009L: /* classic dot1 - ISO version */
		(void) printf("_POSIX_VERSION=%ldL (ISO 9945-1:1990[IEEE Std POSIX.1-1990])\n", _POSIX_VERSION);
		posix=90;
		break;
	case 198808L:  /* classic dot 1 - non ISO version */
		(void) printf("_POSIX_VERSION=%ldL (IEEE Std POSIX.1-1988)\n", _POSIX_VERSION);
		posix=88;
		break;
	case 199309L: /* POSIX realtime */
		(void) printf("_POSIX_VERSION=%ldL (IEEE Std POSIX.1b-1993)\n", _POSIX_VERSION);
		posix=93;
		break;
	case 199506L:  /* POSIX threads */
		(void) printf("_POSIX_VERSION=%ldL (ISO 9945-1:1996 [IEEE Std POSIX.1-1996])\n", _POSIX_VERSION);
		posix=95;
		break;
	default:
		(void) printf("Unknown _POSIX_VERSION=%ldL\n", _POSIX_VERSION);
		break;
	}

	/* check consistency with sysconf */
	user_sc_value = sysconf(_SC_VERSION);
	if ( user_sc_value != _POSIX_VERSION )
		(void) printf("Warning: sysconf(_SC_VERSION) returned %ld, expected %ld\n", user_sc_value, _POSIX_VERSION); /* mjr changed from dL to ld */


#else
(void) printf("_POSIX_VERSION not defined\n");
#endif

#ifdef _POSIX_JOB_CONTROL
	(void) printf("\t_POSIX_JOB_CONTROL supported\n");
#else
	(void) printf("\t_POSIX_JOB_CONTROL not supported\n");
#endif

#ifdef _POSIX_SAVED_IDS
	(void) printf("\t_POSIX_SAVED_IDS supported\n");
#else
	(void) printf("\t_POSIX_SAVED_IDS not supported\n");
#endif

	if (posix >= 95 ) {
#ifdef _POSIX_THREADS
	(void) printf("\t_POSIX_THREADS supported\n");
#else
	(void) printf("\t_POSIX_THREADS not supported\n");
#endif
#ifdef _POSIX_THREAD_SAFE_FUNCTIONS
	(void) printf("\t_POSIX_THREAD_SAFE_FUNCTIONS supported\n");
#else
	(void) printf("\t_POSIX_THREAD_SAFE_FUNCTIONS not supported\n");
#endif
#ifdef _POSIX_THREAD_ATTR_STACKADDR
	(void) printf("\t_POSIX_THREAD_ATTR_STACKADDR supported\n");
#else
	(void) printf("\t_POSIX_THREAD_ATTR_STACKADDR not supported\n");
#endif
#ifdef _POSIX_THREAD_ATTR_STACKSIZE
	(void) printf("\t_POSIX_THREAD_ATTR_STACKSIZE supported\n");
#else
	(void) printf("\t_POSIX_THREAD_ATTR_STACKSIZE not supported\n");
#endif
	}



#ifdef _XOPEN_VERSION
	switch (_XOPEN_VERSION) {

	case 3:
		(void) printf("_XOPEN_VERSION=%d (XPG3 System Interfaces)\n", _XOPEN_VERSION);
		xsh=3;
		break;
	case 4:
		(void) printf("_XOPEN_VERSION=%d (XPG4 System Interfaces)\n", _XOPEN_VERSION);
		xsh=4;
		break;
	case 500:
		(void) printf("_XOPEN_VERSION=%d (XSH5 System Interfaces)\n", _XOPEN_VERSION);
		xsh=5;
		break;
	default:
		(void) printf("Unknown _XOPEN_VERSION=%d\n", _XOPEN_VERSION);
		break;
	}

#ifdef _POSIX2_C_VERSION
	if ( _POSIX2_C_VERSION == 199209L )
		(void) printf("_POSIX2_C_VERSION=199209L : ISO POSIX.2 C Languages Binding is supported\n");
	else
		(void) printf("_POSIX2_C_VERSION != 199209L : ISO POSIX.2 C Language Binding not supported\n");
#else
	(void) printf("_POSIX2_C_VERSION not defined: ISO POSIX.2 C Language Binding  not supported\n");
#endif

#else
(void) printf("_XOPEN_VERSION not defined\n");
#endif


#ifdef _XOPEN_XCU_VERSION
	switch (_XOPEN_XCU_VERSION) {

	case 3:
		(void) printf("_XOPEN_XCU_VERSION=%d (Historical Commands)\n", _XOPEN_XCU_VERSION);
		xcu=3;
		break;
	case 4:
		(void) printf("_XOPEN_XCU_VERSION=%d (POSIX.2 Commands)\n", _XOPEN_XCU_VERSION);
		xcu=4;
		break;
	case 5:
		(void) printf("_XOPEN_XCU_VERSION=%d (POSIX.2 Commands)\n", _XOPEN_XCU_VERSION);
		xcu=5;
		break;
	default:
		(void) printf("Unknown _XOPEN_XCU_VERSION=%d\n", _XOPEN_XCU_VERSION);
		break;
	}
#else
(void) printf("_XOPEN_XCU_VERSION not defined\n");
#endif

#ifdef _POSIX2_VERSION
	if ( _POSIX2_VERSION == 199209L )
		(void) printf("_POSIX2_VERSION=199209L : ISO POSIX.2 is supported\n");
	else
		(void) printf("_POSIX2_VERSION != 199209L : ISO POSIX.2 not supported\n");
#else
	(void) printf("_POSIX2_VERSION not defined: ISO POSIX.2 not supported\n");
#endif

#ifdef _XOPEN_UNIX
	(void) printf("_XOPEN_UNIX support is claimed\n");
	xopen_unix=1;
#else
	(void) printf("_XOPEN_UNIX is not supported\n");
#endif

/* check valid combinations */

#if (defined(_POSIX_SOURCE) && defined(_XOPEN_SOURCE) && defined(_XOPEN_XCU_VERSION) && defined(_XOPEN_VERSION))

	if ( xopen_unix == 1 )  {

		if (  xcu != 4 && xcu != 5 )
			(void) printf("Invalid value found for _XOPEN_XCU_VERSION (%d) when _XOPEN_UNIX is supported\n", _XOPEN_XCU_VERSION);
		if ( xsh != 4  && xsh != 5 ) /* mjr changed from 500 to 5 */
			(void) printf("Invalid value found for _XOPEN_VERSION (%d) when _XOPEN_UNIX is supported\n", _XOPEN_VERSION);
		if ( posix < 90 )
			(void) printf("Invalid value found for _POSIX_VERSION (%ld) when _XOPEN_UNIX is supported\n", _POSIX_VERSION);

	}

	if ( xsh == 4) {
		if ( posix < 90)
			(void) printf("Invalid value found for _POSIX_VERSION (%ld) when _XOPEN_VERSION is set to 4\n", _POSIX_VERSION);

 	if ( (xcu != 3) && (xcu != 4))
			(void) printf("Invalid value found for _XOPEN_XCU_VERSION (%d) when _XOPEN_VERSION is set to 4\n", _XOPEN_XCU_VERSION);

	}
#endif
	return 0;
}
