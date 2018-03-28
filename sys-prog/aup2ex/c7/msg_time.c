/*
	IPC timing comparisons
	AUP2, Sec. 7.15

	Copyright 2003 by Marc J. Rochkind. All rights reserved.
	May be copied only for purposes and under conditions described
	on the Web page www.basepath.com/aup/copyright.htm.

	The Example Files are provided "as is," without any warranty;
	without even the implied warranty of merchantability or fitness
	for a particular purpose. The author and his publisher are not
	responsible for any damages, direct or incidental, resulting
	from the use or non-use of these Example Files.

	The Example Files may contain defects, and some contain deliberate
	coding mistakes that were included for educational reasons.
	You are responsible for determining if and how the Example Files
	are to be used.

*/
/*[top]*/
#include "defs.h"
#include "smi_fifo.h"
#include "smi_msg.h"
#include "smi_skt.h"
#include "smi_shm.h"
#include "smi_pshm.h"
#include "smi_mq.h"
#include "../c6/pcsync_sig.h"
#include <sys/wait.h>
#ifdef FREEBSD
#include <netinet/in.h> /* required for arpa/inet.h */
#endif
#include <arpa/inet.h>

#define NUM_MSGS 500
#define NUM_CLIENTS 4
#define SERVERNAME_LOCAL "MsgTime"
#define SERVERNAME_INET "//localhost:30001"

#define MSG_LEN SMI_MSG_MAX
static int num_msgs = NUM_MSGS;
static int len_msg = 100;
static bool want_check = true;
static bool pcsync = true;
static bool quiet = false;

static bool run_server = true, run_clients = true;
static const char *hostarg = NULL;

typedef enum {MSGCHK_SET, MSGCHK_CALC, MSGCHK_CHK} MSGCHK_OP;

static bool msg_check(void *mbuf, MSGCHK_OP op)
{
	struct smi_msg *m = (struct smi_msg *)mbuf;
	int n = len_msg / sizeof(long);
	long *a = (long *)m->smi_data, sum;
	int i;
	pid_t pid = getpid();

	if (!want_check)
		return true;
	switch (op) {
	case MSGCHK_SET:
		for (i = 1; i < n; i++)
			a[i] = htonl(pid + i);
		break;
	case MSGCHK_CALC:
	case MSGCHK_CHK:
		sum = 0;
		for (i = 1; i < n; i++)
			sum += ntohl(a[i]);
		if (op == MSGCHK_CALC)
			a[0] = htonl(sum);
		else if (ntohl(a[0]) != sum) {
			errno = 0;
			fprintf(stderr, "msg_time: message failed check\n");
			EC_FAIL
		}
	}
	return true;

EC_CLEANUP_BGN
	return false;
EC_CLEANUP_END
}

/*
	Problem: Server can't just exit when finished, as closing its queue kills semaphores (etc.) that may be needed.
*/
/*
	We end up with an opened server in each child, which isn't right, but should not affect timing. Delaying opening the server until the child processes are created would involve making the children wait for the server to start, which involves extra mechanisms. The smi routines typically don't provide for this.
*/

#define time_traffic(method)\
static bool method##_time(const char *servername)\
{\
	int i, k;\
	bool bchild = false;\
	pid_t pid_client[NUM_CLIENTS], pid;\
	SMIQ *sqp;\
	struct pcsync pcs;\
\
	printf("\n" #method "(%s):\n", servername);\
	timestart();\
	if (pcsync)\
		ec_false( pcsync_init(&pcs) )\
	if (run_clients)\
		for (k = 0; k < NUM_CLIENTS; k++)\
			switch (pid_client[k] = fork()) {\
			case -1:\
				syserr("fork 2");\
			case 0:\
				if (pcsync)\
					ec_false( pcsync_wait_for_parent(&pcs) )\
				if (pcsync)\
					(void)pcsync_end(&pcs);\
				if (!quiet)\
					printf("\tProcess %d running freely\n",\
					  (int)getpid());\
				bchild = true;\
				ec_null(  sqp = smi_open_##method(servername, SMI_CLIENT,\
				  len_msg) )\
				for (i = 0; i < num_msgs; i++) {\
					struct smi_msg *msg;\
					ec_false(  smi_send_getaddr_##method(sqp, NULL,\
					  (void **)&msg) )\
					ec_false( msg_check(msg, MSGCHK_SET) )\
					ec_false(  smi_send_release_##method(sqp) )\
					ec_false(  smi_receive_getaddr_##method(sqp,\
					  (void **)&msg) )\
					ec_false( msg_check(msg, MSGCHK_CHK) )\
					ec_false(  smi_receive_release_##method(sqp) )\
				}\
				if (!quiet)\
					printf("\t" #method\
					  " client (process %d) exiting normally\n",\
					  (int)getpid());\
				ec_false( smi_close_##method(sqp) )\
				exit(EXIT_SUCCESS);\
			}\
	if (pcsync && run_clients && strcmp(#method, "fifo") == 0)\
		ec_false( pcsync_unblock_children(&pcs, NUM_CLIENTS, pid_client) )\
	if (run_server)\
		ec_null(  sqp = smi_open_##method(servername, SMI_SERVER,\
		  len_msg) )\
	if (pcsync && run_clients && strcmp(#method, "fifo") != 0)\
		ec_false( pcsync_unblock_children(&pcs, NUM_CLIENTS, pid_client) )\
	if (pcsync)\
		(void)pcsync_end(&pcs);\
	if (run_server)\
		for (i = 0; i < NUM_CLIENTS * num_msgs; i++) {\
			struct smi_msg *msg;\
\
			(void)alarm(200);\
			ec_false( smi_receive_getaddr_##method(sqp, (void **)&msg) )\
			ec_false( msg_check(msg, MSGCHK_CALC) )\
			ec_false( smi_send_getaddr_##method(sqp, &msg->smi_client,\
			  (void **)&msg) )\
			ec_false( smi_receive_release_##method(sqp) )\
			ec_false( smi_send_release_##method(sqp) )\
		}\
	if (!quiet)\
		printf("\tServer waiting for client termination...\n");\
	for (k = 0; k < NUM_CLIENTS; k++) {\
		ec_neg1( waitpid(pid_client[k], NULL, 0) )\
		if (!quiet)\
			printf("\tProcess %d terminated.\n", (int)pid);\
	}\
	if (!quiet)\
		printf("\t" #method " server (process %d) exiting normally\n",\
		  (int)getpid());\
	if (run_server)\
		ec_false( smi_close_##method(sqp) )\
	timestop(#method);\
	return true;\
\
EC_CLEANUP_BGN\
	if (bchild)\
		exit(EXIT_FAILURE);\
	EC_FLUSH(#method " failed")\
	return false;\
EC_CLEANUP_END\
}

time_traffic(fifo)
time_traffic(skt)
time_traffic(msg)
time_traffic(mq)
time_traffic(shm)
#if !defined(FREEBSD) && !defined(LINUX)
time_traffic(pshm)
#endif

static void handler(int signum)
{
	if (signum == SIGALRM)
		(void)write(STDOUT_FILENO, "Alarm Clock -- ignore times\n", 29);
}

int main(int argc, char *argv[])
{
	int c;
	struct sigaction act;

	memset(&act, 0, sizeof(act));
	act.sa_handler = handler;
	ec_neg1( sigaction(SIGALRM, &act, NULL) )
	setbuf(stdout, NULL);
	while ((c = getopt(argc, argv, ":ch:stl:xn:qp")) != -1)
		switch(c) {
		case 'c':
			run_clients = false;
			break;
		case 'h':
			hostarg = optarg;
			break;
		case 'l':
			len_msg = atoi(optarg);
			break;
		case 'n':
			num_msgs = atoi(optarg);
			break;
		case 's':
			run_server = false;
			break;
		case 't':
			num_msgs = 100;
			break;
		case 'q':
			quiet = true;
			break;
		case 'x':
			want_check = false;
			break;
		case 'p':
			pcsync = false;
			break;
		case ':':
			fprintf(stderr, "Option -h requires argument.\n");
			/* fall through */
		default:
			fprintf(stderr, "Usage: mt [-c] [-s] [-t] [-h host]"
			  " [-n nummsgs] [-l msglen] [-q] [-p]\n");
			exit(EXIT_FAILURE);
		}

#if 1
	system("ipcrem >/dev/null 2>/dev/null");
#else
	printf("ipcrem not executed\n");
#endif
	smi_client_nowait = true;
	printf("%d messages of %d bytes; %d clients\n", num_msgs,
	  len_msg, NUM_CLIENTS);
	if (hostarg != NULL)
		skt_time(hostarg);
	else {
		fifo_time(SERVERNAME_LOCAL);
		skt_time(SERVERNAME_LOCAL);
		skt_time(SERVERNAME_INET);
		shm_time(SERVERNAME_LOCAL);
		msg_time(SERVERNAME_LOCAL);
		mq_time(SERVERNAME_LOCAL);
#if !defined(FREEBSD) && !defined(LINUX)
		pshm_time(SERVERNAME_LOCAL);
#endif
	}
	printf("Done\n");
	exit(EXIT_SUCCESS);

EC_CLEANUP_BGN
	exit(EXIT_FAILURE);
EC_CLEANUP_END
}
