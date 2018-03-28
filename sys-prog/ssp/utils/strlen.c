#include <stdio.h>
#include <string.h>

int main (void)
{
	char *s1 = "aabbccdd12345678";
	char *s2 = "abc123";
	char *s3 = "123";

	printf ("strlen (\"%s\") = %d\n", s1, strlen (s1));
	printf ("strspn (\"%s\", \"%s\") = %d\n", s1, s2, strspn (s1, s2));
	printf ("strcspn (\"%s\", \"%s\") = %d\n", s1, s3, strcspn (s1, s3));

	return (0);
}
