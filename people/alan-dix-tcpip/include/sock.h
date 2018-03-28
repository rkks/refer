/****************************************/
/*                                      */
/*      sock.h                          */
/*                                      */
/*      Alan Dix                        */
/*                                      */
/*      package up socket garbage       */
/*                                      */
/*      5th January 1990                */
/*                                      */
/****************************************/

#ifndef SOCK_HEAD
#define SOCK_HEAD

#include "cpp.h"

BEGIN_OLDC

#ifdef ANSI

int  unix_socket(char *name);
int  unix_accept(char *name);
int  unix_connect(char *name);
int  inet_socket(char *hostname,int portno);
int  inet_connect(char *hostname,int portno);
int  sock_accept(int sock);

#else

int  unix_socket();
int  unix_accept();
int  unix_connect();
int  inet_socket();
int  inet_connect();
int  sock_accept();

#endif

END_OLDC

#endif  /* SOCK_HEAD  */
