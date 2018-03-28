/****************************************/
/*                                      */
/*      inform.c                        */
/*                                      */
/*      Alan Dix                        */
/*                                      */
/*      notifier like I/O multiplexer   */
/*                                      */
/*      5th January 1990                */
/*                                      */
/****************************************/

#include  <stdio.h>
#include  <assert.h>
#include  <sys/types.h>
/* #include  <sys/time.h> -- now in inform.h */
#include  <errno.h>
#include  <signal.h>

#include "fd.h"
#include "cpp.h"
#include "inform.h"

#define BUFFSIZE 512
#define CTRL_Z 26

#define MAX_TICKS 30

struct d_s { inform_fun f; inform_id id; };

static struct d_s input_dispatch[MAX_FD],
                  output_dispatch[MAX_FD],
                  exception_dispatch[MAX_FD];

static struct d_s signal_dispatch[NSIG];

static struct d_s slack;
static int slack_on = 0;

#define is_slack()   ( (int)slack.f && slack_on )

static volatile int  caught_any_interupt = 0;
static volatile int  caught_new_interupt = 0;
static volatile unsigned  int  caught_interupt[NSIG];

#ifdef ANSI
  typedef void (*sigfun)(int signo);
#else
  typedef void (*sigfun)();
#endif

#define OK_SIG(signo) ( 0 <= (signo) && signo < NSIG )

static int    invoke_slack(t)
    int t;
{
    return (slack.f)( t, slack.id );
}

static void   catch_signal(signo)
    int signo;
{
    if ( !OK_SIG(signo) )  return;
    caught_interupt[signo]++;
    caught_new_interupt = 1;
    caught_any_interupt = 1;
    (void)signal(signo,catch_signal);
}

static int    invoke_signals()
{
    int i, code;
    struct d_s  *tab;
    caught_new_interupt = 0;
    for ( i=0; i<NSIG; i++ ) {
        tab = & signal_dispatch[i];
        if ( tab->f == 0 )  {
            caught_interupt[i] = 0;
            continue;
            }
        while ( caught_interupt[i] > 0 ) {
            caught_interupt[i]--;
            code =  (*(tab->f))( i, tab->id );
            if ( code < 0 ) return code;
            }
     /*   assert( caught_interupt[i]==0 );             */
     /*    NO!!  may have been incremented by interupt */
        }
    caught_any_interupt = 0;
    if ( caught_new_interupt ) caught_any_interupt = 1;
}

static int    add_sig_dispatch(signo,f,id)
    int         signo;
    inform_fun  f;
    inform_id   id;
{
    struct d_s  *tab;
    if ( ! OK_SIG(signo) ) return -1;
    tab = & signal_dispatch[signo];
    if ( f == 0 && tab->f != 0 ) {
        (void)signal(signo,SIG_DFL);
        }
    else if ( f != 0 && tab->f == 0 ) {
        (void)signal(signo,catch_signal);
        }
    tab->f  = f;
    tab->id = id;
    return 0;
}

static int    add_dispatch(table,fd,f,id)
    struct d_s *table;
    int         fd;
    inform_fun  f;
    inform_id   id;
{
    if ( ! OK_FD(fd) ) return -1;
    table[fd].f  = f;
    table[fd].id = id;
    return 0;
}

static int    tell_dispatch(table,fd)
    struct d_s *table;
    int         fd;
{
    if ( ! OK_FD(fd) ) return -1;
    if ( table[fd].f == 0 ) return -1;
    return (*(table[fd].f))( fd, table[fd].id );
}

static int    make_fds(table,fds)
    struct d_s *table;
    fd_set     *fds;
{
    int i;
    FD_ZERO(fds);
    for ( i=0; i<MAX_FD; i++ ) {
        if ( table[i].f != 0 ) FD_SET(i,fds);
        }
    return 0;
}

static int    invoke_fds(table,fds)
    struct d_s *table;
    fd_set     *fds;
{
    int i;
    for ( i=0; i<MAX_FD; i++ ) {
        if ( ! FD_ISSET(i,fds) ) continue;
        if ( table[i].f == 0 ) return -1;  /* internal error */
        return (*(table[i].f))( i, table[i].id );
        }
    return 0;
}

int    inform_signal(signo,f,id)
    int         signo;
    inform_fun  f;
    inform_id   id;
{
    return add_sig_dispatch(signo,f,id);
}

int    inform_itimer_times(p_s,p_us,v_s,v_us,f,id)
    long        p_s, p_us,
                v_s, v_us;
    inform_fun  f;
    inform_id   id;
{
    struct itimerval value;
    value.it_interval.tv_sec  = p_s;
    value.it_interval.tv_usec = p_us;
    value.it_value.tv_sec     = v_s;
    value.it_value.tv_usec    = v_us;
    return inform_itimer(&value,f,id);
}

int    inform_itimer(value,f,id)
    struct itimerval *value;
    inform_fun  f;
    inform_id   id;
{
    int res;
    if ( value == NULL || ! timerisset(&(value->it_value))
                       || f== NULL ) {
        static struct itimerval zero_time =  { {0,0}, {0,0} };      
        (void)setitimer(ITIMER_REAL,&zero_time,NULL);
        return add_sig_dispatch(SIGALRM,NULL,NULL);
        }
    res = setitimer(ITIMER_REAL,value,NULL);
    if ( res < 0 ) {
        (void)add_sig_dispatch(SIGALRM,NULL,NULL);
        return res;
        }
    return add_sig_dispatch(SIGALRM,f,id);
}

int    inform_input(fd,f,id)
    int         fd;
    inform_fun  f;
    inform_id   id;
{
    return add_dispatch(input_dispatch,fd,f,id);
}

int    inform_output(fd,f,id)
    int         fd;
    inform_fun  f;
    inform_id   id;
{
    return add_dispatch(output_dispatch,fd,f,id);
}

int    inform_exception(fd,f,id)
    int         fd;
    inform_fun  f;
    inform_id   id;
{
    return add_dispatch(exception_dispatch,fd,f,id);
}

int    inform_slack(f,id)
    inform_fun  f;
    inform_id   id;
{
    slack.f  = f;
    slack.id = id;
    if ( f ) slack_on = 1;
    else     slack_on = 0;
}

int    inform_slack_on()
{
    if ( slack.f )
        slack_on = 1;
}

int    inform_slack_off()
{
    slack_on = 0;
}

int inform_finished = 0;

int   inform_done()
{
    inform_finished = 1;
    return 0;
}

#define MAX_SLACK_CT  30
#define MIN_SLACK_CT   5

static int slack_ct = MIN_SLACK_CT;

#define do_slack      if ( slack_ct < MAX_SLACK_CT ) { slack_ct++; }\
                      else { /* do nout */; }
#define do_no_slack   if ( slack_ct > MIN_SLACK_CT ) { slack_ct--; }\
                      else { /* do nout */; }

int   inform_one_loop(t)
    long       t;
{
    int        num;
    fd_set     readfds;
    fd_set     writefds;
    fd_set     errfds;
    static     struct timeval timeout = {0,0};
    int        i;

    if ( ! is_slack() ) 
        timeout.tv_sec = t;

    if ( caught_any_interupt ) {
        do_no_slack;
        return invoke_signals();
        }
    make_fds(input_dispatch,&readfds);
    make_fds(output_dispatch,&writefds);
    make_fds(exception_dispatch,&errfds);
    num = select(32, &readfds, &writefds, &errfds, &timeout);
    if (num < 0 ) {
        if ( errno != EINTR ) {
	    perror("select");
	    return -1;
	    }
	else {
            if ( caught_any_interupt ) {
                do_no_slack;
                return invoke_signals();
                }
	    perror("EINTR");
	    return 0;
	    }
	}
    if ( num == 0 ) {
        if ( is_slack() ) {
              do_slack;
              return invoke_slack(slack_ct);
              }
        else  return 0;
        }
    do_no_slack;
    if ( ! FD_ISEMPTY(&readfds) )
        return invoke_fds(input_dispatch,&readfds);
    if ( ! FD_ISEMPTY(&writefds) )
        return invoke_fds(output_dispatch,&writefds);
    if ( ! FD_ISEMPTY(&errfds) )
        return invoke_fds(exception_dispatch,&errfds);
    return -1;   /*  no non-zero fds */
}

int   inform_loop()
{
    int        code;

    while ( !inform_finished ) {
        code = inform_one_loop(1L);
        if ( code >= 0 ) continue;
        if ( code == FATAL_ERROR ) {
            fprintf(stderr,"FATAL ERROR in inform_loop %d\n",code);
            return FATAL_ERROR;
            }
        fprintf(stderr,"ERROR in inform_loop %d\n",code);
        }
    return 0;
}
