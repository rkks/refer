/****************************************/
/*                                      */
/*      cpp.h                           */
/*                                      */
/*      cplusplus compatibility         */
/*                                      */
/*      Alan Dix                        */
/*                                      */
/*      15th May 1991                   */
/*                                      */
/****************************************/

#ifndef CPP_HEAD
#define CPP_HEAD

#define volatile

#ifdef __cplusplus

#ifndef ANSI
#define ANSI
#endif

#define BEGIN_OLDC   extern "C" {
#define END_OLDC     }
#define OLDC_FUN     extern "C"

#else  /* not __cplusplus */

#define BEGIN_OLDC
#define END_OLDC
#define OLDC_FUN

#define form MY_form

#ifdef ANSI
extern char *MY_form(char *fmt);
#else
extern char *MY_form();
#endif

#endif  /* __cplusplus */

#endif  /* CPP_HEAD  */

/**/
