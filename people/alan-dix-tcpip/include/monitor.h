/****************************************/
/*                                      */
/*      monitor.h                       */
/*                                      */
/*      Alan Dix                        */
/*                                      */
/*      command interpretter to         */
/*      monitor conference server       */
/*                                      */
/*      11th January 1990               */
/*                                      */
/****************************************/

#ifdef ANSI

typedef int (*monitor_fun)(int id, char*command, char* buff);
extern perform_line(char *line);

#else

typedef int (*monitor_fun)();
extern perform_line();

#endif


struct  mon_tab_struct {
    int   id;
    char *name;
    monitor_fun f;
    char *help;
    };


extern  struct mon_tab_struct monitor_tab[];
