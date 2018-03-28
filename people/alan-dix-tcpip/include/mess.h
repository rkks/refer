/****************************************/
/*                                      */
/*      mess.h                          */
/*                                      */
/*      Alan Dix                        */
/*                                      */
/*      print to tty and transcript     */
/*                                      */
/*      8th January 1990                */
/*                                      */
/****************************************/

#ifdef ANSI
void  mess( char *fmt, ... );
void  init_mess( char *transcript );
#else
void  mess();
void  init_mess();
#endif
