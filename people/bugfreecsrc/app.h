/*--- Standard Includes ----------------------------------------*/
#include <string.h>                     /* strcpy/memset        */
#include <stdio.h>                      /* sprintf              */
#include <stdlib.h>                     /* malloc/free          */

/*--- The include file for this book ---------------------------*/
#include "book.h"

/*--- NEWHANDLE section ----------------------------------------*/
NEWHANDLE(HRAND);
NEWHANDLE(HDOSFH);

/*--- USE_* section --------------------------------------------*/

#ifdef USE_HRAND
/*----------------------------------------------------------------
 *
 *  Random number generator
 *
 *--------------------------------------------------------------*/

EXTERNC HRAND APIENTRY RandCreate  ( int );
EXTERNC HRAND APIENTRY RandDestroy ( HRAND );
EXTERNC int   APIENTRY RandNext    ( HRAND );
#endif

#ifdef USE_LOWIO
/*--------------------------------------------------------------
 *
 *  Access to low-level I/O run-time library functions
 *
 *-------------------------------------------------------------*/

#include <fcntl.h>
#include <sys\types.h>
#include <sys\stat.h>
#include <io.h>
#endif

#ifdef USE_HDOSFH
/*--------------------------------------------------------------
 *
 *  Code wrapper to low-level file I/O
 *
 *-------------------------------------------------------------*/

EXTERNC HDOSFH APIENTRY DosOpenFile  ( LPSTR );
EXTERNC WORD   APIENTRY DosRead      ( HDOSFH, LPVOID, WORD );
EXTERNC WORD   APIENTRY DosWrite     ( HDOSFH, LPVOID, WORD );
EXTERNC HDOSFH APIENTRY DosCloseFile ( HDOSFH );
#endif
