/*
	Unified Event Manager test program
	AUP2, Sec. 5.18.2

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
#include "defs.h"
#include "uem.h"
#include <sys/msg.h>
#include <sys/sem.h>
#ifdef POSIX_IPC
#include <mqueue.h>
#include <semaphore.h>
#endif

#define SYSTEMV_MSG_NAME	"/tmp/uemtst_systemv_msg"
#define POSIX_MSG_NAME		"/uemtst_posix_msg"
#define POSIX_SEM_NAME		"/uemtst_posix_sem"
#define MSG_SIZE			100
#define TEST_SIGNAL			SIGHUP

static void handler_nothing(int signum)
{
	printf("Got %d in handler\n", signum);
}

static bool setup_signal(void)
{
	pid_t pid;

	if ((pid = fork()) == 0) {
		int i;

		srand(getpid());
		for (i = 0; i < 10; i++) {
			sleep(rand() % 9 + 1);
			ec_neg1( kill(getppid(), TEST_SIGNAL) )
		}
		exit(EXIT_SUCCESS);
	}
	ec_false( uem_register_signal(TEST_SIGNAL, NULL) )
	return true;

EC_CLEANUP_BGN
	if (pid == 0)
		exit(EXIT_FAILURE);
	else
		return false;
EC_CLEANUP_END
}

static bool setup_systemv_sem(void)
{
	int semid;
	key_t key;
	pid_t pid;
	struct sembuf sop;

	ec_neg1( key = ftok(SYSTEMV_MSG_NAME, 2) )
	if ((semid = semget(key, 1, PERM_FILE)) != -1)
		(void)semctl(semid, 0, IPC_RMID, NULL);
	ec_neg1( semid = semget(key, 1, IPC_CREAT | PERM_FILE) )
	if ((pid = fork()) == 0) {
		int i;

		sop.sem_num = 0;
		sop.sem_op = 1;
		sop.sem_flg = 0;
		srand(getpid());
		for (i = 0; i < 10; i++) {
			sleep(rand() % 9 + 1);
			ec_neg1( semop(semid, &sop, 1) )
		}
		exit(EXIT_SUCCESS);
	}
	sop.sem_num = 0;
	sop.sem_op = -1;
	sop.sem_flg = 0;
	ec_false( uem_register_svsem(semid, &sop, 1, NULL) )
	return true;

EC_CLEANUP_BGN
	if (pid == 0)
		exit(EXIT_FAILURE);
	else
		return false;
EC_CLEANUP_END
}

static bool setup_systemv_msg(void)
{
	int mqid;
	key_t key;
	pid_t pid;

	ec_neg1( key = ftok(SYSTEMV_MSG_NAME, 1) )
	if ((mqid = msgget(key, PERM_FILE)) != -1)
		(void)msgctl(mqid, IPC_RMID, NULL);
	ec_neg1( mqid = msgget(key, IPC_CREAT | PERM_FILE) )
	if ((pid = fork()) == 0) {
		int i;
		struct {
			long mtype;
			char mtext[MSG_SIZE - sizeof(long)];
		} msg;

		msg.mtype = 1;
		srand(getpid());
		for (i = 0; i < 10; i++) {
			sleep(rand() % 9 + 1);
			snprintf(msg.mtext, sizeof(msg.mtext), "System V msg #%d", i);
			ec_neg1( msgsnd(mqid, &msg, sizeof(msg.mtext), 0) )
		}
		exit(EXIT_SUCCESS);
	}
	ec_false( uem_register_svmsg(mqid, MSG_SIZE, NULL) )
	return true;

EC_CLEANUP_BGN
	if (pid == 0)
		exit(EXIT_FAILURE);
	else
		return false;
EC_CLEANUP_END
}

static bool setup_posix_sem(void)
{
#ifdef POSIX_IPC
	sem_t *sem;
	pid_t pid;

	if ((sem = sem_open(POSIX_SEM_NAME, O_RDWR | O_CREAT,
	  PERM_FILE, NULL)) == SEM_FAILED) /* note strange error return */
		EC_FAIL
	if ((pid = fork()) == 0) {
		int i;

		srand(getpid());
		for (i = 0; i < 10; i++) {
			sleep(rand() % 9 + 1);
			ec_neg1( sem_post(sem) )
		}
		exit(EXIT_SUCCESS);
	}
	ec_false( uem_register_pxsem(sem, NULL) )
	return true;

EC_CLEANUP_BGN
	if (pid == 0)
		exit(EXIT_FAILURE);
	else
		return false;
EC_CLEANUP_END
#else
	return true;
#endif
}

static bool setup_posix_msg(void)
{
#ifdef POSIX_IPC
	mqd_t mqd;
	pid_t pid;

	ec_neg1( mqd = mq_open(POSIX_MSG_NAME, O_RDWR | O_CREAT,
	  PERM_FILE, NULL) )
	if ((pid = fork()) == 0) {
		int i;
		char msg[MSG_SIZE];

		srand(getpid());
		for (i = 0; i < 10; i++) {
			sleep(rand() % 9 + 1);
			snprintf(msg, sizeof(msg), "POSIX msg #%d", i);
			ec_neg1( mq_send(mqd, msg, MSG_SIZE, 0) )
		}
		exit(EXIT_SUCCESS);
	}
	ec_false( uem_register_pxmsg(mqd, NULL) )
	return true;

EC_CLEANUP_BGN
	if (pid == 0)
		exit(EXIT_FAILURE);
	else
		return false;
EC_CLEANUP_END
#else
	return true;
#endif
}

static bool setup_process(void)
{
	pid_t pid;

	if ((pid = fork()) == 0) {
		sleep(3);
		exit(1234);
	}
	ec_false( uem_register_process(pid, NULL) )
	return true;

EC_CLEANUP_BGN
	if (pid == 0)
		exit(EXIT_FAILURE);
	else
		return false;
EC_CLEANUP_END
}

static bool setup_fdset(void)
{
	pid_t pid;
	int pfd1[2], pfd2[2];
	fd_set fdset;

	ec_neg1( signal(SIGPIPE, SIG_IGN) )
	ec_neg1( pipe(pfd1) )
	ec_neg1( pipe(pfd2) )
	if ((pid = fork()) == 0) {
		int i;

		srand(getpid());
		ec_neg1( close(pfd1[0]) )
		ec_neg1( close(pfd2[1]) )
		for (i = 0; i < 10; i++) {
			int x;

			sleep(rand() % 9 + 1);
			ec_neg1( write(pfd1[1], &i, sizeof(i)) )
			ec_neg1( read(pfd2[0], &x, sizeof(x)) )
		}
		ec_neg1( close(pfd1[1]) )
		ec_neg1( close(pfd2[0]) )
		exit(EXIT_SUCCESS);
	}
	ec_neg1( close(pfd1[1]) )
	ec_neg1( close(pfd2[0]) )
	while (true) {
		setblock(pfd2[1], false);
		if (write(pfd2[1], "abcde", 5) == -1) {
			if (errno == EAGAIN)
				break;
			else
				EC_FAIL
		}
	}
	FD_ZERO(&fdset);
	FD_SET(pfd1[0], &fdset);
	ec_false( uem_register_fdset(pfd1[0] + 1, &fdset, UEM_FD_READ, NULL) )
	FD_ZERO(&fdset);
	FD_SET(pfd2[1], &fdset);
	ec_false( uem_register_fdset(pfd2[1] + 1, &fdset, UEM_FD_WRITE, NULL) )
	return true;

EC_CLEANUP_BGN
	if (pid == 0)
		exit(EXIT_FAILURE);
	else
		return false;
EC_CLEANUP_END
}

int main(void)
{
	struct uem_event *e;

	(void)close(open(SYSTEMV_MSG_NAME, O_WRONLY | O_CREAT, 0));
	ec_false( uem_bgn() )
	ec_false( setup_systemv_msg() )
	ec_false( setup_systemv_sem() )
	ec_false( setup_posix_msg() )
	ec_false( setup_posix_sem() )
	ec_false( setup_signal() )
	ec_false( setup_process() )
	ec_false( setup_fdset() )
	ec_false( uem_register_heartbeat(3 * 1000000, NULL) )
	while (true) {
		ec_null( e = uem_wait() )
		if (e->ue_errno != 0)
			printf("***** EVENT ERROR: (type = %d) -- %s\n",
			  e->ue_reg->ur_type, strerror(e->ue_errno));
		else
			switch (e->ue_reg->ur_type) {
			case UEM_SVMSG:
				{
					struct msg {
						long mtype;
						char mtext[1];
					};
					printf("Got SysV message: %s\n",
					  ((struct msg *)e->ue_buf)->mtext);
				}
				break;
			case UEM_PXMSG:
				printf("Got POSIX message: %s\n", (char *)e->ue_buf);
				break;
			case UEM_SVSEM:
				printf("Got SysV semaphore\n");
				break;
			case UEM_PXSEM:
				printf("Got POSIX semaphore\n");
				break;
			case UEM_FD_READ:
			{
				int n;
				ssize_t nread;

				ec_neg1( nread = read(e->ue_result, &n, sizeof(n)) )
				if (nread == 0) {
					ec_neg1( close(e->ue_result) )
					ec_false( uem_unregister(e) )
				}
				else
					printf("Got fd -- read %d\n", n);
			}
				break;
			case UEM_FD_WRITE:
			{
				if (write(e->ue_result, "a", 1) == -1) {
					if (errno == EPIPE) {
						printf("EPIPE on pipe... unregistering\n");
						ec_neg1( close(e->ue_result) )
						ec_false( uem_unregister(e) )
					}
					else
						EC_FAIL
				}
				else
					printf("Got fd -- write\n");
			}
				break;
			case UEM_FD_ERROR:
				break;
			case UEM_SIG:
				printf("Got signal: %d\n", (int)e->ue_result);
				break;
			case UEM_PROCESS:
				printf("Got process status: %ld\n", (long)e->ue_result);
				break;
			case UEM_HEARTBEAT:
				printf("Got heartbeat\n");
				break;
			case UEM_NONE:
			default:
				printf("Strange event type: %d\n", e->ue_reg->ur_type);
				break;
			}
		uem_free(e);
	}
	ec_false( uem_end() )
	exit(EXIT_SUCCESS);

EC_CLEANUP_BGN
	exit(EXIT_FAILURE);
EC_CLEANUP_END
}
