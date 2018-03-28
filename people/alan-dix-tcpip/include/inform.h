/****************************************/
/*                                      */
/*      inform.h                        */
/*                                      */
/*      Alan Dix                        */
/*                                      */
/*      notifier like I/O multiplexer   */
/*                                      */
/*      5th January 1990                */
/*                                      */
/****************************************/

#ifndef INFORM_HEAD
#define INFORM_HEAD

#include <sys/time.h>
#include "cpp.h"

BEGIN_OLDC

extern int inform_finished;
extern int inform_tick_count;
extern int inform_tick_id;

#ifdef ANSI

typedef void *inform_id;
typedef (*inform_fun)( int fd, inform_id id );
int    inform_itimer_times( long p_s, long p_us, long v_s, long v_us,
                      inform_fun f, inform_id id );
int    inform_itimer( struct itimerval *value,
                      inform_fun f, inform_id id );
int    inform_signal( int signo, inform_fun f, inform_id id );
int    inform_input( int fd, inform_fun f, inform_id id );
int    inform_output( int fd, inform_fun f, inform_id id );
int    inform_exception( int fd, inform_fun f, inform_id id );
int    inform_slack( inform_fun f, inform_id id );
int    inform_done(void);
int    inform_one_loop(long t);
int    inform_loop(void);
  /* inform_tick */
int    do_inform_tick();
int    inform_tick( struct itimerval *value,
                    inform_fun f, inform_id id );
int    inform_tick_times( long p_s, long p_us, long v_s, long v_us,
                             inform_fun f, inform_id id );
int    rm_inform_tick( inform_fun f, inform_id id );
int    rm_inform_tick_no( int tick_id );
int    inform_tick_reset( int tick_id );
int    inform_tick_on( int tick_id );
int    inform_tick_off( int tick_id );

#else

typedef void *inform_id;
typedef (*inform_fun)();
int    inform_itimer_times();
int    inform_itimer();
int    inform_signal();
int    inform_input();
int    inform_output();
int    inform_exception();
int    inform_slack();
int    inform_done();
int    inform_one_loop();
int    inform_loop();

  /* inform_tick */
int    do_inform_tick();
int    inform_tick();
int    inform_tick_times();
int    rm_inform_tick();
int    rm_inform_tick_no();
int    inform_tick_reset();
int    inform_tick_on();
int    inform_tick_off();

#endif

#define FATAL_ERROR        -151
#define TICK_ALARM_MISSED  -150

END_OLDC

#endif  /* INFORM_HEAD  */
