/*
	ls command
	AUP2, Sec. 3.5, 3.6.5

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
#define WANT_LSDIR_FIXEDx
#define AUPLS_FULL

#include "defs.h"
#include <dirent.h>
#include <pwd.h>
#include <grp.h>
/*[aupls-top1]*/
#define USAGE "Usage: aupls [-Rd] [dir]\n"

static long total_entries = 0, total_dirs = 0;
/*[aupls-top2]*/
typedef enum {SHOW_PATH, SHOW_INFO} SHOW_OP;

struct traverse_info {
	bool ti_recursive;								 /* -R option? */
	char *ti_name;									 /* current entry */
	struct stat ti_stat;							 /* stat for ti_name */
	bool (*ti_fcn)(struct traverse_info *, SHOW_OP); /* callback fcn */
};
/*[print_mode]*/
#define TYPE(b) ((statp->st_mode & (S_IFMT)) == (b))
#define MODE(b) ((statp->st_mode & (b)) == (b))

static void print_mode(const struct stat *statp)
{
	if (TYPE(S_IFBLK))
		putchar('b');
	else if (TYPE(S_IFCHR))
		putchar('c');
	else if (TYPE(S_IFDIR))
		putchar('d');
	else if (TYPE(S_IFIFO)) /* sic */
		putchar('p');
	else if (TYPE(S_IFREG))
		putchar('-');
	else if (TYPE(S_IFLNK))
		putchar('l');
	else if (TYPE(S_IFSOCK))
		putchar('s');
	else
		putchar('?');
	putchar(MODE(S_IRUSR) ? 'r' : '-');
	putchar(MODE(S_IWUSR) ? 'w' : '-');
	if (MODE(S_ISUID)) {
		if (MODE(S_IXUSR))
			putchar('s');
		else
			putchar('S');
	}
	else if (MODE(S_IXUSR))
		putchar('x');
	else
		putchar('-');
	putchar(MODE(S_IRGRP) ? 'r' : '-');
	putchar(MODE(S_IWGRP) ? 'w' : '-');
	if (MODE(S_ISGID)) {
		if (MODE(S_IXGRP))
			putchar('s');
		else
			putchar('S');
	}
	else if (MODE(S_IXGRP))
		putchar('x');
	else
		putchar('-');
	putchar(MODE(S_IROTH) ? 'r' : '-');
	putchar(MODE(S_IWOTH) ? 'w' : '-');
	if (MODE(S_IFDIR) && MODE(S_ISVTX)) {
		if (MODE(S_IXOTH))
			putchar('t');
		else
			putchar('T');
	}
	else if (MODE(S_IXOTH))
		putchar('x');
	else
		putchar('-');
}
/*[]*/

#undef TYPE
#undef MODE

/*[print_numlinks]*/
static void print_numlinks(const struct stat *statp)
{
	printf("%5ld", (long)statp->st_nlink);
}
/*[print_owner]*/
static void print_owner(const struct stat *statp)
{
	struct passwd *pwd = getpwuid(statp->st_uid);

	if (pwd == NULL)
		printf(" %-8ld", (long)statp->st_uid);
	else
		printf(" %-8s", pwd->pw_name);
}
/*[print_group]*/
static void print_group(const struct stat *statp)
{
	struct group *grp = getgrgid(statp->st_gid);

	if (grp == NULL)
		printf(" %-8ld", (long)statp->st_gid);
	else
		printf(" %-8s", grp->gr_name);
}
/*[print_size]*/
static void print_size(const struct stat *statp)
{
	switch (statp->st_mode & S_IFMT) {
	case S_IFCHR:
	case S_IFBLK:
		printf("%4u,%4u", (unsigned)(statp->st_rdev >> 8),
		  (unsigned)(statp->st_rdev & 0xFF));
		break;
	default:
		printf("%9lu", (unsigned long)statp->st_size);
	}
}
/*[print_date]*/
static void print_date(const struct stat *statp)
{
	time_t now;
	double diff;
	char buf[100], *fmt;

	if (time(&now) == -1) {
		printf(" ????????????");
		return;
	}
	diff = difftime(now, statp->st_mtime);
	if (diff < 0 || diff > 60 * 60 * 24 * 182.5) /* roughly 6 months */
		fmt = "%b %e  %Y";
	else
		fmt = "%b %e %H:%M";
	strftime(buf, sizeof(buf), fmt, localtime(&statp->st_mtime));
	printf(" %s", buf);
}
/*
	In code below, the "not supposed to happen" test modified to include EINVAL, which Darwin
	returned for paths like /dev and /dev/fd.
*/
/*[get_max_pathname]*/
static long get_max_pathname(const char *path)
{
	long max_path;

	errno = 0;
	max_path = pathconf(path, _PC_PATH_MAX);
	if (max_path == -1) {
		if (errno == 0 || errno == EINVAL) /* not supposed to happen */
			max_path = 4096;
		else
			EC_FAIL
	}
	return max_path + 1;

EC_CLEANUP_BGN
	return -1;
EC_CLEANUP_END
}
/*[]*/
/*[print_name]*/
static void print_name(const struct stat *statp, const char *name)
{
	if (S_ISLNK(statp->st_mode)) {
		char *contents = malloc(statp->st_size + 1);
		ssize_t n;

		if (contents != NULL && (n = readlink(name, contents,
		  statp->st_size)) != -1) {
			contents[n] = '\0'; /* can't assume NUL-terminated */
			printf(" %s -> %s", name, contents);
		}
		else
			printf(" %s -> [can't read link]", name);
		free(contents);
	}
	else
		printf(" %s", name);
}
/*[ls_long-a]*/
static void ls_long(const struct stat *statp, const char *name)
{
	print_mode(statp);
	print_numlinks(statp);
	print_owner(statp);
	print_group(statp);
	print_size(statp);
	print_date(statp);
	print_name(statp, name);
	putchar('\n');
}
/*[]*/
#ifdef WANT_LSLONG
/*[ls_long-b]*/
int main(int argc, char *argv[])
{
	int i;
	struct stat statbuf;

	for (i = 1; i < argc; i++) {
		ec_neg1( lstat(argv[i], &statbuf) )
		ls_long(&statbuf, argv[i]);
	}
	exit(EXIT_SUCCESS);

EC_CLEANUP_BGN
	exit(EXIT_FAILURE);
EC_CLEANUP_END
}
/*[]*/
#endif
/*[get_cwd]*/
static char *get_cwd(bool cleanup)
{
	static char *cwd = NULL;
	static long max_path;

	if (cleanup) {
		free(cwd);
		cwd = NULL;
	}
	else {
		if (cwd == NULL) {
			ec_neg1( max_path = get_max_pathname(".") )
			ec_null( cwd = malloc((size_t)max_path) )
		}
		ec_null( getcwd(cwd, max_path) )
		return cwd;
	}
	return NULL;

EC_CLEANUP_BGN
	return NULL;
EC_CLEANUP_END
}
/*[aupls-print_cwd]*/
static bool print_cwd(bool cleanup)
{
	char *cwd;

	if (cleanup)
		(void)get_cwd(true);
	else {
		ec_null( cwd = get_cwd(false) )
		printf("\n%s:\n", cwd);
	}
	return true;

EC_CLEANUP_BGN
	return false;
EC_CLEANUP_END
}
/*[aupls-show_stat]*/
static bool show_stat(struct traverse_info *p, SHOW_OP op)
{
	switch (op) {
	case SHOW_PATH:
		ec_false( print_cwd(false) )
		break;
	case SHOW_INFO:
		ls_long(&p->ti_stat, p->ti_name);
	}
	return true;

EC_CLEANUP_BGN
	return false;
EC_CLEANUP_END
}
/*[aupls-check_parent]*/
static bool check_parent(int dirfd, const char *name)
{
	struct stat statbuf1, statbuf2;

	if (name[0] != '/') {
		ec_neg1( fstat(dirfd, &statbuf1) )
		ec_neg1( lstat("..", &statbuf2) )
		if (statbuf1.st_dev != statbuf2.st_dev ||
		  statbuf1.st_ino != statbuf2.st_ino) {
			print_cwd(false);
			fprintf(stderr, "Doubly-linked directory encountered (%s).\n",
			  name);
			errno = 0;
			EC_FAIL
		}
	}
	return true;

EC_CLEANUP_BGN
	return false;
EC_CLEANUP_END
}
/*[]*/

static bool do_entry(struct traverse_info *p, bool stat_only);

/*[aupls-do_dir1]*/
static bool do_dir(struct traverse_info *p)
{
	DIR *sp = NULL;
	struct dirent *dp;
	int dirfd = -1;
	bool result = false;

	/*
		Entry could be changed from directory to symlink between
		lstat() and here, in which case opendir would follow it,
		possibly resulting in double-visiting or even a loop.
	*/
	ec_neg1(  dirfd = open(".", O_RDONLY) )
	/*
		[Not in book.]
		On Darwin, fails on directory /dev/fd. Problem is that it thinks that some of
		the file descriptors there are directories, and opendir fails on them with a bus
		error. Skipping the entire /dev directory is a good idea. The shortcut is to skip
		directories whose name is "dev", which isn't really correct, because they may not
		be at the root. Fix is left to the reader...
	*/
	if (strcmp(p->ti_name, "dev") == 0 || strncmp(p->ti_name, "/dev", 4) == 0) {
		printf("Skipping directory \"dev\" (even if it's not /dev).\n");
		result = true;
		EC_CLEANUP
	}
	/* [End of stuff not in book.] */
	if ((sp = opendir(p->ti_name)) == NULL || chdir(p->ti_name) == -1) {
		if (errno == EACCES) {
			fprintf(stderr, "%s: Permission denied.\n", p->ti_name);
			result = true;
			EC_CLEANUP
		}
		/* Book showed just EC_FAIL here. Change so we can keep going. */
		else {
			syserr_print("do_dir()");
			result = true; /* repeat of above, but want to keep book's logic */
			EC_CLEANUP
		}
		/* EC_FAIL -- don't want this line anymore */
	}
/*[]*/
	/*
		Following is a good idea, but left out of book for space reasons.
	*/
	if (strcmp(p->ti_name, ".") != 0)
		ec_false( check_parent(dirfd, p->ti_name) )
/*[aupls-do_dir2]*/
	if (p->ti_recursive)
		ec_false( (p->ti_fcn)(p, SHOW_PATH) )
	while (errno = 0, ((dp = readdir(sp)) != NULL)) {
		if (strcmp(dp->d_name, ".") == 0 ||
		  strcmp(dp->d_name, "..") == 0)
			continue;
		p->ti_name = dp->d_name;
		ec_false( do_entry(p, true) )
	}
	if (errno != 0)
		syserr_print("Reading directory (Pass 1)");
	if (p->ti_recursive) {
		rewinddir(sp);
		while (errno = 0, ((dp = readdir(sp)) != NULL)) {
			if (strcmp(dp->d_name, ".") == 0 ||
			  strcmp(dp->d_name, "..") == 0)
				continue;
			p->ti_name = dp->d_name;
			ec_false( do_entry(p, false) )
		}
		if (errno != 0)
			syserr_print("Reading directory (Pass 2)");
	}
	result = true;
	EC_CLEANUP

EC_CLEANUP_BGN
	if (dirfd != -1) {
		(void)fchdir(dirfd);
		(void)close(dirfd);
	}
	if (sp != NULL)
		(void)closedir(sp);
	return result;
EC_CLEANUP_END
}
/*[aupls-do_entry]*/
static bool do_entry(struct traverse_info *p, bool stat_only)
{
	bool is_dir;

	ec_neg1( lstat(p->ti_name, &p->ti_stat) )
	is_dir = S_ISDIR(p->ti_stat.st_mode);
	if (stat_only/* || !is_dir*/) { /* Bug fix: 24-March-2004 */
		total_entries++;
		if (is_dir)
			total_dirs++;
		ec_false( (p->ti_fcn)(p, SHOW_INFO) )
	}
	else if (is_dir)
		ec_false( do_dir(p) )
	return true;

EC_CLEANUP_BGN
	return false;
EC_CLEANUP_END
}
/*[]*/
#ifdef WANT_LSDIR_BUGGY
/*[ls_dir]*/
int main(int argc, char *argv[]) /* has a bug */
{
	bool ok = false;
	int i;
	DIR *dir = NULL;
	struct dirent *entry;
	struct stat statbuf;

	for (i = 1; i < argc; i++) {
		ec_neg1( lstat(argv[i], &statbuf) )
		if (!S_ISDIR(statbuf.st_mode)) {
			ls_long(&statbuf, argv[i]);
			ok = true;
			EC_CLEANUP
		}
		ec_null( dir = opendir(argv[i]) )
		while (errno = 0, ((entry = readdir(dir)) != NULL)) {
			ec_neg1( lstat(entry->d_name, &statbuf) )
			ls_long(&statbuf, entry->d_name);
		}
		ec_nzero( errno )
	}
	ok = true;
	EC_CLEANUP

EC_CLEANUP_BGN
	if (dir != NULL)
		(void)closedir(dir);
	exit(ok ? EXIT_SUCCESS : EXIT_FAILURE);
EC_CLEANUP_END
}
/*[]*/
#endif /* WANT_LSDIR_BUGGY */
#ifdef WANT_LSDIR_FIXED
int main(int argc, char *argv[])
{
	bool ok = false;
	int i, fd;
	DIR *dir = NULL;
	struct dirent *entry;
	struct stat statbuf;

	for (i = 1; i < argc; i++) {
		ec_neg1( lstat(argv[i], &statbuf) )
		if (!S_ISDIR(statbuf.st_mode)) {
			ls_long(&statbuf, argv[i]);
			ok = true;
			EC_CLEANUP
		}
/*[ls_dir_fixed]*/
		ec_null( dir = opendir(argv[i]) )
		ec_neg1( fd = open(".", O_RDONLY) )
		ec_neg1( chdir(argv[i]) )
		while (errno = 0, ((entry = readdir(dir)) != NULL)) {
			ec_neg1( lstat(entry->d_name, &statbuf) )
			ls_long(&statbuf, entry->d_name);
		}
		ec_nzero( errno )
		ec_neg1( fchdir(fd) )
/*[]*/
	}
	ok = true;
	EC_CLEANUP

EC_CLEANUP_BGN
	if (dir != NULL)
		(void)closedir(dir);
	exit(ok ? EXIT_SUCCESS : EXIT_FAILURE);
EC_CLEANUP_END
}
#endif /* WANT_LSDIR_FIXED */
#ifdef WANT_LSDIR_ALTERNATIVE
int main(int argc, char *argv[])
{
	bool ok = false;
	int i, fd, lstat_result, errno_save;
	DIR *dir = NULL;
	struct dirent *entry;
	struct stat statbuf;

	for (i = 1; i < argc; i++) {
		ec_neg1( lstat(argv[i], &statbuf) )
		if (!S_ISDIR(statbuf.st_mode)) {
			ls_long(&statbuf, argv[i]);
			ok = true;
			EC_CLEANUP
		}
/*[ls_dir_fixed2]*/
		ec_null( dir = opendir(argv[i]) )
		ec_neg1( fd = open(".", O_RDONLY) )
		ec_neg1( chdir(argv[i]) ) /* no jumps allowed until fchdir! */
		while (errno = 0, ((entry = readdir(dir)) != NULL)) {
			if ((lstat_result = lstat(entry->d_name, &statbuf)) == -1)
				break;
			ls_long(&statbuf, entry->d_name);
		}
		errno_save = errno;
		ec_neg1( fchdir(fd) )
		errno = errno_save;
		ec_neg1( lstat_result )
		ec_nzero( errno )
/*[]*/
	}
	ok = true;
	EC_CLEANUP

EC_CLEANUP_BGN
	if (dir != NULL)
		(void)closedir(dir);
	exit(ok ? EXIT_SUCCESS : EXIT_FAILURE);
EC_CLEANUP_END
}
#endif /* WANT_LSDIR_ALTERNATIVE */

static bool ls_one(const char *name)
{
	struct stat statbuf;

	ec_neg1( lstat(name, &statbuf) )
	ls_long(&statbuf, name);
	return true;

EC_CLEANUP_BGN
	return false;
EC_CLEANUP_END
}

static char *big_cwd = NULL;
static char big_name[FILENAME_MAX + 1];
static off_t big_size = 0;

static bool biggest(struct traverse_info *p, SHOW_OP op)
{
	char *cwd;

	if (p->ti_stat.st_size > big_size && S_ISREG(p->ti_stat.st_mode)) {
		ec_null( cwd = get_cwd(false) )
		free(big_cwd);
		ec_null( big_cwd = malloc(strlen(cwd) + 1) )
		strcpy(big_cwd, cwd);
		strncpy(big_name, p->ti_name, sizeof(big_name));
		big_name[sizeof(big_name) - 1] = '\0';
		big_size = p->ti_stat.st_size;
	}
	return true;

EC_CLEANUP_BGN
	return false;
EC_CLEANUP_END
}

static void biggest_file(void)
{
	struct traverse_info ti = {0};

	ti.ti_fcn = biggest;
	ti.ti_recursive = true;
	ti.ti_name = "/";
	ec_false( do_entry(&ti, false) )
	printf("Biggest file:\n%s\n%s\n%lu\n", big_cwd, big_name,
	  (unsigned long)big_size);
	return;

EC_CLEANUP_BGN
	EC_FLUSH("biggest_file");
EC_CLEANUP_END
}

static bool runtest(char testtype, const char *name)
{
	struct stat statbuf;

	ec_neg1( lstat(name, &statbuf) )
	switch (testtype) {
	case 'a':
		printf("get_max_pathname = %ld\n", get_max_pathname("."));
		printf("get_max_pathname = %ld\n", get_max_pathname("/aup"));
		/* Following line failed on DARWIN */
		printf("get_max_pathname = %ld\n", get_max_pathname("/dev/fd"));
		break;
	case 'b':
		biggest_file();
		break;
	case 'y':
		ec_false( ls_one(name) )
		break;
	case 'c':
	{
/*[test_get_cwd]*/
char *cwd;

ec_null( cwd = get_cwd(false) )
printf("%s\n", cwd);
(void) get_cwd(true);
/*[]*/
ec_null( cwd = get_cwd(false) )
printf("%s\n", cwd);
(void) get_cwd(true);
ec_null( cwd = get_cwd(false) )
printf("%s\n", cwd);
(void) get_cwd(true);
ec_null( cwd = get_cwd(false) )
printf("%s\n", cwd);
(void) get_cwd(true);
	}
		break;
	case 'M':
{
/*[test_print_mode]*/
struct stat statbuf;

ec_neg1( lstat("fifo", &statbuf) )
print_mode(&statbuf);
putchar('\n');
ec_neg1( system("ls -l fifo") )
/*[]*/
}
		break;
	case 'm':
		print_mode(&statbuf);
		break;
	case 'l':
		print_numlinks(&statbuf);
		break;
	case 'o':
		print_owner(&statbuf);
		break;
	case 'g':
		print_group(&statbuf);
		break;
	case 's':
		print_size(&statbuf);
		break;
	case 'd':
		print_date(&statbuf);
		break;
	case 'n':
		print_name(&statbuf, name);
		break;
	default:
		fprintf(stderr, "Unknown test letter: %c\n", testtype);
		errno = 0;
		EC_FAIL
	}
	printf("\n");
	return true;

EC_CLEANUP_BGN
	return false;
EC_CLEANUP_END
}
#ifdef AUPLS_FULL
/*[aupls-main1]*/
int main(int argc, char *argv[])
{
	struct traverse_info ti = {0};
	int c, status = EXIT_FAILURE;
	bool stat_only = false;
/*[]*/
	char testtype = '\0';
/*[aupls-main2]*/

	ti.ti_fcn = show_stat;
	while ((c = getopt(argc, argv, "dRt:")) != -1)
		switch(c) {
		case 'd':
			stat_only = true;
			break;
		case 'R':
			ti.ti_recursive = true;
			break;
/*[]*/
		case 't':
			testtype = *optarg;
			break;
/*[aupls-main3]*/
		default:
			fprintf(stderr, USAGE);
			EC_CLEANUP
		}
	switch (argc - optind) {
	case 0:
		ti.ti_name = ".";
		break;
	case 1:
		ti.ti_name = argv[optind];
		break;
	default:
		fprintf(stderr, USAGE);
		EC_CLEANUP
	}
/*[]*/
	if (testtype != '\0')
		ec_false( runtest(testtype, ti.ti_name) )
	else {
/*[aupls-main4]*/
	ec_false( do_entry(&ti, stat_only) )
	printf("\nTotal entries: %ld; directories = %ld\n", total_entries,
	  total_dirs);
/*[]*/
	}
/*[aupls-main5]*/
	status = EXIT_SUCCESS;
	EC_CLEANUP

EC_CLEANUP_BGN
	print_cwd(true);
	exit(status);
EC_CLEANUP_END
}
/*[]*/
#endif
