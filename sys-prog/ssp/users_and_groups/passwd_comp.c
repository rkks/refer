#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <shadow.h>
#include <string.h>
#include "ssp.h"

static boolean_t compare_passwd (const char *user, const char *passwd);

int main (int argc, char **argv)
{
	char *pass;

	if (argc !=2)
		err_quit ("Usage: passwd_comp username");

	pass = getpassphrase ("Password: ");
	printf ("Password = \"%s\"\n", pass);

	if (compare_passwd (argv [1], pass))
		printf ("Passwords match\n");
	else
		printf ("Passwords don't match\n");

	return (0);
}

static boolean_t compare_passwd (const char *user, const char *passwd)
{
	struct spwd *shadow_ent;
	char *secret;

	if ((shadow_ent = getspnam (user)) == NULL)
		err_msg ("Can't get shadow database entry for %s", user);

	secret = crypt (passwd, shadow_ent -> sp_pwdp);

	return (strcmp (secret, shadow_ent -> sp_pwdp) == 0);
}
