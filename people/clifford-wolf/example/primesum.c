/* Primesum - Calculate the sum of all primes smaller than 4.000.000.000
 *
 * Copyright (C) 2003, 2007 Clifford Wolf
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#include <stdio.h>
#include <math.h>

// Overview of the algorithm:
//
// I'm using a Sieve of Eratosthenes to get the prime numbers. A
// big problem with this algorithm is that in its straigt implementation
// the entire prime-number-bitmap is processed for each found prime
// number. Since the prime number bitmap is by far to large to fit in
// the CPU caches the CPU just spends most of the time waiting for the
// memory bus.
//
// Fortunately, every new prime number found in a Sieve of Eratosthenes
// algorithm will not find any new non-primes smaller than it's square
// (because this non-primes must be the product of smaller primes found
// already earlier). So we can simply 'collect' a larger number of primes
// and then sieving all their multiples in one pass keeping the number of
// CPU cache misses low.
//
// This program has two configuration parameters which can be tuned to
// create a best-fit to the CPU cache sizes: GROUP_SIZE and PAGE_BLOCK.
// Note that performance will drop rapidly if this configuration parameters
// have to high values.
//
// The array holding the prime/non-prime flags (p) has only bits for the
// odd numbers since even number are never prime (besides '2' of course,
// which is handled explicitely in the program). This makes addressing
// the bits in the bitmap harder but reduces the size of the bitmap by
// a factor of two which reduces the memory-bus traffic and so incrases
// performance.
//
// For reference and comparison after changes in the source code:
// The sum of all primes < 4.000.000.000 is: 370.412.807.102.643.725
//
// There might be better ways of generating the sum of all prime numbers
// up to a limit but my focus was on the prime number generation.
// Unfortunately this program is so fast that actually printing out the
// primes would cost much more performance than generating them. So I
// decided to only output the sum of the primes to keep this performance
// overhead low and get a better impression of how fast the actual prime
// number generator works.
//
// This program is pretty well-documented. Usually I'm far less-verbose
// with my sourcecode comments, except there is a complicated algorithm
// in place which really needs some comments to be well understood. Some
// time ago I've created a page on my website where I've tried to explain
// my coding-style philosophy: http://www.clifford.at/style.html

#define DEBUG 0

#define MAX_PRIMES 4000000000U

// Set this to one to enable the x86 assembler code. As usual, the compiler
// will generate much better code than this handwritten stuff. Left in s
// reference if someone else feels like optimizing the main loop in assembler
// some time in the future..
#define X86ASM 0

// number of pages (4k blocks) to processes in one pass. this number of pages
// and the group_c, group_d variables (together with the acutal program code)
// must fit into the CPU cache. This must be a power of two.
#define PAGE_BLOCK 32

// 1 page = 4096 bytes = 4096*8 prime-flags
// 4096*8 = 32768 = 0x8000
#define PAGE_SIZE (0x8000*PAGE_BLOCK)
#define PAGE_MASK (~(PAGE_SIZE-1))

// The actual sieving bitmap. There are only bits for the odd numbers here
// and we add one page padding so we can use less-aggressive (and faster)
// termination conditions in the loops below.
//
//	numerical_value = 2 * bit_position_in_p + 1
//
unsigned char p[MAX_PRIMES/16+PAGE_SIZE+1] __attribute__ ((aligned (4096)));

// GROUP_SIZE is the number of primes we process in parallel. We need
// to mark all multiples of this primes as non-primes. So we have a
// counter here holding the current multiple of the prime number
// (initialized to the square of the prime) and increment deltas (set to
// the double of the prime since we are not interested in the even
// multiples anyway). Both values are 'encoded' to apply to bit positions
// in p[] instead of the numerical values.
//
// Obviously the bit-position delta in group_d[] also is the numeric
// value of the original prime. Don't let yourself get confused by that.
#define GROUP_SIZE 8192
unsigned int group_c[GROUP_SIZE] __attribute__ ((aligned (4096))); // counters
unsigned int group_d[GROUP_SIZE] __attribute__ ((aligned (4096))); // deltas

int main()
{
	// the current bit position. we start with a=1 because the first
	// prime number we are interested in is 3
	unsigned int a=1;

	// maxp_roof is the highest bit-number p[bit] we are interested in
	unsigned int maxp_roof = MAX_PRIMES/2;

	// maxp_sqrt is the highest bit number for collecting primes for sieving
	// higher non-primes. when we have reached this limit the remaining primes
	// up to maxp_roof are already sieved.
	unsigned int maxp_sqrt = sqrt(MAX_PRIMES)/2;

	// the sum of the primes. initialized to 2 because we do not generate this
	// number in the actual algorithm.
	unsigned long long primesum = 2;

#if DEBUG
	printf("size of p[] = %.3f MB\n", sizeof(p) / (1024*1024.));
	printf("size of a page = %.3f kB\n", PAGE_SIZE / (8*1024.));
#endif

	// main loop: do the sieving and add up all primes smaller sqrt(MAX_PRIMES)
	while (a<maxp_sqrt)
	{
		// especially for the first few iterations we can't fill the group_* arrays.
		// this variable contains the number of valid entries in group_c and group_d.
		unsigned int group_roof = 0;

		unsigned int current_page = ~0;

		// Yep - this is an evil goto label! It is used to continue filling the group_*
		// variables when there is still free space in group_* and new pages have been
		// processed.
continue_filling_group_arrays:;

		// the maximum p[] bit position for which we have complete prime information
		// we can safely aquire primes up to this bit position for this group. This
		// is recalculated whenever continue_filling_group_arrays is jumped to.
		unsigned int group_window = 2*a*a+2*a;
		if (group_window > current_page)
			group_window = current_page;
		if (group_window > maxp_sqrt)
			group_window = maxp_sqrt;

		// increment a and look for new prime numbers (unset bits in p[]) smaller than 
		// the current group_window value and not more than GROUP_SIZE. Also add all
		// found primes to primesum and set current_page to the smallest group_c[]
		// entry found.
		for (; a < group_window && group_roof < GROUP_SIZE; a++) {
			if ( p[a>>3] & (1<<(a&7)) ) continue;
			primesum += 2*a+1;
			group_d[group_roof] = 2*a+1;
			group_c[group_roof] = 2*a*a+2*a;
			if (current_page > (group_c[0] & PAGE_MASK))
				current_page = group_c[0] & PAGE_MASK;
			group_roof++;
		}

#if DEBUG
		printf("num:%d window_size:%d current_page:%d\n", group_roof, group_window, current_page);
#endif

		// count up the group_c[] variables 'parallel' and mark all the found non-primes
		// by setting their bit in p[]. The 'parallel' increment is done by incrementing
		// the one group_c[] element after another until they exceed the current page.
		// After that the current_page variable is set to the page with the smallest
		// group_c[] value pointing to and group_c[] is incremented again until the end
		// of p[] is reached. Filling group_* is continued if there is free space and
		// we haven't yet reached maxp_sqrt.
		while ( current_page <= (maxp_roof & PAGE_MASK) )
		{
			unsigned int b, g;
			unsigned int next_page = (maxp_roof & PAGE_MASK) + PAGE_SIZE;
			for (g=0; g<group_roof; g++)
			{
#if !X86ASM
				for (b=group_c[g]; b < (current_page + PAGE_SIZE); b+=group_d[g])
					p[b>>3] |= (1<<(b&7));
#else
				b = group_c[g];
				__asm__ (
					"	jmp	.Lx2			\n"
					".p2align 4				\n"
					".Lx0:					\n"
					"	btsl	%[gc],	%[pp]		\n"
					"	addl	%[gd],	%[gc]		\n"
					".Lx2:					\n"
					"	cmpl	%[cp],	%[gc]		\n"
					"	jl	.Lx0			\n"
					: /* outputs  */
						[gc] "+r" (b),
						[pp] "+o" (p)
					: /* inputs   */
						[gd] "r" (group_d[g]),
						[cp] "r" (current_page + PAGE_SIZE)
					: /* clobbers */
						"memory"
				);
#endif
				if ( b < next_page )
					next_page = b & PAGE_MASK;
				group_c[g]=b;
			}
			current_page = next_page;

			if (group_roof < GROUP_SIZE && group_window < maxp_sqrt)
				goto continue_filling_group_arrays;
		}

#if DEBUG
		printf("finished main loop pass\n");
#endif
	}

#if DEBUG
	printf("Now make a scan over the remainging numbers\n");
#endif

	// add up the remaining primes larger or equal sqrt(MAX_PRIMES)
	for (; a<maxp_roof; a++)
		if ( !(p[a>>3] & (1<<(a&7))) )
			primesum += 2*a+1;

	printf("%Ld\n", primesum);

	return 0;
}

