/*--- If target is C8 segmented architecture ---*/
#if (defined(_MSC_VER) && defined(MSDOS))
#pragma message("BOOK.H: Using Microsoft SEGMENTED architecture defines")
#define FAR _far
#define NEAR _near
#define FASTCALL _fastcall
#define PASCAL _pascal
#define EXPORT _export
#define BASEDIN(seg) _based(_segname(#seg))

/*--- Else assume target is flat memory model (Unix/etc) ---*/
#else
#pragma message("BOOK.H: Using FLAT/UNIX architecture defines")
#define FAR
#define NEAR
#define FASTCALL
#define PASCAL
#define EXPORT
#define BASEDIN(seg)
#endif

/*--- size_t mapping ---*/
#define SIZET size_t

/*--- Useful defines ---*/
#ifndef _WINDOWS
typedef char FAR*LPSTR;
typedef void FAR*LPVOID;
typedef unsigned short WORD;
typedef int BOOL;
#define FALSE (0)
#define TRUE (1)
#endif

/*--- Assert during compiling (not run-time) ---*/
#define CompilerAssert(exp) extern char _CompilerAssert[(exp)?1:-1]

/*--- TYPEDEF depends upon C/C++ ---*/
#ifdef __cplusplus
#define TYPEDEF
#else
#define TYPEDEF typedef
#endif

/*--- EXTERNC depends upon C/C++ ---*/
#ifdef __cplusplus
#define EXTERNC extern "C"
#else
#define EXTERNC
#endif

/*--- APIENTRY for app/dll ---*/
#ifdef _WINDLL
#define APIENTRY EXPORT FAR PASCAL
#else
#define APIENTRY FAR PASCAL
#endif

/*--- LOCAL/LOCALASM defines ---*/
#define LOCAL static NEAR FASTCALL
#define LOCALASM static NEAR PASCAL

/*--- Other useful defines ---*/
#define CSCHAR static char BASEDIN(_CODE)
#define NewScope

/*--- Absolute value macro ---*/
#define ABS(x) (((x)>0)?(x):-(x))

/*--- Is a number a power of two ---*/
#define ISPOWER2(x) (!((x)&((x)-1)))

/*--- Number of static elements in an array ---*/
#define NUMSTATICELS(pArray) (sizeof(pArray)/sizeof(*pArray))

/*--- Loop Macros ---*/
#define LOOP(nArg) { int _nMax=nArg; int loop; \
    for (loop=0; loop<_nMax; ++loop)
#define LLOOP(lArg) { long _lMax=lArg; long lLoop; \
    for (lLoop=0; lLoop<_lMax; ++lLoop)
#define ENDLOOP }

/*--- WinAssert support ---*/
#define USEWINASSERT CSCHAR szSRCFILE[]=__FILE__;   \
  BOOL static NEAR _DoWinAssert( int nLine ) {      \
    ReportWinAssert(szSRCFILE, nLine);              \
    WinAssert(nLine);                               \
    return(FALSE);                                  \
    }
#define AssertError _DoWinAssert(__LINE__)
#define WinAssert(exp) if (!(exp)) {AssertError;} else
EXTERNC void APIENTRY ReportWinAssert( LPSTR, int );

/*--- What is a class descriptor---*/
typedef struct {
    LPSTR lpVarName;
    } CLASSDESC, FAR*LPCLASSDESC;

/*--- Declare a new handle ---*/
#define NEWHANDLE(Handle) typedef struct tag##Handle *Handle

/*--- Class descriptor name from object name ---*/
#define _CD(hObj) hObj##_ClassDesc

/*--- The class macro ---*/
#define CLASS(hObj,Handle) \
    static CLASSDESC _CD(hObj)={#hObj}; TYPEDEF struct tag##Handle

/*--- Object verification macros---*/
#define VERIFY(hObj) WinAssert(_VERIFY(hObj))
#define VERIFYZ(hObj) if (!(hObj)) {} else VERIFY(hObj)

/*--- WARNING: _VERIFY needs be tailored to your environment ---*/
#define _S4 (sizeof(LPCLASSDESC))
#define _S8 (sizeof(LPCLASSDESC)+sizeof(LPVOID))
#define _VERIFY(hObj) \
    ( FmIsPtrOk(hObj) && \
    (((LPVOID)hObj)==*(LPVOID FAR*)((LPSTR)hObj-_S8)) \
    && ((&_CD(hObj))==*(LPCLASSDESC FAR*)((LPSTR)hObj-_S4)) )

/*--- Heap manager prototypes ---*/
EXTERNC LPVOID APIENTRY FmNew      ( SIZET, LPCLASSDESC, LPSTR, int );
EXTERNC LPVOID APIENTRY FmFree     ( LPVOID );
EXTERNC LPVOID APIENTRY FmRealloc  ( LPVOID, SIZET, LPSTR, int );
EXTERNC LPVOID APIENTRY FmStrDup   ( LPSTR, LPSTR, int );
EXTERNC void   APIENTRY FmWalkHeap ( void );
EXTERNC BOOL   APIENTRY FmIsPtrOk  ( LPVOID );

/*--- NEWOBJ() and FREE() Interface ---*/
#define _LPV(hObj) *(LPVOID FAR*)&hObj
#define NEWOBJ(hObj) \
  (_LPV(hObj)=FmNew(sizeof(*hObj),&_CD(hObj),szSRCFILE,__LINE__))
#define FREE(hObj) (_LPV(hObj)=FmFree(hObj))

/*--- String interface macros ---*/
#define NEWSTRING(lpDst,wSize) \
  (_LPV(lpDst)=FmNew((SIZET)(wSize),NULL,szSRCFILE,__LINE__))
#define MYLSTRDUP(lpDst,lpSrc) \
  (_LPV(lpDst)=FmStrDup(lpSrc,szSRCFILE,__LINE__))

/*--- Array interface macros ---*/
#define NEWARRAY(lpArray, wSize) \
  (_LPV(lpArray)=FmNew((SIZET)(sizeof(*(lpArray))*(wSize)), \
  NULL,szSRCFILE,__LINE__))
#define SIZEARRAY(lpArray, wSize) \
  (_LPV(lpArray)=FmRealloc((lpArray), \
  (SIZET)(sizeof(*(lpArray))*(wSize)),szSRCFILE,__LINE__))
