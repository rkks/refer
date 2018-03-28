/*
	getcwd
	AUP2, Sec. 3.6.4

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
#include <dirent.h>

/*[push_pathlist]*/
struct pathlist_node {
	struct pathlist_node *c_next;
	char c_name[1]; /* flexible array */
};

static bool push_pathlist(struct pathlist_node **head, const char *name)
{
	struct pathlist_node *p;

	ec_null( p = malloc(sizeof(struct pathlist_node) + strlen(name)) )
	strcpy(p->c_name, name);
	p->c_next = *head;
	*head = p;
	return true;

EC_CLEANUP_BGN
	return false;
EC_CLEANUP_END
}
/*[free_pathlist]*/
static void free_pathlist(struct pathlist_node **head)
{
	struct pathlist_node *p, *p_next;

	for (p = *head; p != NULL; p = p_next) {
		p_next = p->c_next;
		free(p);
	}
	*head = NULL;
}
/*[get_pathlist]*/
static char *get_pathlist(struct pathlist_node *head)
{
	struct pathlist_node *p;
	char *path;
	size_t total = 0;

	for (p = head; p != NULL; p = p->c_next)
		total += strlen(p->c_name) + 1;
	ec_null( path = malloc(total + 1) )
	path[0] = '\0';
	for (p = head; p != NULL; p = p->c_next) {
		strcat(path, "/");
		strcat(path, p->c_name);
	}
	return path;

EC_CLEANUP_BGN
	return NULL;
EC_CLEANUP_END
}
/*[SAME_INODE]*/
#define SAME_INODE(s1, s2)	((s1).st_dev == (s2).st_dev &&\
							(s1).st_ino == (s2).st_ino)
/*[getcwdx]*/
char *getcwdx(void)
{
	struct stat stat_child, stat_parent, stat_entry;
	DIR *sp = NULL;
	struct dirent *dp;
	struct pathlist_node *head = NULL;
	int dirfd = -1, rtn;
	char *path = NULL;

	ec_neg1( dirfd = open(".", O_RDONLY) )
	ec_neg1( lstat(".", &stat_child) )
	while (true) {
		ec_neg1( lstat("..", &stat_parent) )

		/* change to parent and detect root */
		if (((rtn = chdir("..")) == -1 && errno == ENOENT) ||
		  SAME_INODE(stat_child, stat_parent)) {
			if (head == NULL)
				ec_false( push_pathlist(&head, "") )
			ec_null( path = get_pathlist(head) )
			EC_CLEANUP
		}
		ec_neg1( rtn )

		/* read directory looking for child */
		ec_null( sp = opendir(".") )
		while (errno = 0, (dp = readdir(sp)) != NULL) {
			ec_neg1( lstat(dp->d_name, &stat_entry) )
			if (SAME_INODE(stat_child, stat_entry)) {
				ec_false( push_pathlist(&head, dp->d_name) )
				break;
			}
		}
		if (dp == NULL) {
			if (errno == 0)
				errno = ENOENT;
			EC_FAIL
		}
		stat_child = stat_parent;
	}

EC_CLEANUP_BGN
	if (sp != NULL)
		(void)closedir(sp);
	if (dirfd != -1) {
		(void)fchdir(dirfd);
		(void)close(dirfd);
	}
	free_pathlist(&head);
	return path;
EC_CLEANUP_END
}
/*[main1]*/
int main(void)
{
	char *path;

/*[]*/
{
/*[push_pathlist-example]*/
struct pathlist_node *head = NULL;
char *path;

ec_false( push_pathlist(&head, "grandchild") )
ec_false( push_pathlist(&head, "child") )
ec_false( push_pathlist(&head, "parent") )
ec_null( path = get_pathlist(head) );
free_pathlist(&head);
printf("%s\n", path);
free(path);
/*[]*/
/*[main2]*/
}
/*[main2]*/
	ec_null( path = getcwdx() )
	printf("%s\n", path);
	free(path);
	exit(EXIT_SUCCESS);

EC_CLEANUP_BGN
	exit(EXIT_FAILURE);
EC_CLEANUP_END
}
/*[]*/

#if 0 /* old function -- not used anymore */
static bool prepend(char **p, size_t *alloc, const char *name)
{
	size_t namelen, plen;

	if (*p == NULL) {
		*alloc = 2;
		ec_null( *p = malloc(*alloc) )
		strcpy(*p, "/");
		return true;
	}
	namelen = strlen(name);
	*alloc += namelen + 1; /* +1 for slash */
	ec_null( *p = realloc(*p, *alloc) )
	plen = strlen(*p);
	if (plen > 1) /* move slash only with name */
		memmove(*p + namelen + 1, *p, strlen(*p) + 1);
	memcpy(*p + 1, name, namelen);
	**p = '/';
	return true;

EC_CLEANUP_BGN
	return false;
EC_CLEANUP_END
}
#endif
