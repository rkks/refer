#include <stdio.h>
#include <stdlib.h>
#include <sys/acl.h>
#include "ssp.h"

int main (int argc, char **argv)
{
	int num_acls;
	aclent_t *acl_buf;
	char *acl_text;
	int i;

	if (argc < 2)
		err_quit ("Usage: get_acl file ...");

	for (i = 1; i < argc; i++) {
		if ((num_acls = acl (argv [i], GETACLCNT, 0, NULL)) == -1)
			err_ret ("acl (GETACLCNT) failed for %s", argv [i]);

		if (num_acls == MIN_ACL_ENTRIES) {
			printf ("%s: Trivial ACL\n", argv [i]);
		}
		else {
			if ((acl_buf = malloc (sizeof (aclent_t) * num_acls)) == NULL)
				err_ret ("Malloc failed");

			if (acl (argv [i], GETACL, num_acls, acl_buf) != num_acls)
				err_ret ("acl (GETACL) failed for %s", argv [i]);

			acl_text = acltotext (acl_buf, num_acls);
			printf ("%s: %s\n", argv [i], acl_text);

			free (acl_text);
			free (acl_buf);
		}
	}

	return (0);
}
