/*pm--------------------------------------------------------------
 *
 *  OUTLINE:
 *
 *    This module REPLACES the memory management routines of
 *    the C run-time library.  As such, this new interface
 *    should be used exclusively.
 *
 *  IMPLEMENTATION:
 *
 *    A wrapper is provided around all memory objects that
 *    allows for run-time type checking, symbolic dumps of
 *    the heap and validation of heap pointers.
 *
 *  NOTES:
 *
 *    - YOU must code an FmIsPtrOk() that works properly for
 *      your environment.
 *
 *--------------------------------------------------------------*/

#include "app.h"

USEWINASSERT

/*--- Heap objects are aligned on sizeof(int) boundaries ---*/
#define ALIGNMENT (sizeof(int))
#define DOALIGN(num) (((num)+ALIGNMENT-1)&~(ALIGNMENT-1))
CompilerAssert(ISPOWER2(ALIGNMENT));

/*--- Declare what LPPREFIX/LPPOSTFIX are ---*/
typedef struct tagPREFIX  FAR*LPPREFIX;
typedef struct tagPOSTFIX FAR*LPPOSTFIX;

/*--- Prefix structure before every heap object---*/
typedef struct tagPREFIX {
    LPPREFIX lpPrev;           /* previous object in heap      */
    LPPREFIX lpNext;           /* next object in heap          */
    LPPOSTFIX lpPostfix;       /* ptr to postfix object        */
    LPSTR lpFilename;          /* filename ptr or NULL         */
    long lLineNumber;          /* line number or 0             */
    LPVOID lpMem;              /* FmNew() ptr of object        */
    LPCLASSDESC lpClassDesc;   /* class descriptor ptr or NULL */
    } PREFIX;

/*--- Postfix structure after every heap object ---*/
typedef struct tagPOSTFIX {
    LPPREFIX lpPrefix;
    } POSTFIX;

/*--- Verify alignment of prefix structure ---*/
CompilerAssert(!(sizeof(PREFIX)%ALIGNMENT));

/*--- Points to first object in linked list of heap objects ---*/
static LPPREFIX lpHeapHead=NULL;

/*--- Local prototypes ---*/
void LOCAL AddToLinkedList      ( LPPREFIX );
void LOCAL RemoveFromLinkedList ( LPPREFIX );
BOOL LOCAL VerifyHeapPointer    ( LPVOID );
void LOCAL RenderDesc           ( LPPREFIX, LPSTR );


/*pf--------------------------------------------------------------
 *
 *  DESCRIPTION: (Far Memory New)  JLJ
 *
 *    Allocate a new block of memory from the heap.
 *
 *  ARGUMENTS:
 *
 *    wSize       - Size of object to allocate
 *    lpClassDesc - Class descriptor for object (or NULL)
 *    lpFile      - Filename where object was allocated
 *    nLine       - Line number where object was allocated
 *
 *  RETURNS:
 *
 *    A long pointer to the memory object or NULL
 *
 *--------------------------------------------------------------*/

LPVOID APIENTRY FmNew( SIZET wSize, LPCLASSDESC lpClassDesc,
    LPSTR lpFile, int nLine )
{
  LPPREFIX lpPrefix;
  wSize = DOALIGN(wSize);
  lpPrefix=(LPPREFIX)malloc(sizeof(PREFIX)+wSize+sizeof(POSTFIX));
  if (lpPrefix) {
    AddToLinkedList( lpPrefix );
    lpPrefix->lpPostfix = (LPPOSTFIX)((LPSTR)(lpPrefix+1)+wSize);
    lpPrefix->lpPostfix->lpPrefix = lpPrefix;
    lpPrefix->lpFilename = lpFile;
    lpPrefix->lLineNumber = nLine;
    lpPrefix->lpMem = lpPrefix+1;
    lpPrefix->lpClassDesc = lpClassDesc;
    memset( lpPrefix->lpMem, 0, wSize );
    }
  else {
    AssertError;             /* Report out of memory error */
    }
  return(lpPrefix ? lpPrefix+1 : NULL);

} /* FmNew */


/*pf--------------------------------------------------------------
 *
 *  DESCRIPTION: (Far Memory Free)  JLJ
 *
 *    Free a block of memory that was previously allocated
 *    through FmNew().
 *
 *  ARGUMENTS:
 *
 *    lpMem - Heap pointer to free or NULL
 *
 *  RETURNS:
 *
 *    NULL
 *
 *--------------------------------------------------------------*/

LPVOID APIENTRY FmFree( LPVOID lpMem )
{
    if (VerifyHeapPointer(lpMem)) {
       LPPREFIX lpPrefix=(LPPREFIX)lpMem-1;
       SIZET wSize=(LPSTR)(lpPrefix->lpPostfix+1)-(LPSTR)lpPrefix;
       RemoveFromLinkedList( lpPrefix );
       memset( lpPrefix, 0, wSize );
       free(lpPrefix);
       }
    return (NULL);

} /* FmFree */


/*pf--------------------------------------------------------------
 *
 *  DESCRIPTION: (Far Memory String Dup)  JLJ
 *
 *    Helper function for the MYLSTRDUP() macro
 *
 *  ARGUMENTS:
 *
 *    lpS    - String to duplicate (or NULL)
 *    lpFile - Filename where string is being duplicated
 *    nLine  - Line number where string is being duplicated
 *
 *  RETURNS:
 *
 *    A pointer to the duplicated string or NULL
 *
 *--------------------------------------------------------------*/

LPVOID APIENTRY FmStrDup( LPSTR lpS, LPSTR lpFile, int nLine )
{
    LPVOID lpReturn=NULL;

    if (lpS) {
        SIZET wSize = (SIZET)(strlen(lpS)+1);
        lpReturn = FmNew( wSize, NULL, lpFile, nLine );
        if (lpReturn) {
            memcpy( lpReturn, lpS, wSize );
            }
        }
    return(lpReturn);

} /* FmStrDup */


/*pf--------------------------------------------------------------
 *
 *  DESCRIPTION: (Far Memory Realloc)  JLJ
 *
 *    Reallocate a block of memory
 *
 *  ARGUMENTS:
 *
 *    lpOld  - Heap object to reallocate or NULL
 *    wSize  - New size of the object
 *    lpFile - Filename where realloc is taking place
 *    nLIne  - Line number where realloc is taking place
 *
 *  RETURNS:
 *
 *    A pointer to the reallocated memory or NULL
 *
 *--------------------------------------------------------------*/

LPVOID APIENTRY FmRealloc( LPVOID lpOld, SIZET wSize,
    LPSTR lpFile, int nLine )
{
  LPVOID lpNew=NULL;

  /*--- Try to realloc ---*/
  if (lpOld) {
    if (VerifyHeapPointer(lpOld)) {
      LPPREFIX lpPrefix=(LPPREFIX)lpOld-1;
      LPPREFIX lpNewPrefix;
      LPPREFIX lpPre;

      /*--- Try to reallocate block ---*/
      RemoveFromLinkedList( lpPrefix );
      memset( lpPrefix->lpPostfix, 0, sizeof(POSTFIX) );
      wSize = DOALIGN(wSize);
      lpNewPrefix=(LPPREFIX)realloc(lpPrefix,
          sizeof(PREFIX)+wSize+sizeof(POSTFIX));

      /*--- Add new (or failed old) back in ---*/
      lpPre=(lpNewPrefix?lpNewPrefix:lpPrefix);
      AddToLinkedList( lpPre );
      lpPre->lpPostfix = (LPPOSTFIX)((LPSTR)(lpPre+1)+wSize);
      lpPre->lpPostfix->lpPrefix = lpPre;
      lpPre->lpMem = lpPre+1;

      /*--- Finish ---*/
      lpNew = (lpNewPrefix ? &lpNewPrefix[1] : NULL);
      if (!lpNew) {
        /* Report out of memory error */
        AssertError;
        }
      }
    }

  /*--- Else try new allocation ---*/
  else {
    lpNew = FmNew( wSize, NULL, lpFile, nLine );
    }

  /*--- Return address to object ---*/
  return(lpNew);

} /* FmRealloc */


/*pf--------------------------------------------------------------
 *
 *  DESCRIPTION: (Walk Heap)  JLJ
 *
 *    Display a symbolic dump of the heap by walking the
 *    heap and displaying all objects in the heap.
 *
 *  ARGUMENTS:
 *
 *    (void)
 *
 *  RETURNS:
 *
 *    (void)
 *
 *--------------------------------------------------------------*/

void APIENTRY FmWalkHeap( void )
{
    if (lpHeapHead) {
        LPPREFIX lpCur=lpHeapHead;
        while (VerifyHeapPointer(&lpCur[1])) {
            char buffer[100];
            RenderDesc( lpCur, buffer );
            /*--- print out buffer ---*/
            printf( "walk: %s\n", buffer );
            lpCur = lpCur->lpNext;
            if (lpCur==lpHeapHead) {
                break;
                }
            }
        }

} /* FmWalkHeap */


/*p---------------------------------------------------------------
 *
 *  DESCRIPTION: (Add Heap Object to Linked List)  JLJ
 *
 *    Add the given heap object into the doubly linked list
 *    of heap objects.
 *
 *  ARGUMENTS:
 *
 *    lpAdd - Prefix pointer to heap object
 *
 *  RETURNS:
 *
 *    (void)
 *
 *--------------------------------------------------------------*/

void LOCAL AddToLinkedList( LPPREFIX lpAdd )
{
    /*--- Add before current head of list ---*/
    if (lpHeapHead) {
        lpAdd->lpPrev = lpHeapHead->lpPrev;
        (lpAdd->lpPrev)->lpNext = lpAdd;
        lpAdd->lpNext = lpHeapHead;
        (lpAdd->lpNext)->lpPrev = lpAdd;
        }

    /*--- Else first node ---*/
    else {
        lpAdd->lpPrev = lpAdd;
        lpAdd->lpNext = lpAdd;
        }

    /*--- Make new item head of list ---*/
    lpHeapHead = lpAdd;

} /* AddToLinkedList */


/*p---------------------------------------------------------------
 *
 *  DESCRIPTION: (Remove Heap Object from Linked List)  JLJ
 *
 *    Remove the given heap object from the doubly linked list
 *    of heap objects.
 *
 *  ARGUMENTS:
 *
 *    lpRemove - Prefix pointer to heap object
 *
 *  RETURNS:
 *
 *    (void)
 *
 *--------------------------------------------------------------*/

void LOCAL RemoveFromLinkedList( LPPREFIX lpRemove )
{

    /*--- Remove from doubly linked list ---*/
    (lpRemove->lpPrev)->lpNext = lpRemove->lpNext;
    (lpRemove->lpNext)->lpPrev = lpRemove->lpPrev;

    /*--- Possibly correct head pointer ---*/
    if (lpRemove==lpHeapHead) {
        lpHeapHead = ((lpRemove->lpNext==lpRemove) ? NULL : 
            lpRemove->lpNext);
        }

} /* RemoveFromLinkedList */


/*p---------------------------------------------------------------
 *
 *  DESCRIPTION: (Verify Heap Pointer)  JLJ
 *
 *    Verify that a pointer points into that heap to a valid
 *    object in the heap.
 *
 *  ARGUMENTS:
 *
 *    lpMem - Heap pointer to validate
 *
 *  RETURNS:
 *
 *    Heap pointer is valid (TRUE) or not (FALSE)
 *
 *--------------------------------------------------------------*/

BOOL LOCAL VerifyHeapPointer( LPVOID lpMem )
{
  BOOL bOk=FALSE;

  if (lpMem) {
    WinAssert(FmIsPtrOk(lpMem)) {
      LPPREFIX lpPrefix=(LPPREFIX)lpMem-1;
      WinAssert(lpPrefix->lpMem==lpMem) {
        WinAssert(lpPrefix->lpPostfix->lpPrefix==lpPrefix) {
          bOk = TRUE;
          }
        }
      }
    }

  return (bOk);

} /* VerifyHeapPointer */


/*pf--------------------------------------------------------------
 *
 *  DESCRIPTION: (Does Pointer Point into the Heap)  JLJ
 *
 *    Does the given memory pointer point anywhere into
 *    the heap.
 *
 *  ARGUMENTS:
 *
 *    lpMem - Heap pointer to check
 *
 *  RETURNS:
 *
 *    Pointer points into the heap (TRUE) or not (FALSE)
 *
 *--------------------------------------------------------------*/

#if (defined(_WINDOWS) || defined(_WINDLL))
BOOL APIENTRY FmIsPtrOk( LPVOID lpMem )
{
    BOOL bOk=FALSE;
    _asm xor ax, ax                  ;; assume bad selector
    _asm lsl ax, word ptr [lpMem+2]  ;; get selector limit
    _asm cmp word ptr [lpMem], ax    ;; is ptr offset under limit
    _asm jae done                    ;; no, bad pointer
    _asm mov bOk, 1                  ;; yes, pointer OK
    _asm done:
    return (bOk);

} /* FmIsPtrOk */
#else
BOOL APIENTRY FmIsPtrOk( LPVOID lpMem )
{
    return ((lpMem) && (!((long)lpMem&(ALIGNMENT-1))));

} /* FmIsPtrOk */
#endif


/*p---------------------------------------------------------------
 *
 *  DESCRIPTION: (Render Description of Heap Object)  JLJ
 *
 *    Render a text description for the given heap object.
 *
 *  ARGUMENTS:
 *
 *    lpPrefix - Prefix pointer to heap object
 *    lpBuffer - Where to place text description
 *
 *  RETURNS:
 *
 *    (void)
 *
 *--------------------------------------------------------------*/

void LOCAL RenderDesc( LPPREFIX lpPrefix, LPSTR lpBuffer )
{
    if (lpPrefix->lpMem==&lpPrefix[1]) {
        sprintf( lpBuffer, "%08lx ", lpPrefix );
        if (lpPrefix->lpFilename) {
            sprintf( lpBuffer+strlen(lpBuffer), "%12s %4ld ",
                lpPrefix->lpFilename, lpPrefix->lLineNumber );
            }
        if (lpPrefix->lpClassDesc) {
            sprintf( lpBuffer+strlen(lpBuffer), "%s",
                lpPrefix->lpClassDesc->lpVarName );
            }
        }
    else {
        strcpy( lpBuffer, "(bad)" );
        }

} /* RenderDesc */
