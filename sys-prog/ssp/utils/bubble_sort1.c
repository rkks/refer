#include <stdio.h>
#include <string.h>

#define STR_LEN 80
#define NUM_STR 1024

static void bubble_sort (char **string_ptrs, int num_strings);

int main (void)
{
	char strings [NUM_STR] [STR_LEN];
	char *string_ptrs [NUM_STR];
	int num_strings;
	int i;

	for (num_strings = 0; num_strings < NUM_STR; num_strings++) {
		if (fgets (strings [num_strings], STR_LEN, stdin) == NULL)
			break;
		strings [num_strings] [strlen (strings [num_strings]) - 1] = '\0';
		string_ptrs [num_strings] = strings [num_strings];
	}

	bubble_sort (string_ptrs, num_strings);

	for (i = 0; i < num_strings; i++) {
		printf ("String %d = '%s'\n", i, string_ptrs [i]);
	}

	return (0);
}

static void bubble_sort (char **string_ptrs, int num_strings)
{
	char *tmp_string;
	int i;
	int done;

	done = 0;

	while (!done) {
		done = 1;
		num_strings--;
		for (i = 0; i < num_strings; i++) {
			if (strcmp (string_ptrs [i], string_ptrs [i + 1]) > 0) {
				tmp_string = string_ptrs [i];
				string_ptrs [i] = string_ptrs [i + 1];
				string_ptrs [i + 1] = tmp_string;
				done = 0;
			}
		}
	}
}
