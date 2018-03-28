/****************************************/
/*                                      */
/*      line_by_line.c                  */
/*                                      */
/*      Alan Dix                        */
/*                                      */
/*      line input protocol             */
/*                                      */
/*      8th January 1990                */
/*                                      */
/****************************************/

#ifdef ANSI
typedef void * id_type;
typedef void (*line_fun)( int fd, id_type id, char*buff );
typedef void (*eof_fun)( int fd, id_type id );
int inform_line_by_line( int fd,      line_fun line,
                         eof_fun eof, id_type id    );
int eof_line_by_line(int fd);
int cancel_line_by_line(int fd);
#else
typedef void * id_type;
typedef void (*line_fun)();
typedef void (*eof_fun)();
int inform_line_by_line();
int eof_line_by_line();
int cancel_line_by_line();
#endif
