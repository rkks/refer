/*
	Simple Semaphore test program
	AUP2, Sec. 7.09.2, 7.10.1

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
#include "SimpleSem.h"

/*
	For POSIX portability, name must begin with slash and contain no other slashes. If SystemV semaphores are used, however, this would require write permission in the root directory.
*/
#define SEMNAME "simplesemtest"

int main(void)
{
	struct SimpleSem *sem;
	pid_t pid;
	int i;

	ec_false( SimpleSemRemove(SEMNAME) )
	if ((pid = fork()) == 0) {
		ec_null( sem = SimpleSemOpen(SEMNAME) )
		printf("child has semaphore open\n");
		while (true) {
			ec_false( SimpleSemWait(sem) )
			printf("child returned from wait\n");
		}
		/* won't get here */
		ec_false( SimpleSemClose(sem) )
		exit(EXIT_SUCCESS);
	}
sleep(1);
	ec_null( sem = SimpleSemOpen(SEMNAME) )
	printf("parent has semaphore open\n");
	for (i = 0; i < 10; i++) {
		printf("parent posting (iteration %d)\n", i);
		ec_false( SimpleSemPost(sem) )
		sleep(1);
	}
	ec_false( SimpleSemClose(sem) )
	ec_neg1( kill(pid, SIGINT) )
	exit(EXIT_SUCCESS);

EC_CLEANUP_BGN
	exit(EXIT_FAILURE);
EC_CLEANUP_END
}
