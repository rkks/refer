/*
	Example using shared memory and semaphore (inefficient)
	AUP2, Sec. 7.13.2

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
#include <sys/shm.h>
#include "SimpleSem.h"

static int *getaddr(void)
{
	key_t key;
	int shmid, *p;

	(void)close(open("shmseg", O_WRONLY | O_CREAT, 0));
	ec_neg1( key = ftok("shmseg", 1) )
	ec_neg1( shmid = shmget(key, sizeof(int), IPC_CREAT | PERM_FILE) )
	ec_neg1( p = shmat(shmid, NULL, 0) )
	return p;

EC_CLEANUP_BGN
	return NULL;
EC_CLEANUP_END
}
/*[pgm]*/
int main(void)
{
	pid_t pid;

	ec_false( SimpleSemRemove("shmexsem") )
	if ((pid = fork()) == 0) {
		struct SimpleSem *sem;
		int *p, prev = 0, n;

		ec_null( sem = SimpleSemOpen("shmexsem") )
		ec_null( p = getaddr() )
		while (true) {
			ec_false( SimpleSemWait(sem) )
			n = *p;
			ec_false( SimpleSemPost(sem) )
			if (n == 99)
				break;
			if (prev != n) {
				printf("child saw %d\n", n);
				prev = n;
			}
		}
		printf("child is done\n");
		ec_false( SimpleSemClose(sem) )
	}
	else {
		struct SimpleSem *sem;
		int *p, i;

		ec_null( sem = SimpleSemOpen("shmexsem") )
		ec_null( p = getaddr() )
		*p = 0;
		ec_false( SimpleSemPost(sem) )
		for (i = 1; i < 4; i++) {
			ec_false( SimpleSemWait(sem) )
			*p = i;
			ec_false( SimpleSemPost(sem) )
			sleep(1);
		}
		ec_false( SimpleSemWait(sem) )
		*p = 99;
		ec_false( SimpleSemPost(sem) )
		ec_false( SimpleSemClose(sem) )
	}
	exit(EXIT_SUCCESS);

EC_CLEANUP_BGN
	exit(EXIT_FAILURE);
EC_CLEANUP_END
}

