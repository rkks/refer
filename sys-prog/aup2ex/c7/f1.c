/*
	File locking (bad example)
	AUP2, Sec. 7.11.1

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
/*[rec]*/
struct rec {
	int r_data;
	off_t r_next;
};
/*[readrec]*/
bool readrec(int dbfd, struct rec *r, off_t off)
{
	ssize_t nread;

	if ((nread = pread(dbfd, r, sizeof(struct rec), off)) ==
	  sizeof(struct rec))
		return true;
	if (nread != -1)
		errno = EIO;
	EC_FAIL
	return true;

EC_CLEANUP_BGN
	return false;
EC_CLEANUP_END
}

bool writerec(int dbfd, struct rec *r, off_t off)
{
	ssize_t nwrote;

	if ((nwrote = pwrite(dbfd, r, sizeof(struct rec), off)) ==
	  sizeof(struct rec))
		return true;
	if (nwrote != -1)
		errno = EIO;
	EC_FAIL
	return true;

EC_CLEANUP_BGN
	return false;
EC_CLEANUP_END
}
/*[store]*/
bool store(int dbfd, int data)
{
	struct rec r, rnew;
	off_t end, prev;

	ec_neg1( end = lseek(dbfd, 0, SEEK_END) )
	prev = 0;
	ec_false( readrec(dbfd, &r, prev) )
	while (r.r_next != 0) {
		ec_false( readrec(dbfd, &r, r.r_next) )
		if (r.r_data > data)
			break;
		prev = r.r_next;
	}
	ec_false( readrec(dbfd, &r, prev) )
	rnew.r_next = r.r_next;
	r.r_next = end;
	ec_false( writerec(dbfd, &r, prev) )
	rnew.r_data = data;
	usleep(1); /* give up CPU */
	ec_false( writerec(dbfd, &rnew, end) )
	return true;

EC_CLEANUP_BGN
	return false;
EC_CLEANUP_END
}
/*[process1]*/
#define DBNAME "termdb"

static void process1(void)
{
	int dbfd, data;
	struct rec r;

	ec_neg1( dbfd = open(DBNAME, O_CREAT | O_TRUNC | O_RDWR, PERM_FILE) )
	memset(&r, 0, sizeof(r));
	ec_false( writerec(dbfd, &r, 0) )
	for (data = 100; data >= 0; data--)
		ec_false( store(dbfd, data) )
	ec_neg1( close(dbfd) )
	exit(EXIT_SUCCESS);

EC_CLEANUP_BGN
	exit(EXIT_FAILURE);
EC_CLEANUP_END
}
/*[process2]*/
static void process2(void)
{
	int try, dbfd;
	struct rec r1, r2;

	for (try = 0; try < 10; try++)
		if ((dbfd = open(DBNAME, O_RDWR)) == -1) {
			if (errno == ENOENT) {
				continue;
			}
			else
				EC_FAIL
		}
	ec_neg1( dbfd )
	for (try = 0; try < 100; try++) {
		ec_false( readrec(dbfd, &r1, 0) )
		while (r1.r_next != 0) {
			ec_false( readrec(dbfd, &r2, r1.r_next) )
			if (r1.r_data > r2.r_data) {
				printf("Found sorting error (try %d)\n", try);
				break;
			}
			r1 = r2;
		}
	}
	ec_neg1( close(dbfd) )
	return;

EC_CLEANUP_BGN
	exit(EXIT_FAILURE);
EC_CLEANUP_END
}
/*[main]*/
int main(void)
{
	pid_t pid;

	ec_neg1( pid = fork() )
	if (pid == 0)
		process1();
	else {
		process2();
		ec_neg1( waitpid(pid, NULL, 0) )
	}
	exit(EXIT_SUCCESS);

EC_CLEANUP_BGN
	exit(EXIT_FAILURE);
EC_CLEANUP_END
}
/*[]*/
