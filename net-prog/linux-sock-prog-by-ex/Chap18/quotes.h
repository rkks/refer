/* quotes.h :
 * 
 * Project header file:
 */
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <ctype.h>
#include <string.h>
#include <getopt.h>
#include <memory.h>
#include <stdarg.h>
#include <math.h>
#include <syslog.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <netinet/in.h>

/*
 * Default Quote Server :
 */
#define DFLT_SERVER "finance.yahoo.com:80"

/*
 * Default Broadcast Address :
 */
#define DFLT_BCAST  "127.255.255.255:9777"

/*
 * *.CSV Parsing Parameter :
 */
typedef struct {
    char    type;       /* 'S' or 'D' */
    void    *parm;      /* Ptr to parameter */
} Parm;

/*
 * Timeout on Quote Fetch :
 */
#define TIMEOUT_SECS    10

/*
 * Ticker load file :
 */
#define TICKPATH        "tickers.rc"

/*
 * Maximum number of tickers :
 */
#define MAX_TICKERS     256

/*
 * Ticker length :
 */
#define TICKLEN         8

/*
 * Date Length :
 */
#define DTLEN           10

/*
 * Time field length :
 */
#define TMLEN           7

/*
 * Define TRUE & FALSE if not defined :
 */
#ifndef TRUE
#define TRUE    1
#define FALSE   0
#endif

/*
 * Ticker Request Structure :
 */
typedef struct {
    char    ticker[TICKLEN+1];   /* Symbol */
    double  last_trade;      /* Last Price */
    char    *date;                 /* Date */
    char    *time;   /* Time of Last Trade */
    double  change;          /* +/- Change */
    double  open_price;   /* Opening Price */
    double  high;            /* High Price */
    double  low;              /* Low Price */
    double  volume;    /* Volume of Trades */
    int     flags;         /* Server flags */
    time_t  next_samp; /* Time of next evt */
} TickReq;

/*
 * Ticker Flags :
 */
        /* Ticker unknown */
#define FLG_UNKNOWN     1
        /* Data format error */
#define FLG_ERROR       2

/*
 * External Function References :
 */
extern int load(
    TickReq *tick,int *pntick,int nmax);
extern int extract_parms(
    Parm *plist,short n,char *src);
extern void msgf(
    char type,const char *format,...);
extern int Connect(const char *addr);
extern int mkaddr(
    void *addr,
    int *addrlen,
    char *str_addr,
    char *protocol);
extern char *Basename(char *cmd);
extern char *strtick(char *str);
extern int get_tickinfo(TickReq *req,char *addr);
extern void broadcast(
    int s,TickReq *quote,struct sockaddr *bc_addr,
    socklen_t bc_len);

/* End */
