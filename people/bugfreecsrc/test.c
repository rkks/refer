#define USE_HRAND
#define USE_HDOSFH

#include "app.h"

USEWINASSERT

void APIENTRY ReportWinAssert( LPSTR lpFilename, int nLine )
{
    printf( "WinAssert: %s-%d (Press Enter) ", lpFilename, nLine );
    while ('\n'!=getchar()) {
        }

} /* ReportWinAssert */

int main()
{
    HRAND hRand1;
    HRAND hRand2;
    int nTest;

    printf( "***** Walking heap with no objects\n" );
    FmWalkHeap();

    printf( "***** Calling RandCreate() twice\n" );
    hRand1=RandCreate(3);
    hRand2=RandCreate(4);

    printf( "***** Calling RandNext() twice\n" );
    RandNext(hRand1);
    RandNext(hRand2);

    printf( "***** Walking the heap\n" );
    FmWalkHeap();

    printf( "***** Testing invalid handle detection (EXPECT WINASSERT ERROR)\n" );
    RandNext((HRAND)0x87654321);

    printf( "***** Calling RandDestroy()\n" );
    hRand1=RandDestroy(hRand1);
    hRand2=RandDestroy(hRand2);

    printf( "***** Walking the heap\n" );
    FmWalkHeap();

    nTest = printf( "***** Forcing a WinAssert error (EXPECT WINASSERT ERROR)\n" );
    WinAssert(nTest==9999) {
        printf( "ERROR 1\n" );
        }

    printf( "***** Testing WinAssert block\n" );
    WinAssert(nTest!=9999) {
        printf( "Block WinAssert worked\n" );
        }

    /*--- Testing strings ---*/
    NewScope {
        LPSTR lpS1;
        LPSTR lpS2;
        LPSTR lpS3;

        printf( "***** Testing strings\n" );
        NEWSTRING(lpS1,100);
        NEWSTRING(lpS2,1000);
        MYLSTRDUP(lpS3,"Testing");
        printf( "***** Walking the heap\n" );
        FmWalkHeap();
        FREE(lpS1);
        FREE(lpS2);
        FREE(lpS3);
        }

    /*--- ---*/
    NewScope {
        LPSTR lpMem=(LPSTR)0x87654321;
        printf( "***** FREE unallocated block (EXPECT WINASSERT ERROR)\n" );
        FREE(lpMem);
        }

    /*--- Testing Arrays ---*/
    NewScope {
        LPSTR lpS1;

        printf( "***** Testing arrays\n" );
        NEWARRAY(lpS1, 2);
        SIZEARRAY(lpS1, 100);
        SIZEARRAY(lpS1, 1);
        SIZEARRAY(lpS1, 1000);
        printf( "***** Walking the heap\n" );
        FmWalkHeap();
        FREE(lpS1);
        }

    /*--- Test DOS Module ---*/
    /*
    NewScope {
        HDOSFH hDosFh=DosOpenFile("dos.cpp");
        if (hDosFh) {
            char buffer[40];
            DosRead( hDosFh, buffer, sizeof(buffer) );
            buffer[sizeof(buffer)-1] = '\0';
            printf( "DOS first chars = '%s'\n", buffer );
            hDosFh = DosCloseFile( hDosFh );
            }
        else {
            printf( "DOS open failed\n" );
            }
        }
    */

    /*--- Final walk heap ---*/
    printf( "***** Final FmWalkHeap()\n" );
    FmWalkHeap();

    return 0;

} /* main */
