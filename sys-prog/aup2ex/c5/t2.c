/*
	Thread example (background sorting; no mutexes)
	AUP2, Sec. 5.17 (not in book)

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
#include <pthread.h>

#define MAX_ENTRIES 100

struct dict {
	int d_nentries;
	char *d_entry[MAX_ENTRIES];
	bool d_issorted;
};

static int compar(const void *x, const void *y)
{
	return strcmp(*(char **)x, *(char **)y);
}

static void *sorter(void *arg)
{
	struct dict *d = arg;

	while (true) {
		if (!d->d_issorted) {
			qsort(d->d_entry, d->d_nentries, sizeof(char *), compar);
			d->d_issorted = true;
		}
		sleep(1);
	}
}

int main(void)
{
	struct dict dictionary;
	pthread_t tid;
	char word[100];
	int i;
	size_t wordlen;

	dictionary.d_nentries = 0;
	dictionary.d_issorted = true;
	ec_rv( pthread_create(&tid, NULL, sorter, &dictionary) )
	while (!feof(stdin)) {
		printf("Word? ");
		if (fgets(word, sizeof(word), stdin) == NULL && ferror(stdin))
			EC_FAIL
		if (word[0] == '-') {
			if (dictionary.d_issorted)
				for (i = 0; i < dictionary.d_nentries; i++)
					printf("\t%s\n", dictionary.d_entry[i]);
			else
				printf("Not sorted -- try again later\n");
			continue;
		}
		wordlen = strlen(word);
		if (word[wordlen - 1] == '\n')
			word[--wordlen] = '\0';
		if (dictionary.d_nentries >= MAX_ENTRIES) {
			errno = E2BIG;
			EC_FAIL
		}
		ec_null( dictionary.d_entry[dictionary.d_nentries] =
		  malloc(wordlen + 1) )
		strcpy(dictionary.d_entry[dictionary.d_nentries], word);
		/*
			Make entry visible to sorter. Value of d_issorted is
			unknown.
		*/
		dictionary.d_nentries++;
		/*
			Force sort. May be a redundant one because sort may
			occur between previous statement and next. In this case
			an extra sort will occur later.
		*/
		dictionary.d_issorted = false;
	}
	return EXIT_SUCCESS;

EC_CLEANUP_BGN
	return EXIT_FAILURE;
EC_CLEANUP_END
}
