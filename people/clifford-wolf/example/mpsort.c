/*
 *  Simple sort program using the OpenMP parallelization architecture
 *
 *  Copyright (C) 2010  Clifford Wolf <clifford@clifford.at>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 *  gcc -o mpsort -O2 -std=gnu99 -Wall -Wextra -fopenmp mpsort.c
 *
 *  This program is using parallelization only for the actual sorting.
 *  The input and output routines are implemented by plain serial logic.
 *
 *  WARNING: In most cases this program isn't really faster than the normal
 *  GNU sort command. It just demonstrates how to use OpenMP by giving
 *  a very simple example..
 */

#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int data_len, data_max_len;
char *data, *output;

int n, max_n;
char **lines;

int strp_cmp(const void *vp1, const void *vp2)
{
	const char **p1 = (void*)vp1, **p2 = (void*)vp2;
	return strcmp(*p1, *p2);
}

int main(int argc, char **argv)
{
	int cores = argc == 2 ? atoi(argv[1]) : -1;

	if (cores <= 0) {
		fprintf(stderr, "Usage: %s n_cores < input > output\n", argv[0]);
		return 1;
	}

	/* input */
	data_max_len = 1024*1024;
	data = malloc(data_max_len);
	while (1) {
		int rc = read(0, data+data_len, data_max_len-data_len);
		if (rc < 0) {
			perror("read error");
			return 1;
		}
		if (rc == 0)
			break;
		data_len += rc;
		if (data_len == data_max_len) {
			data_max_len *= 2;
			data = realloc(data, data_max_len);
		}
	}
	if (data_len == data_max_len) {
		data_max_len++;
		data = realloc(data, data_max_len);
	}
	data[data_len+1] = 0;

	/* slicing */
	max_n = 1024;
	lines = malloc(sizeof(char*)*max_n);
	lines[n++] = data;
	for (int i=0; i<data_len; i++) {
		if (data[i] == '\n' || data[i] == 0) {
			data[i] = 0;
			lines[n++] = data+i+1;
			if (n == max_n) {
				max_n *= 2;
				lines = realloc(lines, sizeof(char*)*max_n);
			}
		}
	}

	/* sort segments */
	int segsize = n/cores;
	char **p[cores], **lastp[cores];
#pragma omp parallel for
	for (int i=0; i<cores; i++) {
		int first = i*segsize;
		int len = (i == cores-1 ? n-1 : (i+1)*segsize) - first;
		qsort(lines+first, len, sizeof(char*), strp_cmp);
		lastp[i] = lines+first+len;
		p[i] = lines+first;
	}

	/* merge segments */
	output = malloc(data_len+2);
	char *o = output;
	while (1) {
		int next = -1;
		for (int i=0; i<cores; i++) {
			if (p[i] == lastp[i])
				continue;
			if (next < 0)
				next = i;
			else if (strcmp(*p[i], *p[next]) < 0)
				next = i;
		}
		if (next < 0)
			break;
		int slen = strlen(*p[next]);
		memcpy(o, *p[next], slen);
		o[slen] = '\n';
		o += slen + 1;
		p[next]++;
	}

	/* output */
	for (int i = 0; i < o-output;) {
		int rc = write(1, output+i, o-output-i);
		if (rc <= 0)
			perror("write error");
		i += rc;
	}

	return 0;
}

