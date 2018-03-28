/*
 *  Read/write directly to I/O memory using /dev/mem.
 *
 *  Copyright (C) 2007  RIEGL Research ForschungsGmbH
 *  Copyright (C) 2007  Clifford Wolf <clifford@clifford.at>
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
 *  WARINING: This tool assumes that a 'short' is 16 bits
 *  and 'long' is 32 bits long. Usually this is correct but
 *  the word sizes are not defined by any C-Standard document!
 */

#define _LARGEFILE64_SOURCE

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <endian.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <readline/readline.h>
#include <readline/history.h>

void help()
{
	fprintf(stderr, "\n");
	fprintf(stderr, "Usage: iomem <mode>[-port][<:|*>[<length>]][?|!] \\\n");
	fprintf(stderr, "		<addr> [ <data>[/<mask>] [...] ]\n");
	fprintf(stderr, "\n");
	fprintf(stderr, "Read modes:\n");
	fprintf(stderr, "	r8\n");
	fprintf(stderr, "	r16 r16le r16be\n");
	fprintf(stderr, "	r32 r32le r32be\n");
	fprintf(stderr, "\n");
	fprintf(stderr, "Write modes:\n");
	fprintf(stderr, "	w8\n");
	fprintf(stderr, "	w16 w16le w16be\n");
	fprintf(stderr, "	w32 w32le w32be\n");
	fprintf(stderr, "\n");
	fprintf(stderr, "Poll modes:\n");
	fprintf(stderr, "	p8\n");
	fprintf(stderr, "	p16 p16le p16be\n");
	fprintf(stderr, "	p32 p32le p32be\n");
	fprintf(stderr, "\n");
	fprintf(stderr, "Shell modes:\n");
	fprintf(stderr, "	s8\n");
	fprintf(stderr, "	s16 s16le s16be\n");
	fprintf(stderr, "	s32 s32le s32be\n");
	fprintf(stderr, "\n");
	fprintf(stderr, "Adding the '-port' suffix to the mode causes iomem to\n");
	fprintf(stderr, "access i/o ports instead of physical memory addresses.\n");
	fprintf(stderr, "\n");
	fprintf(stderr, "Length with a ':' seperator refers to a linear memory\n");
	fprintf(stderr, "while length with a '*' seperator refers to a FIFO port.\n");
	fprintf(stderr, "\n");
	fprintf(stderr, "The '?' modifier makes the output less verbose so it can\n");
	fprintf(stderr, "be parsed easier from a shell script.\n");
	fprintf(stderr, "\n");
	fprintf(stderr, "The '!' modifier supresses all output, example given for\n");
	fprintf(stderr, "i/o performance tests.\n");
	fprintf(stderr, "\n");
	fprintf(stderr, "The poll modes are polling the specified io memory word\n");
	fprintf(stderr, "until it matches any of the data/mask pairs passed on the\n");
	fprintf(stderr, "command line.\n");
	fprintf(stderr, "\n");
	fprintf(stderr, "The shell mode open up a simple command line interface.\n");
	fprintf(stderr, "Commands can be just a memory address to perform a single\n");
	fprintf(stderr, "word read or an <addr>=<data> pair to perform a single word\n");
	fprintf(stderr, "write. All addresses are relative to the address passed as\n");
	fprintf(stderr, "command line argument.\n");
	fprintf(stderr, "\n");
	exit(1);
}

int get_this_endian()
{
	int probe = 1;
	return ((unsigned char*)(&probe))[0] ? 'l' : 'b';
}

unsigned long endian_swap(unsigned long v, int size)
{
	if (size >= 4)
		v = ((v&0x0000ffff) << 16) | ((v&0xffff0000) >> 16);
	if (size >= 2)
		v = ((v&0x00ff00ff) <<  8) | ((v&0xff00ff00) >>  8);
	return v;
}

int main(int argc, char **argv)
{
	int host_endian = get_this_endian();
	int endian = 0, size = 0, length = 1, we = 0, pe = 0, se = 0;
	unsigned long addr = 0, data = 0, mask = 0, waitcycles = 0;
	unsigned long *databuf = 0;
	unsigned long *maskbuf = 0;
	int fifo_mode = 0, i, j;
	int port_mode = 0;
	int verbose = 1;
	int perftest = 0;

	/* a 'short' must be 16 bits wide */
	if (sizeof(unsigned short) != 2)
		abort();

	/* a 'long' must be 32 bits wide */
	if (sizeof(unsigned long) != 4)
		abort();

	/* expect at least a mode and an address */
	if (argc < 3)
		help();

	/* parse mode and set 'we', 'size' and 'endian' */
	{
		char *mode = argv[1];

		if (mode[0] != 'r' && mode[0] != 'w' && mode[0] != 'p' && mode[0] != 's')
			help();

		if (mode[0] == 'w')
			we = 1;

		if (mode[0] == 'p')
			pe = 1;

		if (mode[0] == 's')
			se = 1;

		mode++;

		if (!strncmp(mode, "8", 1)) {
			mode++;
			size=1;
		} else
		if (!strncmp(mode, "16", 2)) {
			mode+=2;
			size=2;
		} else
		if (!strncmp(mode, "32", 2)) {
			mode+=2;
			size=4;
		} else
			help();

		if (size > 1) {
			if (!strncmp(mode, "le", 2)) {
				mode+=2;
				endian='l';
			} else
			if (!strncmp(mode, "be", 2)) {
				mode+=2;
				endian='b';
			}
		}

		if (!strncmp(mode, "-port", 5)) {
			port_mode = 1;
			mode += 5;
		}

		if (mode[0] == ':' || mode[0] == '*') {
			if (pe || se)
				help();
			if (mode[0] == '*')
				fifo_mode = 1;
			mode++;
			if (*mode)
				length = strtoul(mode, &mode, 0);
			else
			if (we || pe)
				length = argc-3;
			if (!length)
				help();
		} else
			if (we || pe)
				length = argc-3;

		if (*mode == '?') {
			verbose = 0;
			mode++;
		}

		if (*mode == '!') {
			perftest = 1;
			mode++;
		}

		if (*mode)
			help();
	}
	
	/* read address and data */
	{
		char *endptr;

		addr = strtoul(argv[2], &endptr, 0);
		if (*endptr || !*argv[2])
			help();

		if (we || pe)
		{
			int dargs = argc-3;

			if (dargs <= 0 || dargs > length)
				help();

			databuf = malloc(sizeof(unsigned long) * length);
			maskbuf = malloc(sizeof(unsigned long) * length);

			for (i=0; i<length; i++) {
				databuf[i] = strtoul(argv[3+i%dargs], &endptr, 0);
				if (*endptr == '/')
					maskbuf[i] = strtoul(endptr+1, &endptr, 0);
				else
					maskbuf[i] = ~0;
				if (*endptr || !*argv[3+i%dargs])
					help();
			}
		}
		else {
			if (argc != 3)
				help();
		}
	}

	/* open /dev/mem device file */
	int fd = open(port_mode ? "/dev/port" : "/dev/mem", we || se ? O_RDWR : O_RDONLY);
	if (fd < 0) {
		fprintf(stderr, "Can't open %s: %s\n", port_mode ? "/dev/port" : "/dev/mem", strerror(errno));
		exit(1);
	}

	/* hack for shell mode */
	unsigned long se_base_addr = addr;
	if (se) {
		static unsigned long shell_databuf, shell_maskbuf;
		databuf = &shell_databuf;
		maskbuf = &shell_maskbuf;
next_shell_command:;
		char *line;
		if (0) {
shell_syntax_error:
			free(line);
			fprintf(stderr, "Syntax error!\n");
		}
		line = readline("> ");
		if (!line)
			goto shell_quit;
		char *addr_str = strtok(line, "=");
		char *data_str = strtok(NULL, "");
		char *endptr;
		if (!addr_str || !addr_str[0])
			goto shell_syntax_error;
		addr = se_base_addr + strtoul(addr_str, &endptr, 0);
		if (*endptr)
			goto shell_syntax_error;
		if (data_str) {
			if (!data_str[0])
				goto shell_syntax_error;
			we = 1;
			databuf[0] = strtoul(data_str, &endptr, 0);
			maskbuf[0] = ~0;
			if (*endptr)
				goto shell_syntax_error;
		} else {
			we = 0;
		}
		free(line);
	}

	/* calculate offsets to page and within page */
	unsigned long psize = getpagesize();
	unsigned long off_inpage = addr % psize;
	unsigned long off_topage = addr - off_inpage;
	unsigned long mapsize = off_inpage+length*size;

	/* map it into logical memory */
	void *mapping = 0;

	if (!port_mode) {
		mapping = mmap(0, mapsize, we ? PROT_WRITE : PROT_READ, MAP_SHARED, fd, off_topage);
		if (mapping == MAP_FAILED) {
			fprintf(stderr, "Can't map physical memory: %s\n", strerror(errno));
			exit(1);
		}
	}

	/* main loop */
	for (i=0; i<(pe || se ? 1 : length); i++)
	{
poll_retry:
		/* data word from buffer */
		data = we ? databuf[i] : 0;
		mask = we ? maskbuf[i] : 0;

		/* endian swapping */
		if (endian && host_endian != endian) {
			data = endian_swap(data, size);
			mask = endian_swap(mask, size);
		}

		/* do the read/write */
		if (port_mode)
		{
			if (size == 1) {
				unsigned char data8 = 0;
				if (~mask) {
					lseek64(fd, (off64_t)addr, SEEK_SET);
					read(fd, &data8, 1);
				}
				if (we) {
					data8 = (data8 & ~mask) | (data & mask);
					lseek64(fd, (off64_t)addr, SEEK_SET);
					write(fd, &data8, 1);
				}
				data = data8;
			} else
			if (size == 2) {
				unsigned short data16 = 0;
				if (~mask) {
					lseek64(fd, (off64_t)addr, SEEK_SET);
					read(fd, &data16, 2);
				}
				if (we) {
					data16 = (data16 & ~mask) | (data & mask);
					lseek64(fd, (off64_t)addr, SEEK_SET);
					write(fd, &data16, 2);
				}
				data = data16;
			} else
			if (size == 4) {
				unsigned long data32 = 0;
				if (~mask) {
					lseek64(fd, (off64_t)addr, SEEK_SET);
					read(fd, &data32, 4);
				}
				if (we) {
					data32 = (data32 & ~mask) | (data & mask);
					lseek64(fd, (off64_t)addr, SEEK_SET);
					write(fd, &data32, 4);
				}
				data = data32;
			} else
				abort();
		}
		else
		{
			if (size == 1) {
				unsigned char *mapping8 = mapping + off_inpage;
				if (we)
					*mapping8 = mask == ~0U ? data :
							(data = (*mapping8 & ~mask) | (data & mask));
				else
					data = *mapping8;
			} else
			if (size == 2) {
				unsigned short *mapping16 = mapping + off_inpage;
				if (we)
					*mapping16 = mask == ~0U ? data :
							(data = (*mapping16 & ~mask) | (data & mask));
				else
					data = *mapping16;
			} else
			if (size == 4) {
				unsigned long *mapping32 = mapping + off_inpage;
				if (we)
					*mapping32 = mask == ~0U ? data :
							(data = (*mapping32 & ~mask) | (data & mask));
				else
					data = *mapping32;
			} else
				abort();
		}

		/* endian swapping */
		if (endian && host_endian != endian) {
			data = endian_swap(data, size);
			mask = endian_swap(mask, size);
		}

		/* polling checks */
		if (pe)
		{
			for (j=0; j<length; j++) {
				if ((data & maskbuf[j]) == (databuf[j] & maskbuf[j]))
					goto poll_found_match;
			}
			usleep(25000);
			waitcycles++;
			goto poll_retry;
poll_found_match:;
		}

		/* print read/written data */
		if (!perftest)
		{
			if (verbose)
			{
				if (size == 1 && data >= 32 && data <= 126)
					printf("[%04x] 0x%08lx: 0x%0*lx%s (%s) [ASCII=%c]", i, addr, size*2, data, endian ? endian == 'l' ? " LE" : " BE" : "", we ? "written" : "read", (char)data);
				else
					printf("[%04x] 0x%08lx: 0x%0*lx%s (%s)", i, addr, size*2, data, endian ? endian == 'l' ? " LE" : " BE" : "", we ? "written" : "read");

				if (pe)
					printf(" <after %ldms>\n", waitcycles*25);
				else
					printf("\n");
			}
			else
				printf("0x%0*lx\n", size*2, data);
		}

		/* increment address if we aren't in fifo mode*/
		if (!fifo_mode) {
			off_inpage+=size;
			addr+=size;
		}
	}

	/* munmap, close and sync back */
	if (!port_mode)
		munmap(mapping, mapsize);
	if (se)
		goto next_shell_command;
shell_quit:
	close(fd);

	/* bye, bye */
	return 0;
}

