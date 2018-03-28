/*
 * Solution to exercise 5-5 in K&R2, page 107:
 *
 *	Write versions of the library functions strncpy, strncat,
 *	and strncmp, which operate on at most the first n characters
 *	of their argument strings. For example, strncpy(s,t,n) copies
 *	at most n characters of t to s. Full descriptions are in
 *	Appendix B.
 *
 * Note that the description in the exercise is not precise. Here are
 * descriptions from Appendix B (though one should really follow the
 * descriptions in the standard):
 *
 *	char *strncpy(s,ct,n)	copy at most n characters of string ct
 *				to s, return s. Pad with '\0's is ct
 *				has fewer than n characters.
 *	char *strncat(s,ct,n)	concatenate at most n characters of
 *				string ct to string s, terminate s with
 *				'\0'; return s.
 *	int strncmp(cs,ct,n)	compare at most n characters of string
 *				cs to string ct; return <0 if cs<ct,
 *				0 if cs==ct, or >0 if cs>ct.
 *
 * Further note that the standard requires strncmp to compare the
 * characters using unsigned char internally.
 * 
 * Implementation note: since the function names are reserved by the
 * standard, I've used the prefix `liw_'. This also allows me to check
 * the functions against the standard library versions. For each library
 * function, I've written a test function that tests a particular test
 * case. Where appropriate, the test functions use internal buffers that
 * are of size MAX_BUF; at least some of the test cases should be longer
 * to test all boundary conditions.
 *
 * Feel free to modify, copy, and use as you wish.
 *
 * Lars Wirzenius <liw@iki.fi>
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_BUF 16

char *liw_strncpy(char *s, const char *ct, size_t n) {
	char *p;
	
	p = s;
	for (; n > 0 && *ct != '\0'; --n)
		*p++ = *ct++;
	for (; n > 0; --n)
		*p++ = '\0';
	return s;
}

char *liw_strncat(char *s, const char *ct, size_t n) {
	char *p;
	
	p = s;
	while (*p != '\0')
		++p;
	for (; n > 0 && *ct != '\0'; --n)
		*p++ = *ct++;
	*p = '\0';
	return s;
}

int liw_strncmp(const char *cs, const char *ct, size_t n) {
	while (n > 0 && *cs == *ct && *cs != '\0') {
		++cs;
		++ct;
		--n;
	}
	if (n == 0 || *cs == *ct)
		return 0;
	if (*(unsigned char *) cs < *(unsigned char *) ct)
		return -1;
	return 1;
}

void test_ncpy(const char *str) {
	char std_buf[MAX_BUF];
	char liw_buf[MAX_BUF];

	memset(std_buf, 0x42, sizeof(std_buf));
	strncpy(std_buf, str, sizeof(std_buf));

	memset(liw_buf, 0x42, sizeof(liw_buf));
	liw_strncpy(liw_buf, str, sizeof(liw_buf));

	if (memcmp(std_buf, liw_buf, sizeof(std_buf)) != 0) {
		fprintf(stderr, "liw_strncpy failed for <%s>\n", str);
		exit(EXIT_FAILURE);
	}
}

void test_ncat(const char *first, const char *second) {
	char std_buf[MAX_BUF];
	char liw_buf[MAX_BUF];

	memset(std_buf, 0x69, sizeof(std_buf));
	strcpy(std_buf, first);
	strncat(std_buf, second, sizeof(std_buf) - strlen(std_buf) - 1);

	memset(liw_buf, 0x69, sizeof(liw_buf));
	strcpy(liw_buf, first);
	liw_strncat(liw_buf, second, sizeof(liw_buf) - strlen(liw_buf) - 1);
	
	if (memcmp(std_buf, liw_buf, sizeof(std_buf)) != 0) {
		fprintf(stderr, "liw_strncat failed, <%s> and <%s>\n",
			first, second);
		exit(EXIT_FAILURE);
	}
}

void test_ncmp(const char *first, const char *second) {
	size_t len;
	int std_ret, liw_ret;

	if (strlen(first) < strlen(second))
		len = strlen(second);
	else
		len = strlen(first);
	std_ret = strncmp(first, second, len);
	liw_ret = liw_strncmp(first, second, len);
	if ((std_ret < 0 && liw_ret >= 0) || (std_ret > 0 && liw_ret <= 0) ||
	    (std_ret == 0 && liw_ret != 0)) {
		fprintf(stderr, "liw_strncmp failed, <%s> and <%s>\n",
			first, second);
		exit(EXIT_FAILURE);
	}
}

int main(void) {
	test_ncpy("");
	test_ncpy("a");
	test_ncpy("ab");
	test_ncpy("abcdefghijklmnopqrstuvwxyz");     /* longer than MAX_BUF */
	
	test_ncat("", "a");
	test_ncat("a", "bc");
	test_ncat("ab", "cde");
	test_ncat("ab", "cdefghijklmnopqrstuvwxyz"); /* longer than MAX_BUF */

	test_ncmp("", "");
	test_ncmp("", "a");
	test_ncmp("a", "a");
	test_ncmp("a", "ab");
	test_ncmp("abc", "ab");

	printf("All tests pass.\n");
	return 0;
}
