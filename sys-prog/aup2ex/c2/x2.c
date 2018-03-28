/*
	Miscellaneous examples for Chap. 2
	AUP2, Chap. 2

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

#include <sys/uio.h>

/*[synctest]*/
#define SYNCREPS 5000
#define PATHNAME "tmp"

void synctest(void)
{
	int i, fd = -1;
	char buf[4096];

#if !defined(_POSIX_SYNCHRONIZED_IO) || _POSIX_SYNCHRONIZED_IO == -1
	printf("No synchronized I/O -- comparison skipped\n");
#else
	/* Create the file so it can be checked */
	ec_neg1( fd = open("tmp", O_WRONLY | O_CREAT, PERM_FILE) )
	ec_neg1( close(fd) )
	switch (option_sync_io(PATHNAME)) {
	case OPT_YES:
		break;
	case OPT_NO:
		printf("sync unsupported on %s\n", PATHNAME);
		return;
	case OPT_ERROR:
		EC_FAIL
	}

	memset(buf, 1234, sizeof(buf));

	ec_neg1( fd = open(PATHNAME, O_WRONLY | O_TRUNC | O_DSYNC) )
	timestart();
	for (i = 0; i < SYNCREPS; i++)
		ec_neg1( write(fd, buf, sizeof(buf)) )
	ec_neg1( close(fd) )
	timestop("synchronized");

	ec_neg1( fd = open(PATHNAME, O_WRONLY | O_TRUNC) )
	timestart();
	for (i = 0; i < SYNCREPS; i++)
		ec_neg1( write(fd, buf, sizeof(buf)) )
	ec_neg1( close(fd) )
	timestop("unsynchronized");
#endif
	return;

EC_CLEANUP_BGN
	EC_FLUSH("backward");
	(void)close(fd);
EC_CLEANUP_END
}
/*[]*/
void scatter(void)
{
/*[structs]*/
#define VERSION 506
#define STR_MAX 100

struct header {
	int h_version;
	int h_num_items;
} hdr, *hp;
struct data {
	enum {TYPE_STRING, TYPE_FLOAT} d_type;
	union {
		float d_val;
		char d_str[STR_MAX];
	} d_data;
} d1, d2, *dp;
struct iovec v[3];
/*[]*/
	int fd = -1, i;
	ssize_t n;
	void *buf = NULL;

	ec_neg1( fd = open("tmp", O_RDWR | O_CREAT | O_TRUNC, PERM_FILE) )

	/* initialize */
/*[init]*/
hdr.h_version = VERSION;
hdr.h_num_items = 2;
d1.d_type = TYPE_STRING;
strcpy(d1.d_data.d_str, "Some data to write");
d2.d_type = TYPE_FLOAT;
d2.d_data.d_val = 123.456;
/*[]*/

	/* write with writev */
/*[writev]*/
v[0].iov_base = (char *)&hdr; /* iov_base is sometimes char * */
v[0].iov_len = sizeof(hdr);
v[1].iov_base = (char *)&d1;
v[1].iov_len = sizeof(d1);
v[2].iov_base = (char *)&d2;
v[2].iov_len = sizeof(d2);
ec_neg1( n = writev(fd, v, sizeof(v) / sizeof(v[0])) )
/*[]*/

	/* read back in with read */
/*[read]*/
ec_null( buf = malloc(n) )
ec_neg1( lseek(fd, 0, SEEK_SET) )
ec_neg1( read(fd, buf, n) )
hp = buf;
dp = (struct data *)(hp + 1);
printf("Version = %d\n", hp->h_version);
for (i = 0; i < hp->h_num_items; i++) {
	printf("#%d: ", i);
	switch (dp[i].d_type) {
	case TYPE_STRING:
		printf("%s\n", dp[i].d_data.d_str);
		break;
	case TYPE_FLOAT:
		printf("%.3f\n", dp[i].d_data.d_val);
		break;
	default:
		errno = 0;
		EC_FAIL
	}
}
ec_neg1( close(fd) )
free(buf);
/*[]*/
	return;

EC_CLEANUP_BGN
	EC_FLUSH("scatter");
	(void)close(fd);
	free(buf);
EC_CLEANUP_END
}

void print_C_info(void)
{
	printf("__DATE__: \"%s\"\n", __DATE__);
	printf("__TIME__: \"%s\"\n", __TIME__);
#ifdef __STDC__
	printf("__STDC__: \"%d\"\n", __STDC__);
#endif
#ifdef __STDC_VERSION__
	printf("__STDC_VERSION__: \"%ld\"\n", __STDC_VERSION__);
#endif
#ifdef __STDC_HOSTED__
	printf("__STDC_HOSTED__: \"%d\"\n", __STDC_HOSTED__);
#endif
#ifdef __STDC_IEC_559__
	printf("__STDC_IEC_559__: \"%d\"\n", __STDC_IEC_559__);
#endif
#ifdef __STDC_IEC_559_COMPLEX__
	printf("__STDC_IEC_559_COMPLEX__: \"%d\"\n", __STDC_IEC_559_COMPLEX__);
#endif
#ifdef __STDC_ISO_10646__
	printf("__STDC_ISO_10646__: \"%ld\"\n", __STDC_ISO_10646__);
#endif
}

static bool cleanup_previous_run(void)
{
	/*
		deal with previous abnormal termination, and
		then unlink (delete) the file
	*/
	printf("unlink? ");
	if (getchar() == 'y') {
		unlink("/tmp/apptemp");
		return true;
	}
	return false;
}

static bool temp_open(void)
{
/*[o_excl]*/
int fd;

while ((fd = open("/tmp/apptemp", O_RDWR | O_CREAT | O_TRUNC | O_EXCL,
  PERM_FILE)) == -1) {
	if (errno == EEXIST) {
		if (cleanup_previous_run())
			continue;
		errno = EEXIST; /* may have been reset */
	}
	EC_FAIL /* some other error or can't cleanup */
}
/* file is open; go ahead with rest of app */
/*[]*/
	printf("file is open; go ahead with rest of app\n");
	return true;

EC_CLEANUP_BGN
	return false;
EC_CLEANUP_END
}

/*[copy-with-bug]*/
#define BUFSIZE 512

void copy(char *from, char *to)  /* has a bug */
{
	int fromfd = -1, tofd = -1;
	ssize_t nread;
	char buf[BUFSIZE];

	ec_neg1( fromfd = open(from, O_RDONLY) )
	ec_neg1( tofd = open(to, O_WRONLY | O_CREAT | O_TRUNC,
	  S_IRUSR | S_IWUSR) )
	while ((nread = read(fromfd, buf, sizeof(buf))) > 0)
		if (write(tofd, buf, nread) != nread)
			EC_FAIL
	if (nread == -1)
		EC_FAIL
	ec_neg1( close(fromfd) )
	ec_neg1( close(tofd) )
	return;

EC_CLEANUP_BGN
	(void)close(fromfd); /* can't use ec_neg1 here! */
	(void)close(tofd);
EC_CLEANUP_END
}
/*[]*/

/*[copy2]*/
#undef BUFSIZE
#define BUFSIZE 1024

bool copy2(char *from, char *to)
{
	int fromfd = -1, tofd = -1;
	ssize_t nread, nwrite, n;
	char buf[BUFSIZE];

	ec_neg1( fromfd = open(from, O_RDONLY) )
	ec_neg1( tofd = open(to, O_WRONLY | O_CREAT | O_TRUNC,
	  S_IRUSR | S_IWUSR) )
	while ((nread = read(fromfd, buf, sizeof(buf))) > 0) {
		nwrite = 0;
		do {
			ec_neg1( n = write(tofd, &buf[nwrite], nread - nwrite) )
			nwrite += n;
		} while (nwrite < nread);
	}
	if (nread == -1)
		EC_FAIL
	ec_neg1( close(fromfd) )
	ec_neg1( close(tofd) )
	return true;

EC_CLEANUP_BGN
	(void)close(fromfd); /* can't use ec_neg1 here! */
	(void)close(tofd);
	return false;
EC_CLEANUP_END
}
/*[]*/

/*2-3*/
#undef BUFSIZE
#define BUFSIZE 1100

bool copy2a(char *from, char *to)
{
	int fromfd = -1, tofd = -1;
	ssize_t nread, nwrite, n;
	size_t amt;
	char buf[BUFSIZE];

	ec_neg1( fromfd = open(from, O_RDONLY) )
	ec_neg1( tofd = open(to, O_WRONLY | O_CREAT | O_TRUNC,
	  S_IRUSR | S_IWUSR) )
	amt = sizeof(buf);	/* no Shift I/O off of a block boundary. */
	while ((nread = read(fromfd, buf, amt)) > 0) {
		nwrite = 0;
		do {
			ec_neg1( n = write(tofd, &buf[nwrite], nread - nwrite) )
			nwrite += n;
		} while (nwrite < nread);
		amt = sizeof(buf);
	}
	if (nread == -1)
		EC_FAIL
	ec_neg1( close(fromfd) )
	ec_neg1( close(tofd) )
	return true;

EC_CLEANUP_BGN
	(void)close(fromfd); /* can't use ec_neg1 here! */
	(void)close(tofd);
	return false;
EC_CLEANUP_END
}
/**/

#undef BUFSIZE
/*[copy3]*/
#include "bufio.h"

bool copy3(char *from, char *to)
{
	BUFIO *stfrom, *stto;
	int c;

	ec_null( stfrom = Bopen(from, "r") )
	ec_null( stto = Bopen(to, "w") )
	while ((c = Bgetc(stfrom)) != -1)
		ec_false( Bputc(stto, c) )
	if (errno != 0)
		EC_FAIL
	ec_false( Bclose(stfrom) )
	ec_false( Bclose(stto) )
	return true;

EC_CLEANUP_BGN
	(void)Bclose(stfrom);
	(void)Bclose(stto);
	return false;
EC_CLEANUP_END
}
/*[]*/

#include <stdio.h>
/*[copy4]*/
bool copy4(char *from, char *to)
{
	FILE *stfrom, *stto;
	int c;

	ec_null( stfrom = fopen(from, "r") )
	ec_null( stto = fopen(to, "w") )
	while ((c = getc(stfrom)) != EOF)
		ec_eof( putc(c, stto) )
	ec_false( !ferror(stfrom) )
	ec_eof( fclose(stfrom) )
	ec_eof( fclose(stto) )
	return true;

EC_CLEANUP_BGN
	(void)fclose(stfrom);
	(void)fclose(stto);
	return false;
EC_CLEANUP_END
}
/*[]*/

#include <errno.h>
/*[lockpath]*/
#define LOCKDIR "/tmp/"
#define MAXTRIES 10
#define NAPLENGTH 2

static char *lockpath(char *name)
{
	static char path[100];

	if (snprintf(path, sizeof(path), "%s%s", LOCKDIR, name) > sizeof(path))
		return NULL;
	return path;
}

bool lock(char *name)
{
	char *path;
	int fd, tries;

	ec_null( path = lockpath(name) )
	tries = 0;
	while ((fd = open(path, O_WRONLY | O_CREAT | O_EXCL, 0)) == -1 &&
	  errno == EEXIST) {
		if (++tries >= MAXTRIES) {
			errno = EAGAIN;
			EC_FAIL
		}
		sleep(NAPLENGTH);
	}
	if (fd == -1)
		EC_FAIL
	ec_neg1( close(fd) )
	return(true);

EC_CLEANUP_BGN
	return false;
EC_CLEANUP_END
}

bool unlock(char *name)
{
	char *path;

	ec_null( path = lockpath(name) )
	ec_neg1( unlink(path) )
	return true;

EC_CLEANUP_BGN
	return false;
EC_CLEANUP_END
}
/*[]*/
/*[testlock]*/
void testlock(void)
{
	int i;

	for (i = 1; i <= 4; i++) {
		if (lock("accounts")) {
			printf("Process %ld got the lock\n", (long)getpid());
			sleep(rand() % 5 + 1);
			ec_false( unlock("accounts") )
		}
		else {
			if (errno == EAGAIN) {
				printf("Process %ld tired of waiting\n", (long)getpid());
				ec_reinit(); /* forget this error */
			}
			else
				EC_FAIL /* something serious */
		}
		sleep(rand() % 5 + 5);
	}
	return;

EC_CLEANUP_BGN
	EC_FLUSH("testlock")
EC_CLEANUP_END
}
/*[]*/

void junk(void)
{
	int fd, n;
	char *path, acctrec[50];

/*[tempfile]*/
ec_neg1( fd = open("temp", O_RDWR | O_CREAT | O_TRUNC | O_EXCL, 0) )
ec_neg1( unlink("temp") )
/*[]*/
	ec_neg1( write(fd, &fd, sizeof(fd)) )
	ec_neg1( lseek(fd, SEEK_SET, 0) )
	ec_neg1( read(fd, &n, sizeof(n)) )
	assert(fd == n);
	ec_neg1( close(fd) )

/*[tempfile-lock]*/
ec_false( lock("opentemp") )
ec_neg1(  fd = open("temp", O_RDWR | O_CREAT | O_TRUNC | O_EXCL, 0) )
ec_neg1(  unlink("temp") )
ec_false( unlock("opentemp") )
/*[]*/
	ec_neg1( write(fd, &fd, sizeof(fd)) )
	ec_neg1( lseek(fd, SEEK_SET, 0) )
	ec_neg1( read(fd, &n, sizeof(n)) )
	assert(fd == n);
	ec_neg1( close(fd) )


{
/*[tempfile2]*/
char *pathname;

ec_null( pathname = tmpnam(NULL) )
ec_neg1( fd = open(pathname, O_RDWR | O_CREAT | O_TRUNC | O_EXCL, 0) )
ec_neg1( unlink(pathname) )
/*[]*/
	ec_neg1( write(fd, &fd, sizeof(fd)) )
	ec_neg1( lseek(fd, SEEK_SET, 0) )
	ec_neg1( read(fd, &n, sizeof(n)) )
	assert(fd == n);
	ec_neg1( close(fd) )
}
{
/*[tempfile3]*/
char pathname[] = "/tmp/dataXXXXXX";

errno = 0; /* mkstemp may not set it on error */
ec_neg1( fd = mkstemp(pathname) )
ec_neg1( unlink(pathname) )
printf("%s\n", pathname);
/*[]*/
	ec_neg1( write(fd, &fd, sizeof(fd)) )
	ec_neg1( lseek(fd, SEEK_SET, 0) )
	ec_neg1( read(fd, &n, sizeof(n)) )
	assert(fd == n);
	ec_neg1( close(fd) )
}
{
char pathname[] = "/tmp/dataXXXXXX";
/*[tempfile4]*/
int status;
char cmd[100];

ec_neg1( fd = mkstemp(pathname) )
/* code to write text lines to fd not shown */
/*[]*/
/* following writes not in book */
ec_neg1( write(fd, "zzz\n", 4) )
ec_neg1( write(fd, "mmm\n", 4) )
ec_neg1( write(fd, "qqq\n", 4) )
ec_neg1( write(fd, "aaa\n", 4) )
ec_neg1( write(fd, "rrr\n", 4) )
/*[tempfile4a]*/
snprintf(cmd, sizeof(cmd), "sort %s", pathname);
ec_neg1( status = system(cmd) )
/*[]*/
	ec_neg1( close(fd) )
}

{
/*[fileoffset]*/
int fd1, fd2, fd3;

ec_neg1( fd1 = open("myfile", O_WRONLY | O_CREAT | O_TRUNC, PERM_FILE) )
ec_neg1( fd2 = open("myfile", O_RDONLY) )
ec_neg1( fd3 = open("yourfile", O_RDWR | O_CREAT | O_TRUNC, PERM_FILE) )
/*[]*/
}
return;

/*2-8*/
if (unlink(path) == -1)
	syserr("unlink");
/**/
/*2-7*/
if ((fd = creat(path, 0600)) == -1)
	syserr("creat");
if (close(fd) == -1 || (fd = open(path, 2)) == -1)
	syserr("reopen");
/**/

/*2-9*/
if (!lock("accts"))
	syserr("lock");
if (lseek(fd, 0L, 2) == -1) /* seek to end; see Section 2.11 */
	syserr("lseek");
if (write(fd, acctrec, sizeof(acctrec)) == -1)
	syserr("write");
unlock("accts");
/**/

/*2-10*/
if ((fd = open(path, 1)) == -1) {
	if (errno == ENOENT) {
		if ((fd = creat(path, 0666)) == -1)
			syserr("creat");
	}
	else
		syserr("open");
}
/**/
/*2-11*/
if ((fd = open(path, O_WRONLY | O_CREAT, 0666)) == -1)
	syserr("open");
/**/
	return;

EC_CLEANUP_BGN
EC_CLEANUP_END
}

/*[backward0]*/
void backward0(char *path)
{
	char s[256], c;
	int i, fd;
	ssize_t nread;
	off_t where;

	ec_neg1( fd = open(path, O_RDONLY) )
	ec_neg1( where = lseek(fd, -1, SEEK_END) )
	i = sizeof(s) - 1;
	s[i] = '\0';
	while ((nread = read(fd, &c, 1)) == 1) {
		if (c == '\n') {
			printf("%s", &s[i]);
			i = sizeof(s) - 1;
		}
		if (i <= 0) {
			errno = E2BIG;
			EC_FAIL
		}
		s[--i] = c;
		if (where == 0)
			break;
		ec_neg1( where = lseek(fd, -2, SEEK_CUR) )
	}
	if (nread == -1)
		EC_FAIL
	else if (nread != 1){ /* impossible */
		errno = 0;
		EC_FAIL
	}
	printf("%s", &s[i]);
	ec_neg1( close(fd) )
	return;

EC_CLEANUP_BGN
	EC_FLUSH("backward");
EC_CLEANUP_END
}
/*[]*/
/*[backward]*/
void backward(char *path)
{
	char s[256], c;
	int i, fd;
	off_t where;

	ec_neg1( fd = open(path, O_RDONLY) )
	ec_neg1( where = lseek(fd, 1, SEEK_END) )
	i = sizeof(s) - 1;
	s[i] = '\0';
	do {
		ec_neg1( where = lseek(fd, -2, SEEK_CUR) )
		switch (read(fd, &c, 1)) {
		case 1:
			if (c == '\n') {
				printf("%s", &s[i]);
				i = sizeof(s) - 1;
			}
			if (i <= 0) {
				errno = E2BIG;
				EC_FAIL
			}
			s[--i] = c;
			break;
		case -1:
			EC_FAIL
			break;
		default: /* impossible */
			errno = 0;
			EC_FAIL
		}
	} while (where > 0);
	printf("%s", &s[i]);
	ec_neg1( close(fd) )
	return;

EC_CLEANUP_BGN
	EC_FLUSH("backward");
EC_CLEANUP_END
}
/*[]*/

/*[backward2]*/
void backward2(char *path)
{
	char s[256], c;
	int i, fd;
	off_t file_size, where;

	ec_neg1( fd = open(path, O_RDONLY) )
	ec_neg1( file_size = lseek(fd, 0, SEEK_END) )
	i = sizeof(s) - 1;
	s[i] = '\0';
	for (where = file_size - 1; where >= 0; where--)
		switch (pread(fd, &c, 1, where)) {
		case 1:
			if (c == '\n') {
				printf("%s", &s[i]);
				i = sizeof(s) - 1;
			}
			if (i <= 0) {
				errno = E2BIG;
				EC_FAIL
			}
			s[--i] = c;
			break;
		case -1:
			EC_FAIL
			break;
		default: /* impossible */
			errno = 0;
			EC_FAIL
		}
	printf("%s", &s[i]);
	ec_neg1( close(fd) )
	return;

EC_CLEANUP_BGN
	EC_FLUSH("backward2");
EC_CLEANUP_END
}
/*[]*/

/*[ftruncate_test]*/
void ftruncate_test(void)
{
	int fd;
	const char s[] = "Those are my principles.\n"
	  "If you don't like them I have others.\n"
	  "\t--Groucho Marx\n";

	ec_neg1( fd = open("tmp", O_WRONLY | O_CREAT | O_TRUNC, PERM_FILE) )
	errno = 0;
	ec_false( write(fd, s, sizeof(s)) == sizeof(s) )
	(void)system("ls -l tmp; cat tmp");
	ec_neg1( ftruncate(fd, 25) )
	(void)system("ls -l tmp; cat tmp");
	ec_neg1( close(fd) )
	return;

EC_CLEANUP_BGN
	EC_FLUSH("ftruncate_test");
EC_CLEANUP_END
}
/*[]*/

void pwrite_test(void)
{
	int fd;

	ec_neg1( fd = open("tmp", O_WRONLY | O_CREAT | O_TRUNC | O_APPEND,
	  PERM_FILE) )
	ec_neg1( pwrite(fd, "hello1\n", 7, 0) )
	ec_neg1( pwrite(fd, "hello2\n", 7, 0) )
	ec_neg1( pwrite(fd, "hello3\n", 7, 0) )
	ec_neg1( pwrite(fd, "hello4\n", 7, 0) )
	system("echo '========'; cat tmp");
	ec_neg1( lseek(fd, 0, SEEK_SET) )
	ec_neg1( write(fd, "hello5\n", 7) )
	ec_neg1( write(fd, "hello6\n", 7) )
	ec_neg1( write(fd, "hello7\n", 7) )
	ec_neg1( write(fd, "hello8\n", 7) )
	ec_neg1( close(fd) )
	system("echo '========'; cat tmp; echo '========'");
	return;

EC_CLEANUP_BGN
	EC_FLUSH("pwrite_test");
EC_CLEANUP_END
}

void backward_test(void)
{
	FILE *f;

	ec_null( f = fopen("backward.tmp", "w") )
	ec_eof(  fputs("dog\nbites\nman\n", f) )
	ec_eof(  fclose(f) )
	backward0("backward.tmp");
	backward("backward.tmp");
	backward2("backward.tmp");
	(void)unlink("backward.tmp");
	return;
EC_CLEANUP_BGN
	EC_FLUSH("backward");
EC_CLEANUP_END
}

int main(void)
{
	struct stat sbuf;

	ftruncate_test();
	printf("_SC_OPEN_MAX = %ld\n", sysconf(_SC_OPEN_MAX));
	pwrite_test();
	//synctest();
	scatter();
	backward_test();
	print_C_info();
printf("BUFSIZ = %d\n", BUFSIZ);
printf("getblksize = %lu\n", getblksize(NULL));
	ec_neg1( stat(".", &sbuf) )
printf("st_blksize = %lu\n", (unsigned long)sbuf.st_blksize);
#if 0
	copy("a.tmp", "b.tmp");
	copy2("b.tmp", "c.tmp");
	system("cmp a.tmp c.tmp");
	junk();
	testlock();
	ec_false( temp_open() )
#endif
	unlink("b.tmp");
	unlink("c.tmp");
	unlink("d.tmp");
	unlink("e.tmp");
	system("ls -l file_of_4mb"); /* AUP2 (was "li", whatever that was) */
	timestart();
	ec_false( copy3("file_of_4mb", "d.tmp") )
	timestop("BUFIO pkg");
	system("cmp file_of_4mb d.tmp");
	timestart();
	ec_false( copy4("file_of_4mb", "e.tmp") )
	timestop("Standard I/O");
	system("cmp file_of_4mb e.tmp");
	timestart();
	ec_false( copy2("file_of_4mb", "b.tmp") )
	timestop("BUFSIZ blocks");
	system("cmp file_of_4mb b.tmp");
	timestart();
	ec_false( copy2a("file_of_4mb", "c.tmp") )
	timestop("1-byte blocks");
	system("cmp file_of_4mb c.tmp");
return 0;
	system("ls -l a.tmp");
	if (!lock("lock"))
		syserr("lock1 failed");
	system("ls -l /tmp/lock");
	if (lock("lock"))
		syserr("lock2 failed");
	system("ls -l /tmp/lock");
	unlock("lock");
	system("ls -l /tmp/lock");
	if (!lock("lock"))
		syserr("lock3 failed");
	if (lock("lock"))
		syserr("lock4 failed");
	unlock("lock");
	printf("Done.\n"); /* AUP2 */

EC_CLEANUP_BGN
	return 1;
EC_CLEANUP_END
}
