#include <stdio.h>
#include <unistd.h>

#define ONE_MB (1024 * 1024)

int main (void)
{
	long num_procs;
	long procs_online;
	long page_size;
	long num_pages;
	long free_pages;
	longlong_t mem;
	longlong_t free_mem;

	num_procs = sysconf (_SC_NPROCESSORS_CONF);
	procs_online = sysconf (_SC_NPROCESSORS_ONLN);
	page_size = sysconf (_SC_PAGESIZE);
	num_pages = sysconf (_SC_PHYS_PAGES);
	free_pages = sysconf (_SC_AVPHYS_PAGES);

	mem = (longlong_t) ((longlong_t) num_pages * (longlong_t) page_size);
	mem /= ONE_MB;
	free_mem = (longlong_t) free_pages * (longlong_t) page_size;
	free_mem /= ONE_MB;

	printf ("%ld CPU%s installed, %ld online\n", num_procs,
		(num_procs > 1) ? "s" : "", procs_online);
	printf ("%lld MB physical memory, %lld MB free\n", mem, free_mem);

	return (0);
}
