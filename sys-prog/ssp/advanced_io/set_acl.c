#include <stdio.h>
#include <stdlib.h>
#include <sys/acl.h>
#include "ssp.h"

int main (int argc, char **argv)
{
	int num_acls;
	aclent_t *acl_buf;
	int i;
	int which;

	if (argc < 3)
		err_quit ("Usage: set_acl ACL file ...");

	if ((acl_buf = aclfromtext (argv [1], &num_acls)) == NULL)
		err_msg ("aclfromtext failed");

	switch (aclcheck (acl_buf, num_acls, &which)) {
		case 0:
			break;

		case USER_ERROR:
			err_quit ("Invalid ACL entry: %d: "
				"Multiple USER_OBJ or DEF_USER_OBJ entries", which);
			break;

		case GRP_ERROR:
			err_quit ("Invalid ACL entry: %d: "
				"Multiple GROUP_OBJ or DEF_GROUP_OBJ entries", which);
			break;

		case OTHER_ERROR:
			err_quit ("Invalid ACL entry: %d: "
				"Multiple OTHER_OBJ or DEF_OTHER_OBJ entries", which);
			break;

		case CLASS_ERROR:
			err_quit ("Invalid ACL entry: %d: "
				"Multiple CLASS_OBJ or DEF_CLASS_OBJ entries", which);
			break;

		case DUPLICATE_ERROR:
			err_quit ("Invalid ACL entry: %d: "
				"Duplicate USER, GROUP, DEF_USER, or DEF_GROUP entries", which);
			break;

		case ENTRY_ERROR:
			err_quit ("Invalid ACL entry: %d: Invalid entry type", which);
			break;

		case MISS_ERROR:
			err_quit ("Missing ACL entries");
			break;

		case MEM_ERROR:
			err_quit ("Out of memory!");
			break;

		default:
			err_quit ("aclcheck returns unknown error");
			break;
	}

	for (i = 2; i < argc; i++) {
		if (acl (argv [i], SETACL, num_acls, acl_buf) != 0)
			err_ret ("acl (SETACL) failed for %s", argv [i]);
	}

	free (acl_buf);

	return (0);
}
