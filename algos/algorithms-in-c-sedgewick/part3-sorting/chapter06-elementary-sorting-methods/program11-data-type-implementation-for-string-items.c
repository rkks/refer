/* Program 6.11: Data-type implementation for string items
 * This implementation allows us to use our sorting programs to sort
 * strings. A string is a pointer to a character, so a sort will process
 * an array of pointers to characters, rearranging them so the indicated
 * strings are in alphanumeric order. We statically allocate the sotrage
 * buffer containing the string characters in this module; dynamic
 * allocation is perhaps more appropriate. The ITEMrand implementation is omitted.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string,h>
#include "Item.h"

static char buf[100000];
static int cnt = 0;

int ITEMscan(char **x) {
	int t;
	*x = &buf[cnt];
	t = scanf("%s", *x);
	cnt += strlen(*x)+1;
	return t;
}
void ITEMshow(char *x) {
	printf("%s ", x);
}
