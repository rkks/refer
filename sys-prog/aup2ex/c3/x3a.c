/*
	Miscellaneous examples for Chap. 3
	AUP2, Chap. 3

	NOTE: Some of this code is from AUP1 and no longer works.

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
#define __EXTENSIONS__
#include "defs.h"
/*3-3*/
#ifdef WANT_INODE
#include <sys/types.h>
#include <sys/ino.h>

inodes()  /* print i-node info */
{
	struct dinode di;
	int fd, inumber, nread;

	if ((fd = open("/dev/hd0", 0)) == -1)
		syserr("open");
	if (lseek(fd, 1024L + sizeof(di), 0) == -1) /* seek to i-node 2 */
		syserr("lseek");
/*-*/
	for (inumber = 2; inumber <= 6; inumber++) {
		if ((nread = read(fd, &di, sizeof(di))) != sizeof(di))
			switch (nread) {
			case 0:
				fatal("EOF");
			case -1:
				syserr("read");
			default:
				fatal("partial read");
			}
		printf("i-node %d; owner = %3o; size = %7ld\n", inumber, di.di_uid, di.di_size);
	}
}
/**/
#endif WANT_INODE

/*3-4*/
#if 0
#include <sys/dir.h>

directory(path)  /* list directory */
char *path;
{
	struct directx {
		ino_t d_ino;
		char d_name[DIRSIZ + 1];
	} dlink;
	int fd, nread;
	char *dname;
/*-*/

	if ((fd = open(path, 0)) == -1)
		syserr(path);
	dlink.d_name[DIRSIZ] = '\0';
/*-*/
	while ((nread = read(fd, &dlink, sizeof(struct direct))) == sizeof(struct direct)) {
		if (dlink.d_ino == 0)
			dname = "--- UNUSED ---";
		else
			dname = dlink.d_name;
		printf("%-14s %4d\n", dname, dlink.d_ino);
	}
/*-*/
	switch (nread) {
	case 0:
		return;
	case -1:
		syserr("read");
	default:
		fatal("partial read");
	}
}
/**/
#endif

#ifdef WANT_RAWFILE

#include <sys/mman.h>

#define RAWBUFSIZE (4096*16)
#define NUMBLOCKS 10000

#ifdef LINUX
#define FILE_BLOCK	"/dev/hda2"
#define FILE_RAW	"/dev/raw/raw2"
#elif SOLARIS
#define FILE_BLOCK	"/dev/dsk/c0d0p0"
#define FILE_RAW	"/dev/rdsk/c0d0p0"
#else
#error "Not supported"
#endif

void rawfile(void)
{
	void *buf = NULL;
	int fd, i;
	off_t where[NUMBLOCKS];

	for (i = 0; i < NUMBLOCKS; i++) {
		where[i] = (random() % NUMBLOCKS) * RAWBUFSIZE;
//printf("%d\n", where[i]);
}
	printf("RAWBUFSIZE = %d; NUMBLOCKS = %d\n", RAWBUFSIZE, NUMBLOCKS);
	//ec_neg1( mlockall(MCL_CURRENT | MCL_FUTURE) )
#ifdef SOLARIS
	ec_null( buf = memalign(RAWBUFSIZE, RAWBUFSIZE) )
#else
	ec_nzero( errno = posix_memalign(&buf, RAWBUFSIZE, RAWBUFSIZE) )
#endif
	ec_neg1( fd = open(FILE_BLOCK, O_RDONLY) )
	timestart();
	for (i = 0; i < NUMBLOCKS; i++) {
		errno = 0;
		if (pread(fd, buf, RAWBUFSIZE, where[i]) != RAWBUFSIZE)
			EC_FAIL
	}
	timestop("reading " FILE_BLOCK);
	ec_neg1( close(fd) )
	ec_neg1( fd = open(FILE_RAW, O_RDONLY) )
	timestart();
	for (i = 0; i < NUMBLOCKS; i++) {
		errno = 0;
		if (pread(fd, buf, RAWBUFSIZE, where[i]) != RAWBUFSIZE)
			EC_FAIL
	}
	timestop("reading " FILE_RAW);
	ec_neg1( close(fd) )
	ec_neg1( fd = creat("z.tmp", PERM_FILE) )
	for (i = 0; i < NUMBLOCKS; i++) {
		errno = 0;
		if (write(fd, buf, RAWBUFSIZE) != RAWBUFSIZE)
			EC_FAIL
	}
	ec_neg1( close(fd) )
	system("ls -l z.tmp");
	ec_neg1( fd = open("z.tmp", O_RDONLY) )
	timestart();
	for (i = 0; i < NUMBLOCKS; i++) {
		errno = 0;
		if (pread(fd, buf, RAWBUFSIZE, where[i]) != RAWBUFSIZE)
			EC_FAIL
	}
	timestop("reading ordinary file");
	ec_neg1( close(fd) )
	ec_neg1( fd = open("z.tmp", O_RDONLY | O_RSYNC | O_DSYNC) )
	timestart();
	for (i = 0; i < NUMBLOCKS; i++) {
		errno = 0;
		if (pread(fd, buf, RAWBUFSIZE, where[i]) != RAWBUFSIZE)
			EC_FAIL
	}
	timestop("reading ordinary file (O_RSYNC | O_DSYNC)");
	ec_neg1( close(fd) )
	return;

EC_CLEANUP_BGN
	printf("buf addr = 0x%lx\n", (long)buf);
	EC_FLUSH("rawfile");
EC_CLEANUP_END
}

#endif /* WANT_RAWFILE */

#if 0
/*3-5*/
int mkdir(path)  /* make directory; must be superuser */
char *path;
{
	return(mknod(path, S_IFDIR | 0775, 0));
}
/**/

/*3-6*/
int mkdir2(path)  /* make directory */
char *path;
{
	char cmd[50];

	sprintf(cmd, "mkdir %s", path);
	if (system(cmd) == 0)
		return(0);
	else
		return(-1);
}
/**/

/*3-7*/
int mkfifo(path)  /* make FIFO */
char *path;
{
	return(mknod(path, S_IFIFO | 0666, 0));
}
/**/

/*3-8*/
mv(from, to)  /* move file or directory; must be superuser */
char *from, *to;
{
	bool isdir();
	extern int errno;

	if (!isdir(to))
		if(unlink(to) == -1 && errno != ENOENT)
			syserr("unlink1");
	if (link(from, to) == -1)
		syserr("link");
	if (unlink(from) == -1)
		syserr("unlink2");
}

bool isdir(path)  /* is path a directory? */
char *path;
{
	bool ans;
	int fd;
	extern int errno;

	ans = (fd = open(path, 1)) == -1 && errno == EISDIR;
	if (fd != -1 && close(fd) == -1)
		syserr("close");
	return(ans);
}
/**/

/*3-9*/
char *parent(path, dir)  /* get parent directory */
char *path, *dir;
{
	char *lastslash, *strrchr();

	strcpy(dir, path);
	lastslash = strrchr(dir, '/');
	if (lastslash == NULL) /* current directory */
		strcpy(dir, ".");
	else if (lastslash == dir) /* root */
		strcpy(dir, "/");
	else
		*lastslash = '\0';
	return(dir);
}
/**/

mv2(from, to) /* must be superuser */
char *from, *to;
{
	bool isdir();
	extern int errno;
/*3-10*/
	char dir[50], *parent();

	if (access(parent(from, dir), 2) == -1 || access(parent(to, dir), 2) == -1)
		syserr(dir);
/**/
	if (!isdir(to))
		if(unlink(to) == -1 && errno != ENOENT)
			syserr("unlink1");
	if (link(from, to) == -1)
		syserr("link");
	if (unlink(from) == -1)
		syserr("unlink2");
}
#endif
void fcntl_example(void)
{
	int fd = -1, flags;

	ec_neg1( fd = open("tmpfile", O_WRONLY | O_CREAT | O_TRUNC,
	  PERM_FILE) )
	printf("flags at open are 0%o\n", fcntl(fd, F_GETFL));
/*[setappend-right]*/
	ec_neg1( flags = fcntl(fd, F_GETFL) )
	ec_neg1( fcntl(fd, F_SETFL, flags | O_APPEND) )
/*[clearappend-right]*/
	ec_neg1( fcntl(fd, F_SETFL, flags & ~O_APPEND) )
/*[setappend-wrong]*/
	ec_neg1( fcntl(fd, F_SETFL, O_APPEND) ) /* wrong */
/*[O_ACCMODE-right]*/
	ec_neg1( flags = fcntl(fd, F_GETFL) )
	if ((flags & O_ACCMODE) == O_RDONLY)
		/* file is opened read-only */
/*[O_ACCMODE-wrong]*/
	if (flags & O_RDONLY)				/* wrong */
	if ((flags & O_RDONLY) == O_RDONLY)	/* still wrong */
/*[]*/
		;
	printf("flags are now 0%o\n", fcntl(fd, F_GETFL));
	ec_neg1( close(fd) )
	ec_neg1( unlink("tmpfile") )
	return;

EC_CLEANUP_BGN
	EC_FLUSH("setappend")
EC_CLEANUP_END
}
/**/

/*[print_statvfs]*/
#if _XOPEN_SOURCE >= 4
#include <sys/statvfs.h>
#define FCN_NAME statvfs
#define STATVFS 1

#elif defined(BSD_DERIVED)
#include <sys/param.h>
#include <sys/mount.h>
#define FCN_NAME statfs

#else
#error "Need statvfs or nonstandard substitute"
#endif

void print_statvfs(const char *path)
{
	struct FCN_NAME buf;

	if (path == NULL)
		path = ".";
	ec_neg1( FCN_NAME(path, &buf) )
#ifdef STATVFS
	printf("block size = %lu\n", buf.f_bsize);
	printf("fundamental block (fblock) size = %lu\n", buf.f_frsize);
#else
	printf("block size = %lu\n", buf.f_iosize);
	printf("fundamental block size = %lu\n", buf.f_bsize);
#endif
	printf("total number of fblocks = %llu\n",
	  (unsigned long long)buf.f_blocks);
	printf("number of free fblocks = %llu\n",
	  (unsigned long long)buf.f_bfree);
	printf("number of avail. fblocks = %llu\n",
	  (unsigned long long)buf.f_bavail);
	printf("total number of i-numbers = %llu\n",
	  (unsigned long long)buf.f_files);
	printf("number of free i-numbers = %llu\n",
	  (unsigned long long)buf.f_ffree);
#ifdef STATVFS
	printf("number of avail. i-numbers = %llu\n",
	  (unsigned long long)buf.f_favail);
	printf("file-system ID = %lu\n", buf.f_fsid);
	printf("Read-only = %s\n",
	  (buf.f_flag & ST_RDONLY) == ST_RDONLY ? "yes" : "no");
	printf("No setuid/setgid = %s\n",
	  (buf.f_flag & ST_NOSUID) == ST_NOSUID ? "yes" : "no");
	printf("max length of filename = %lu\n", buf.f_namemax);
#else
	printf("Read-only = %s\n",
	  (buf.f_flags & MNT_RDONLY) == MNT_RDONLY ? "yes" : "no");
	printf("No setuid/setgid = %s\n",
	  (buf.f_flags & MNT_NOSUID) == MNT_NOSUID ? "yes" : "no");
#endif
	printf("\nFree space = %.0f%%\n",
	  (double)buf.f_bfree * 100 / buf.f_blocks);
	return;

EC_CLEANUP_BGN
	EC_FLUSH("print_statvfs");
EC_CLEANUP_END
}
/*[]*/
#undef FCN_NAME
#undef STATVFS

static void file_ctime_test(void)
{
	struct stat buf;
	int fd;

	ec_neg1( fd = open("tmp2", O_RDWR | O_CREAT | O_TRUNC, PERM_FILE) )
	ec_neg1( write(fd, &buf, sizeof(buf)) )
	ec_neg1( lseek(fd, 0, SEEK_SET) )
	ec_neg1( fstat(fd, &buf) )
	printf("st_atime = %s", ctime(&buf.st_atime));
	printf("st_mtime = %s", ctime(&buf.st_mtime));
	printf("st_ctime = %s", ctime(&buf.st_ctime));
	sleep(5);
	ec_neg1( write(fd, "x", 1) )
	ec_neg1( fstat(fd, &buf) )
	printf("st_atime = %s", ctime(&buf.st_atime));
	printf("st_mtime = %s", ctime(&buf.st_mtime));
	printf("st_ctime = %s", ctime(&buf.st_ctime));
	ec_neg1( close(fd) )

	return;

EC_CLEANUP_BGN
	EC_FLUSH("file_ctime_test");
EC_CLEANUP_END
}
/*[dir_read_test]*/
static void dump(const unsigned char *buf, ssize_t n)
{
	int i, j;

	for (i = 0; i < n; i += 16) {
		printf("%4d  ", i);
		for (j = i; j < n && j < i + 16; j++)
			printf("  %c", isprint((int)buf[j]) ? buf[j] : ' ');
		printf("\n      ");
		for (j = i; j < n && j < i + 16; j++)
			printf(" %.2x", buf[j]);
		printf("\n\n");
	}
	printf("\n");
}

static void dir_read_test(void)
{
	int fd;
	unsigned char buf[96];
	ssize_t nread;

	ec_neg1( fd = open(".", O_RDONLY) )
	ec_neg1( nread = read(fd, buf, sizeof(buf)) )
	dump(buf, nread);
	return;

EC_CLEANUP_BGN
	EC_FLUSH("dir_read_test");
EC_CLEANUP_END
}
/*[readdir_r_test]*/
#include <dirent.h>

static void readdir_r_test(void)
{
	bool ok = false;
	long name_max;
	DIR *dir = NULL;
	struct dirent *entry = NULL, *result;

	errno = 0;
	/* failure with errno == 0 means value not found */
	ec_neg1( name_max = pathconf(".", _PC_NAME_MAX) )
	ec_null( entry = malloc(offsetof(struct dirent, d_name) +
	  name_max + 1) )
	ec_null( dir = opendir(".") )
	while (true) {
		ec_rv( readdir_r(dir, entry, &result) )
		if (result == NULL)
			break;
		printf("name: %s; i-number: %ld\n", result->d_name,
		  (long)result->d_ino);
	}
	ok = true;
	EC_CLEANUP

EC_CLEANUP_BGN
	if (dir != NULL)
		(void)closedir(dir);
	free(entry);
	if (!ok)
		EC_FLUSH("readdir_r_test");
EC_CLEANUP_END
}
/*[readdir_test]*/
static void readdir_test(void)
{
	DIR *dir = NULL;
	struct dirent *entry;

	ec_null( dir = opendir(".") )
	while (errno = 0, (entry = readdir(dir)) != NULL) {
char buf[200];
		printf("name: %s; i-number: %ld\n", entry->d_name,
		  (long)entry->d_ino);
sprintf(buf, "ls -id %s", entry->d_name);
system(buf);
}
	ec_nzero( errno )
	EC_CLEANUP

EC_CLEANUP_BGN
	if (dir != NULL)
		(void)closedir(dir);
	EC_FLUSH("readdir_test");
EC_CLEANUP_END
}
/*[rmdir_test]*/
void rmdir_test(void)
{
	ec_neg1( mkdir("somedir", PERM_DIRECTORY) )
	ec_neg1( rmdir("somedir") )
	ec_neg1( mkdir("somedir", PERM_DIRECTORY) )
	ec_neg1( close(open("somedir/x", O_WRONLY | O_CREAT, PERM_FILE)) )
	ec_neg1( system("ls -ld somedir; ls -l somedir") )
	if (rmdir("somedir") == -1)
		perror("Expected error");
	ec_neg1( system("rm -rf somedir") )
	ec_neg1( system("ls -ld somedir") )
	return;

EC_CLEANUP_BGN
	EC_FLUSH("rmdir_test");
EC_CLEANUP_END
}
/*[]*/

static void access_test(void)
{
/*[access-example]*/
if (access("tmp", F_OK) == 0)
	printf("Exists\n");
else if (errno == EACCES)
	printf("Does not exist\n");
else
	EC_FAIL
/*[]*/
	return;

EC_CLEANUP_BGN
	EC_FLUSH("access_test");
EC_CLEANUP_END
}

#ifdef _XOPEN_UNIX
void getdate_example(void)
{
	struct tm *tp;
	char s[100];

	printf("Type dates (Ctrl-d to stop)\n");
	while (fgets(s, sizeof(s), stdin)) {
		s[strlen(s) - 1] = '\0';
		printf("Trying \"%s\" ...\n", s);
		if ((tp = getdate(s)) == NULL) {
			printf("getdate_err = %d\n", getdate_err);
			if (getdate_err == 1)
				printf("***** Must set environment variable DATEMSK to "
				  "getdate_template.txt (and EXPORT it).*****\n");
		}
		else
			printf("%s", asctime(tp));
	}
}
#endif

int main(int argc, char *argv[])
{
/*
	char s[50], dir[50];
	int fd;
	inodes();
	directory("/usr/marc/stuff");
{
	ssize_t n;
	char buf[1024];

	ec_neg1( symlink("lunch with Mike at 11:30", "/home/marc/mylink") )
	ec_neg1( n = readlink("/home/marc/mylink", buf, sizeof(buf) - 1) )
	buf[n] = '\0';

	printf("\"%s\"\n", buf);
}
	file_ctime_test();
*/
	readdir_r_test();
/*
	printf("-----\n");
	readdir_test();
	access_test();
	fcntl_example();
	rmdir_test();
	if (argc > 1)
		print_statvfs(argv[1]);
	else
		print_statvfs(NULL);
*/
#ifdef _XOPEN_UNIX
	getdate_example();
#else
	printf("getdate not supported\n");
#endif
/*
	rawfile();
	mkdir2("dirx.tmp");
	while (gets(s) != NULL)
		printf("\"%s\"\n", parent(s, dir));
	mv2(argv[1], argv[2]);
	mkfifo("fifo.tmp");
	if (mkdir("dir.tmp") == -1)
		syserr("mkdir");
	system("li -ld *.tmp; rm -rf *.tmp");
	if ((fd = open("z.tmp", 1)) == -1)
		syserr("open");
	if (setappend(fd) == -1)
		syserr("setappend");
	write(fd, "should be last\n", 15);
*/
	return EXIT_SUCCESS;
/*
EC_CLEANUP_BGN
	return EXIT_FAILURE;
EC_CLEANUP_END
*/
}
