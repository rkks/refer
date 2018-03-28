/*
 *  xtremesort.c - Example for sorting large arrays
 *
 *  Copyright (C) 2008  Clifford Wolf <clifford@clifford.at>
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
 *
 *  This program (as it is) sorts a large (max. 16GB) array of 128 bit values
 *  with a memory footprint of only 512 MB. This is only an example. It can
 *  be adapted easily for other data structures or different array sizes.
 *
 *  Possible improvements:
 *
 *     - A dynamic number of stages (for huge arrays)
 *     - More efficient I/O handling
 *
 *  Test procedure:
 *
 *     Prepare:
 *     $ gcc -o xtremesort -ggdb -O2 -Wall -pthread xtremesort.c -lm
 *     $ dd if=/dev/urandom of=sort_input.dat bs=1M count=16384
 *
 *     Sort:
 *     $ ./xtremesort sort sort_input.dat sort_tempfile.dat sort_output.dat
 *
 *     Check sort results:
 *     $ ./xtremesort check sort_output.dat
 *
 *     Check if records in output match records in input:
 *     $ ./xtremesort sum sort_output.dat
 *     $ ./xtremesort sum sort_input.dat
 *
 *     Query memory usage:
 *     $ ./xtremesort query sort_input.dat
 *
 *  Happy hacking!
 *
 */


#define _GNU_SOURCE
#define XTREMESORT_ASYNC_IO

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/times.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>
#include <math.h>

#ifdef XTREMESORT_ASYNC_IO
#  include <pthread.h>
#endif

/******************  RECORD TYPE AND SORT ORDER  ******************/

struct record {
	unsigned int a, b, c, d;
};

static int compare_records(struct record *x, struct record *y)
{
	if (x->a < y->a) return -1;
	if (x->a > y->a) return +1;

	if (x->b < y->b) return -1;
	if (x->b > y->b) return +1;

	if (x->c < y->c) return -1;
	if (x->c > y->c) return +1;

	if (x->d < y->d) return -1;
	if (x->d > y->d) return +1;

	return 0;
}

/******************  DETECTION OF BEST BLOCK SIZES  ******************/

off64_t blocksize_check = (1024*1024*16) /  sizeof(struct record);
off64_t blocksize_stage1;
off64_t blocksize_stage2;

static off64_t mem_detect(const char *input_file, off64_t forced_mem_size)
{
	off64_t mem_avail = 0;
	off64_t file_size;
	struct stat64 st;

	if (stat64(input_file, &st) != 0) {
		fprintf(stderr, "Can not stat input file: %s\n", input_file);
		return 1;
	}
	file_size = st.st_size;

	FILE *f = fopen("/proc/meminfo", "r");
	if (f) {
		char buf[512];
		while (fgets(buf, 512, f) == buf) {
			if (strncmp(buf, "Inactive:", strlen("Inactive:")) != 0)
				continue;
			int kb_num;
			if (sscanf(buf, "Inactive: %d kB", &kb_num) == 1)
				mem_avail = (off64_t)kb_num * 1024;
		}
		fclose(f);
	}

	blocksize_stage2 = 1024*1024*4;
	blocksize_stage1 = sqrt(file_size*(blocksize_stage2/2.0));

	if (mem_avail && forced_mem_size && mem_avail < forced_mem_size)
		fprintf(stderr, "WARING: Can auto-detect only %.2f MB of the %.2f MB specified!\n",
				mem_avail/(1024.0*1024.0), forced_mem_size/(1024.0*1024.0));

	if (forced_mem_size)
		mem_avail = forced_mem_size;

	if (!mem_avail) {
		fprintf(stderr, "WARING: Can't auto-detect mem size!\n");
	} else
	if (mem_avail < 2*blocksize_stage1) {
		fprintf(stderr, "WARING: Only %.2f MB of free (inactive) RAM available!\n",
				mem_avail/(1024.0*1024.0));
		fprintf(stderr, "For sorting a %.2f MB file %.2f MB RAM is recommended.\n",
				file_size/(1024.0*1024.0), blocksize_stage1*(2.0/(1024*1024)));
		blocksize_stage1 = mem_avail / 2;
		blocksize_stage2 = (2.0*blocksize_stage1*blocksize_stage1) / file_size;
	}

	blocksize_stage1 = blocksize_stage1 / sizeof(struct record);
	blocksize_stage2 = blocksize_stage2 / sizeof(struct record);
	return file_size / sizeof(struct record);
}

/******************  GENERIC SORTING CODE  ******************/

static int sync_read_count;
static int async_read_count;
static int async_wait_count;
static int sync_write_count;

static ssize_t sync_read_pos(int fd, void *buf, off64_t pos, ssize_t count)
{
	int rc, n;
	sync_read_count++;
	lseek64(fd, pos, SEEK_SET);
	for (n=0; n<count; n+=rc) {
		rc = read(fd, buf+n, count-n);
		if (rc < 0) {
			fprintf(stderr, "[Read Error]\n");
			exit(1);
		}
		if (rc == 0)
			break;
	}
	return n;
}

static ssize_t sync_write(int fd, const void *buf, ssize_t count)
{
	int rc, n;
	sync_write_count++;
	for (n=0; n<count; n+=rc) {
		rc = write(fd, buf+n, count-n);
		if (rc < 0) {
			fprintf(stderr, "[Write Error]\n");
			exit(1);
		}
		if (rc == 0)
			break;
	}
	return n;
}

struct aio_hdl {
#ifdef XTREMESORT_ASYNC_IO
	struct aio_hdl *next;
	pthread_cond_t cond;
	pthread_mutex_t mutex;

	int fd;
	void *buf;
	off64_t pos;
	ssize_t count;
	int done;
#endif
	ssize_t rc;
};

#ifdef XTREMESORT_ASYNC_IO
static pthread_t aio_thread;
static struct aio_hdl *aio_todo_list = NULL;
static struct aio_hdl *aio_todo_list_last = NULL;
static pthread_cond_t aio_todo_list_cond = PTHREAD_COND_INITIALIZER;
static pthread_mutex_t aio_todo_list_mutex = PTHREAD_MUTEX_INITIALIZER;

static void *aio_thread_main(void *arg)
{
	while (1)
	{
		struct aio_hdl *current;

		pthread_mutex_lock(&aio_todo_list_mutex);
		while (!aio_todo_list)
			pthread_cond_wait(&aio_todo_list_cond, &aio_todo_list_mutex);
		current = aio_todo_list;
		aio_todo_list = aio_todo_list->next;
		if (!aio_todo_list)
			aio_todo_list_last = NULL;
		pthread_mutex_unlock(&aio_todo_list_mutex);

		pthread_mutex_lock(&current->mutex);
		current->rc = sync_read_pos(current->fd, current->buf,
				current->pos, current->count);
		current->done = 1;
		pthread_cond_signal(&current->cond);
		pthread_mutex_unlock(&current->mutex);
	}
}
#endif

static void async_read_pos(struct aio_hdl *hdl, int fd, void *buf, off64_t pos, ssize_t count)
{
	async_read_count++;
#ifdef XTREMESORT_ASYNC_IO
	pthread_cond_init(&hdl->cond, NULL);
	pthread_mutex_init(&hdl->mutex, NULL);

	hdl->next = NULL;
	hdl->fd = fd;
	hdl->buf = buf;
	hdl->pos = pos;
	hdl->count = count;
	hdl->done = 0;
	hdl->rc = 0;
	
	pthread_mutex_lock(&aio_todo_list_mutex);
	if (aio_todo_list_last)
		aio_todo_list_last->next = hdl;
	else
		aio_todo_list = hdl;
	aio_todo_list_last = hdl;
	pthread_cond_signal(&aio_todo_list_cond);
	pthread_mutex_unlock(&aio_todo_list_mutex);
#else
	hdl->rc = sync_read_pos(fd, buf, pos, count);
#endif
}

static ssize_t async_read_pos_finalize(struct aio_hdl *hdl)
{
#ifdef XTREMESORT_ASYNC_IO
	pthread_mutex_lock(&hdl->mutex);
	if (!hdl->done)
		async_wait_count++;
	while (!hdl->done)
		pthread_cond_wait(&hdl->cond, &hdl->mutex);
	pthread_mutex_unlock(&hdl->mutex);

	pthread_cond_destroy(&hdl->cond);
	pthread_mutex_destroy(&hdl->mutex);
#endif
	return hdl->rc;
}

int main(int argc, char **argv)
{
#ifdef XTREMESORT_ASYNC_IO
	pthread_create(&aio_thread, NULL, &aio_thread_main, NULL);
#endif

	if (argc == 3 && !strcmp(argv[1], "check"))
	{
		long long recnum = 0;
		struct record *buffer = malloc(sizeof(struct record)*blocksize_check);
		struct record rem;
		int first_iter = 1;
		int error_count = 0;
		FILE *inf = fopen64(argv[2], "r");
		if (!inf) {
			fprintf(stderr, "Can not open input file: %s\n", argv[2]);
			return 1;
		}
		fprintf(stderr, "Checking..");
		while (1) {
			int i, n;
			n = fread(buffer, sizeof(struct record), blocksize_check, inf);
			if (n == 0) {
				break;
			}
			if (!first_iter) {
				if (compare_records(&rem, &buffer[0]) > 0) {
					fprintf(stderr, "\nRecord #%Ld is out of sort order.", recnum);
					error_count++;
				}
			}
			for (i=1; i<n; i++) {
				if (compare_records(&buffer[i-1], &buffer[i]) > 0) {
					fprintf(stderr, "\nRecord #%Ld is out of sort order.", recnum+i);
					error_count++;
				}
			}
			recnum += n;
			rem = buffer[n-1];
			fprintf(stderr, ".");
		}
		fclose(inf);
		if (error_count) {
			fprintf(stderr, "\nFound %d errors.\n", error_count);
			return 1;
		} else {
			fprintf(stderr, "\nPassed sort check.\n");
			return 0;
		}
	}

	if (argc == 3 && !strcmp(argv[1], "sum"))
	{
		long long recnum = 0;
		unsigned char xorbuf[sizeof(struct record)] = {};
		struct record *buffer = malloc(sizeof(struct record)*blocksize_check);
		FILE *inf = fopen64(argv[2], "r");
		if (!inf) {
			fprintf(stderr, "Can not open input file: %s\n", argv[2]);
			return 1;
		}
		fprintf(stderr, "Calculating..");
		while (1) {
			int i, j, n;
			n = fread(buffer, sizeof(struct record), blocksize_check, inf);
			if (n == 0)
				break;
			for (i=0; i<n; i++) {
				unsigned char *d = (void*)&buffer[i];
				for (j=0; j<sizeof(struct record); j++)
					xorbuf[j] ^= d[j];
			}
			recnum += n;
			fprintf(stderr, ".");
		}
		fclose(inf);
		int j;
		fprintf(stderr, "\n");
		for (j=0; j<sizeof(struct record); j++)
			fprintf(stderr, "%02x", xorbuf[j]);
		fprintf(stderr, " %Ld\n", (long long)recnum);
		return 0;
	}

	if ((argc == 5 || argc == 6) && !strcmp(argv[1], "sort"))
	{
		int queuenum = 0;
		int in_fd, temp_fd, out_fd;
		clock_t c1, c2;
		struct tms t1, t2;

		if (argc == 5)
			mem_detect(argv[2], 0);
		else
			mem_detect(argv[2], atoi(argv[5])*(off64_t)(1024*1024));

		in_fd = open(argv[2], O_RDONLY|O_LARGEFILE);
		if (in_fd < 0) {
			fprintf(stderr, "Can not open input file: %s\n", argv[2]);
			return 1;
		}

		temp_fd = open(argv[3], O_RDWR|O_TRUNC|O_LARGEFILE|O_CREAT, 0600);
		if (temp_fd < 0) {
			fprintf(stderr, "Can not open temp file: %s\n", argv[3]);
			return 1;
		}

		out_fd = open(argv[4], O_RDWR|O_TRUNC|O_LARGEFILE|O_CREAT, 0644);
		if (out_fd < 0) {
			fprintf(stderr, "Can not open output file: %s\n", argv[4]);
			return 1;
		}

		c1 = times(&t1);
		sync_read_count = 0;
		async_read_count = 0;
		async_wait_count = 0;
		sync_write_count = 0;
		fprintf(stderr, "Stage 1 sorting..");
		{
			struct record *buffer_rd = malloc(sizeof(struct record)*blocksize_stage1);
			struct record *buffer_sort = malloc(sizeof(struct record)*blocksize_stage1);
			int n = sync_read_pos(in_fd, buffer_sort, 0, sizeof(struct record)*blocksize_stage1);
			struct aio_hdl aioh;
			while (1) {
				async_read_pos(&aioh, in_fd, buffer_rd,
						sizeof(struct record)*(off64_t)blocksize_stage1*(queuenum+1),
						sizeof(struct record)*blocksize_stage1);
				if (n > 0) {
					qsort(buffer_sort, n/sizeof(struct record), sizeof(struct record),
							(int(*)(const void*,const void*))compare_records);
					sync_write(temp_fd, buffer_sort, n);
					queuenum++;
				}
				if (n != sizeof(struct record)*blocksize_stage1) {
					async_read_pos_finalize(&aioh);
					break;
				}
				fprintf(stderr, ".");

				n = async_read_pos_finalize(&aioh);
				struct record *buffer_tmp = buffer_rd;
				buffer_rd = buffer_sort;
				buffer_sort = buffer_tmp;
			}
			fprintf(stderr, "\n");
			free(buffer_sort);
			free(buffer_rd);
		}
		c2 = times(&t2);
		fprintf(stderr, "    sync reads: %d\n", sync_read_count);
		fprintf(stderr, "   async reads: %d\n", async_read_count);
		fprintf(stderr, "   async waits: %d\n", async_wait_count);
		fprintf(stderr, "        writes: %d\n", sync_write_count);
		fprintf(stderr, "     time real: %.2f s\n", (float)(c2-c1)/sysconf(_SC_CLK_TCK));
		fprintf(stderr, "     time user: %.2f s\n", (float)(t2.tms_utime-t1.tms_utime)/sysconf(_SC_CLK_TCK));
		fprintf(stderr, "      time sys: %.2f s\n", (float)(t2.tms_stime-t1.tms_stime)/sysconf(_SC_CLK_TCK));

		c1 = times(&t1);
		sync_read_count = 0;
		async_read_count = 0;
		async_wait_count = 0;
		sync_write_count = 0;
		fprintf(stderr, "Stage 2 sorting..");
		{
			struct record *buffer_p[queuenum];
			struct record *shadowbuf_p[queuenum];

			unsigned int buffer_off[queuenum];
			unsigned int buffer_max[queuenum];
			unsigned int buffer_pos[queuenum];

			unsigned int buffer_aio[queuenum];
			struct aio_hdl buffer_hdl[queuenum];

			struct record *wrbuf;
			unsigned int wroff;

			int i, j=0;

			for (i=0; i<queuenum; i++) {
				buffer_pos[i] = 0;
				buffer_p[i] = malloc(sizeof(struct record)*blocksize_stage2);
				shadowbuf_p[i] = malloc(sizeof(struct record)*blocksize_stage2);
				buffer_max[i] = sync_read_pos(temp_fd, buffer_p[i],
						sizeof(struct record)*(((off64_t)blocksize_stage1)*i),
						sizeof(struct record)*blocksize_stage2) / sizeof(struct record);
				buffer_off[i] = 0;
				buffer_aio[i] = 0;
			}

			wrbuf = malloc(sizeof(struct record)*blocksize_stage2);
			wroff = 0;

			while (1) {
				int next_queue = -1;
				for (i=0; i<queuenum; i++) {
					if (!buffer_aio[i]) {
						int rd_size = blocksize_stage2;
						if (buffer_pos[i]+rd_size*2 > blocksize_stage1)
							rd_size = blocksize_stage1 - (buffer_pos[i]+blocksize_stage2);
						if (rd_size > 0) {
							async_read_pos(&buffer_hdl[i], temp_fd, shadowbuf_p[i],
									sizeof(struct record)*(((off64_t)blocksize_stage1)*i+
											buffer_pos[i]+blocksize_stage2),
									sizeof(struct record)*rd_size);
							buffer_aio[i] = 1;
						} else {
							buffer_aio[i] = 2;
						}
					}
					if (buffer_off[i] == blocksize_stage2) {
						buffer_pos[i] += buffer_off[i];
						if (buffer_aio[i] == 1) {
							buffer_max[i] = async_read_pos_finalize(&buffer_hdl[i]) / sizeof(struct record);
							struct record *buffer_tmp = buffer_p[i];
							buffer_p[i] = shadowbuf_p[i];
							shadowbuf_p[i] = buffer_tmp;
							buffer_aio[i] = 0;
						} else
							buffer_max[i] = 0;
						buffer_off[i] = 0;
					}
					if (buffer_off[i] == buffer_max[i])
						continue;
					if (next_queue < 0 || compare_records(&buffer_p[i][buffer_off[i]],
							&buffer_p[next_queue][buffer_off[next_queue]]) < 0)
						next_queue = i;
				}

				if (next_queue < 0)
					break;

				if (wroff == blocksize_stage2) {
					sync_write(out_fd, wrbuf, sizeof(struct record)*blocksize_stage2);
					wroff = 0;
				}

				memcpy(&wrbuf[wroff], &buffer_p[next_queue][buffer_off[next_queue]], sizeof(struct record));
				buffer_off[next_queue]++;
				wroff++;

				if (++j >= blocksize_stage1) {
					fprintf(stderr, ".");
					j = 0;
				}
			}

			sync_write(out_fd, wrbuf, sizeof(struct record)*wroff);

			fprintf(stderr, "\n");

			for (i=0; i<queuenum; i++) {
				free(shadowbuf_p[i]);
				free(buffer_p[i]);
			}

			free(wrbuf);
		}
		c2 = times(&t2);
		fprintf(stderr, "    sync reads: %d\n", sync_read_count);
		fprintf(stderr, "   async reads: %d\n", async_read_count);
		fprintf(stderr, "   async waits: %d\n", async_wait_count);
		fprintf(stderr, "        writes: %d\n", sync_write_count);
		fprintf(stderr, "     time real: %.2f s\n", (float)(c2-c1)/sysconf(_SC_CLK_TCK));
		fprintf(stderr, "     time user: %.2f s\n", (float)(t2.tms_utime-t1.tms_utime)/sysconf(_SC_CLK_TCK));
		fprintf(stderr, "      time sys: %.2f s\n", (float)(t2.tms_stime-t1.tms_stime)/sysconf(_SC_CLK_TCK));

		close(in_fd);
		close(temp_fd);
		close(out_fd);

		unlink(argv[3]);
		return 0;
	}

	if ((argc == 3 || argc == 4) && !strcmp(argv[1], "query"))
	{
		off64_t file_size;

		if (argc == 3)
			file_size = mem_detect(argv[2], 0) * sizeof(struct record);
		else
			file_size = mem_detect(argv[2], atoi(argv[3])*(off64_t)(1024*1024)) * sizeof(struct record);

		fprintf(stderr, "Check memory usage: %.2f MB\n",
				sizeof(struct record)*(blocksize_check/(1024.0*1024.0)));
		fprintf(stderr, "Stage 1 block size: %.2f MB\n",
				sizeof(struct record)*blocksize_stage1/(1024.0*1024.0));
		fprintf(stderr, "Stage 2 block size: %.2f MB\n",
				sizeof(struct record)*blocksize_stage2/(1024.0*1024.0));
		fprintf(stderr, "Stage 1 memory usage: %.2f MB\n",
				sizeof(struct record)*(blocksize_stage1/(1024.0*1024.0))*2);
		fprintf(stderr, "Stage 2 memory usage: %.2f MB\n",
				(blocksize_stage2/(1024.0*1024.0))*((int)(file_size/blocksize_stage1)+2));
		return 0;
	}

	fprintf(stderr, "Usage: %s { check | sum } <in_file>\n", argv[0]);
	fprintf(stderr, "       %s sort <in_file> <tmp_file> <out_file> [ mem_size_mb ]\n", argv[0]);
	fprintf(stderr, "       %s query <in_file> [ mem_size_mb ]\n", argv[0]);
	return 1;
}

