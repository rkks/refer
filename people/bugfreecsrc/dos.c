/****************************************************************/
/*                                                              */
/*                        (project name)                        */
/*                                                              */
/*    Copyright (date) (Company Name). All rights reserved.     */
/*                                                              */
/*    This program contains the confidential trade secret       */
/*    information of (Company Name).  Use, disclosure, or       */
/*    copying without written consent is strictly prohibited.   */
/*                                                              */
/****************************************************************/

/*pm--------------------------------------------------------------
 *
 *  OUTLINE:
 *
 *    This module provides access to the low-level file I/O
 *    functions of the standard Microsoft C run-time library.
 *
 *  IMPLEMENTATION:
 *
 *    This module is simply a code wrapper module.
 *
 *  NOTES:
 *
 *--------------------------------------------------------------*/

#define USE_LOWIO
#define USE_HDOSFH

#include "app.h"

USEWINASSERT

/*--- The class object ---*/
CLASS(hDosFh, HDOSFH) {
    int fh;
    };


/*pf--------------------------------------------------------------
 *
 *  DESCRIPTION: (Open File)  JLJ
 *
 *    Attempt to open a file
 *
 *  ARGUMENTS:
 *
 *    lpFilename - The name of the file to open
 *
 *  RETURNS:
 *
 *    A file object handle or NULL if there was some error
 *    in opening the specified file.
 *
 *--------------------------------------------------------------*/

HDOSFH APIENTRY DosOpenFile( LPSTR lpFilename )
{
    HDOSFH hDosFh=NULL;
    int fh=open(lpFilename, _O_RDWR|_O_BINARY);
    if (fh!=-1) {
        NEWOBJ(hDosFh);
        hDosFh->fh = fh;
        }
    return (hDosFh);

} /* DosOpenFile */


/*pf--------------------------------------------------------------
 *
 *  DESCRIPTION: (Close File)  JLJ
 *
 *    Close a previously opened file
 *
 *  ARGUMENTS:
 *
 *    hDosFh - The file object or NULL
 *
 *  RETURNS:
 *
 *    NULL
 *
 *--------------------------------------------------------------*/

HDOSFH APIENTRY DosCloseFile( HDOSFH hDosFh )
{

    VERIFYZ(hDosFh) {
        int nResult=close(hDosFh->fh);
        WinAssert(!nResult);
        FREE(hDosFh);
        }
    return (NULL);

} /* DosCloseFile */


/*pf--------------------------------------------------------------
 *
 *  DESCRIPTION: (Read File)  JLJ
 *
 *    Attempt to read a block of information from a file.
 *
 *  ARGUMENTS:
 *
 *    hDosFh - The file object
 *    lpMem  - Pointer to memory buffer
 *    wCount - Number of bytes to read into the memory buffer
 *
 *  RETURNS:
 *
 *    The number of bytes that were actually read
 *
 *--------------------------------------------------------------*/

WORD APIENTRY DosRead( HDOSFH hDosFh, LPVOID lpMem, WORD wCount )
{
    WORD wNumRead=0;

    VERIFY(hDosFh) {
        wNumRead = (WORD)read(hDosFh->fh, lpMem, wCount);
        }
    return (wNumRead);

} /* DosRead */


/*pf--------------------------------------------------------------
 *
 *  DESCRIPTION: (Write File)  JLJ
 *
 *    Attempt to write a block of information to a file.
 *
 *  ARGUMENTS:
 *
 *    hDosFh - The file object
 *    lpMem  - Pointer to memory buffer
 *    wCount - Number of bytes to write to the file
 *
 *  RETURNS:
 *
 *    The number of bytes that were actually written
 *
 *--------------------------------------------------------------*/

WORD APIENTRY DosWrite( HDOSFH hDosFh, LPVOID lpMem, WORD wCount )
{
    WORD wNumWritten=0;

    VERIFY(hDosFh) {
        wNumWritten = (WORD)write(hDosFh->fh, lpMem, wCount);
        }
    return (wNumWritten);

} /* DosWrite */
