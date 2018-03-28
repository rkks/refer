/*
 *  RPN Calculator with arithmetric and bitfield operations
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
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef RPN64

typedef unsigned long long data_t;
#define PRINTF_64 "ll"
#define HEX_DIGITS "16"
#define BIT(num) (1ll<<(num))
#define SEPLINEBITVIEW "+---------+---------+---------+---------+---------+---------+---------+---------+"
#define OCTETS 8
#define MSB 63

#define PRINTF_ZEROS_X "016"
#define PRINTF_ZEROS_D "020"
#define PRINTF_ZEROS_O "022"

#else

typedef unsigned int data_t;
#define PRINTF_64 ""
#define HEX_DIGITS "8"
#define BIT(num) (1<<(num))
#define SEPLINEBITVIEW "+---------+---------+---------+---------+"
#define OCTETS 4
#define MSB 31

#define PRINTF_ZEROS_X "08"
#define PRINTF_ZEROS_D "010"
#define PRINTF_ZEROS_O "011"

#endif

#define MAX_STACK 1024

static data_t stack[MAX_STACK];
static data_t stackp;

static char *cmd_name;

static void help(void)
{
	fprintf(stderr, "\n");
	fprintf(stderr, "Usage: %s [-x|-d|-o|-b|-i] [-z] <RPN-TOKENS>\n", cmd_name);
	fprintf(stderr, "\n");
	fprintf(stderr, "The default output format is a nice multi-base chart.\n");
	fprintf(stderr, "\n");
	fprintf(stderr, "The -x, -d, -o, -b and -i options may be used to change this\n");
	fprintf(stderr, "to simple hexadecimal, decimal, octal, binary or IP-address output.\n");
	fprintf(stderr, "\n");
	fprintf(stderr, "The -z option adds leading zeros to the output format (use with\n");
	fprintf(stderr, "the -x, -d, -o, -b or -i option).\n");
	fprintf(stderr, "\n");
	fprintf(stderr, "<RPN-TOKENS> are constants or operators.\n");
	fprintf(stderr, "\n");
	fprintf(stderr, "Constants:\n");
	fprintf(stderr, "\n");
	fprintf(stderr, "	0d1234  decimal number\n");
	fprintf(stderr, "	0x09ab  hexadecimal number\n");
	fprintf(stderr, "	0o7777  octal number\n");
	fprintf(stderr, "	0b1001  binary number\n");
	fprintf(stderr, "\n");
	fprintf(stderr, "	12345  decimal number\n");
	fprintf(stderr, "	01234  octal number\n");
	fprintf(stderr, "\n");
	fprintf(stderr, "	Underscores in constants are ignored.\n");
	fprintf(stderr, "\n");
	fprintf(stderr, "	Binary, octal and hexadecimal numbers may contain '..' to\n");
	fprintf(stderr, "	fill the middle bits with zeros. (e.g.: 0xe..18f0)\n");
	fprintf(stderr, "\n");
	fprintf(stderr, "	IP-Adresses may be passed as A.B.C.D (A, B, C and D beeing\n");
	fprintf(stderr, "	decimal numbers in the range from 0 to 255).\n");
	fprintf(stderr, "\n");
	fprintf(stderr, "Constant Prefixes:\n");
	fprintf(stderr, "\n");
	fprintf(stderr, "	- ~\n");
	fprintf(stderr, "\n");
	fprintf(stderr, "Constant suffixes:\n");
	fprintf(stderr, "\n");
	fprintf(stderr, "	G  2^30     g  10^9\n");
	fprintf(stderr, "	M  2^20     m  10^6\n");
	fprintf(stderr, "	K  2^10     k  10^3\n");
	fprintf(stderr, "\n");
	fprintf(stderr, "Operations with two operands:\n");
	fprintf(stderr, "\n");
	fprintf(stderr, "	add sub mul div mod\n");
	fprintf(stderr, "	and or nand nor xor xnor\n");
	fprintf(stderr, "	shl shr\n");
	fprintf(stderr, "\n");
	fprintf(stderr, "Operations with one operand:\n");
	fprintf(stderr, "\n");
	fprintf(stderr, "	neg not\n");
#ifdef RPN64
	fprintf(stderr, "	swap swap16 swap32 swap64\n");
#else
	fprintf(stderr, "	swap swap16 swap32\n");
#endif
	fprintf(stderr, "	ffs clz ctz pc\n");
	fprintf(stderr, "\n");
	fprintf(stderr, "Pop-All Operations:\n");
	fprintf(stderr, "\n");
	fprintf(stderr, "	all_add all_mul\n");
	fprintf(stderr, "	all_or all_and all_xor\n");
	fprintf(stderr, "\n");
	fprintf(stderr, "Comment tokens:\n");
	fprintf(stderr, "\n");
	fprintf(stderr, "	//    ignore next token\n");
	fprintf(stderr, "	//*   ignore this token\n");
	fprintf(stderr, "\n");
	fprintf(stderr, "Non-RPN Infix operators (strict left-to-right):\n");
	fprintf(stderr, "\n");
	fprintf(stderr, "	+ - * / %%\n");
	fprintf(stderr, "	& | ^ << >>\n");
	fprintf(stderr, "\n");
	fprintf(stderr, "Quick introduction on RPN:\n");
	fprintf(stderr, "http://en.wikipedia.org/wiki/Reverse_Polish_notation\n");
	fprintf(stderr, "\n");
	fprintf(stderr, "A little warning:\n");
	fprintf(stderr, "This tool is limited to %d bit unsigned integers.\n", MSB+1);
	fprintf(stderr, "\n");
	exit(1);
}

static void push(data_t val)
{
	if (stackp >= MAX_STACK) {
		fprintf(stderr, "Stack overrun!\n");
		exit(2);
	}
	stack[stackp++] = val;
}

static data_t pop(void)
{
	if (stackp <= 0) {
		fprintf(stderr, "Stack underrun!\n");
		exit(3);
	}
	return stack[--stackp];
}

static data_t swap(data_t v, int len)
{
#ifdef RPN64
	if (len >= 64)
		v = ((v&0x00000000ffffffffll) << 32) | ((v&0xffffffff00000000ll) >> 32);
	if (len >= 32)
		v = ((v&0x0000ffff0000ffffll) << 16) | ((v&0xffff0000ffff0000ll) >> 16);
	if (len >= 16)
		v = ((v&0x00ff00ff00ff00ffll) <<  8) | ((v&0xff00ff00ff00ff00ll) >>  8);
#else
	if (len >= 32)
		v = ((v&0x0000ffff) << 16) | ((v&0xffff0000) >> 16);
	if (len >= 16)
		v = ((v&0x00ff00ff) <<  8) | ((v&0xff00ff00) >>  8);
#endif
	return v;
}

static data_t my_strtoul(const char *nptr, char **endptr, int base)
{
	data_t ret = 0;
	data_t ret_high = 0;
	int bits_per_digit = 0;
	int bitc = 0;

	if (base == 2)
		bits_per_digit = 1;
	else
	if (base == 8)
		bits_per_digit = 3;
	else
	if (base == 16)
		bits_per_digit = 4;

	while ((*nptr == '_') ||
			(nptr[0] == '.' && nptr[1] == '.') ||
			(*nptr >= '0' && *nptr <= '9') ||
			(*nptr >= 'a' && *nptr <= 'z') ||
			(*nptr >= 'A' && *nptr <= 'Z'))
	{
		data_t digit = 0;
		if (*nptr == '_') {
			nptr++;
			continue;
		}
		if (*nptr == '.') {
			if (!bits_per_digit)
				break;
			nptr += 2;
			while (bitc <= MSB) {
				ret = ret * base;
				bitc += bits_per_digit;
			}
			ret_high = ret;
			ret = bitc = 0;
			continue;
		}
		if (*nptr >= '0' && *nptr <= '9')
			digit = *nptr - '0';
		if (*nptr >= 'a' && *nptr <= 'z')
			digit = *nptr - 'a' + 10;
		if (*nptr >= 'A' && *nptr <= 'Z')
			digit = *nptr - 'A' + 10;
		if (digit >= (data_t)base)
			break;
		ret = ret * base + digit;
		bitc += bits_per_digit;
		nptr++;
	}

	if (endptr)
		*endptr = (char*)nptr;

	return ret_high | ret;
}

int main(int argc, char **argv)
{
	char out_fmt = 0;
	int out_zeros = 0;
	char sched_infix = 0;
	char const_mod = 0;
	int i;

	cmd_name = argv[0];

	if (argc <= 1)
		help();

	for (i=1; i<argc; i++)
	{
		char *a = argv[i];
		char *e = 0;

		/* constants */

		const_mod = 0;
		if (a[0] == '-' && a[1] >= '0' && a[1] <= '9') {
			const_mod = '-';
			a++;
		} else
		if (a[0] == '~' && a[1] >= '0' && a[1] <= '9') {
			const_mod = '~';
			a++;
		}

		unsigned int ip_a, ip_b, ip_c, ip_d;
		if (sscanf(a, "%d.%d.%d.%d", &ip_a, &ip_b, &ip_c, &ip_d) == 4) {
			push((ip_a << 24) | (ip_b << 16) | (ip_c << 8) | (ip_d << 0));
			goto handle_const_mod;
		}

		if (!strncmp(a, "0d", 2)) {
			if (!a[2])
				help();
			push(my_strtoul(a+2, &e, 10));
			goto parse_const_suffix;
		}
		if (!strncmp(a, "0x", 2)) {
			if (!a[2])
				help();
			push(my_strtoul(a+2, &e, 16));
			goto parse_const_suffix;
		}
		if (!strncmp(a, "0o", 2)) {
			if (!a[2])
				help();
			push(my_strtoul(a+2, &e, 8));
			goto parse_const_suffix;
		}
		if (!strncmp(a, "0b", 2)) {
			if (!a[2])
				help();
			push(my_strtoul(a+2, &e, 2));
			goto parse_const_suffix;
		}
		if (*a == '0') {
			push(my_strtoul(a+1, &e, 8));
			goto parse_const_suffix;
		}
		if (*a >= '1' && *a <= '9') {
			push(my_strtoul(a, &e, 10));
			goto parse_const_suffix;
		}

		/* constant pre- and suffixes, handle scheduled infix ops */

		if (0) {
parse_const_suffix:
			if (!e[0])
				goto handle_const_mod;
			if (e[1])
				help();

			switch (e[0])
			{
			case 'G':
				push(pop()*1024);
			case 'M':
				push(pop()*1024);
			case 'K':
				push(pop()*1024);
				break;
			case 'g':
				push(pop()*1000);
			case 'm':
				push(pop()*1000);
			case 'k':
				push(pop()*1000);
				break;
			default:
				help();
			}

handle_const_mod:
			if (const_mod == '-')
				push(-pop());
			if (const_mod == '~')
				push(~pop());

			switch (sched_infix)
			{
			case '+':
				sched_infix = 0;
				goto process_add;
			case '-':
				sched_infix = 0;
				goto process_sub;
			case '*':
				sched_infix = 0;
				goto process_mul;
			case '/':
				sched_infix = 0;
				goto process_div;
			case '%':
				sched_infix = 0;
				goto process_mod;
			case '&':
				sched_infix = 0;
				goto process_and;
			case '|':
				sched_infix = 0;
				goto process_or;
			case '^':
				sched_infix = 0;
				goto process_xor;
			case '<':
				sched_infix = 0;
				goto process_shl;
			case '>':
				sched_infix = 0;
				goto process_shr;
			case 0:
				break;
			default:
				abort();
			}

			continue;
		}

		/* check and schedule infix operations */

		if (sched_infix)
			help();

		if (!strcmp(a, "+") || !strcmp(a, "-") || !strcmp(a, "*") ||
				!strcmp(a, "/") || !strcmp(a, "%") ||
				!strcmp(a, "&") || !strcmp(a, "|") ||
				!strcmp(a, "^") || !strcmp(a, "<<") ||
				!strcmp(a, ">>")) {
			sched_infix = a[0];
			continue;
		}

		/* binary operations */

		if (!strcmp(a, "add")) {
process_add:;
			data_t op2 = pop();
			data_t op1 = pop();
			push(op1 + op2);
			continue;
		}
		if (!strcmp(a, "sub")) {
process_sub:;
			data_t op2 = pop();
			data_t op1 = pop();
			push(op1 - op2);
			continue;
		}
		if (!strcmp(a, "mul")) {
process_mul:;
			data_t op2 = pop();
			data_t op1 = pop();
			push(op1 * op2);
			continue;
		}
		if (!strcmp(a, "div")) {
process_div:;
			data_t op2 = pop();
			data_t op1 = pop();
			push(op1 / op2);
			continue;
		}
		if (!strcmp(a, "mod")) {
process_mod:;
			data_t op2 = pop();
			data_t op1 = pop();
			push(op1 % op2);
			continue;
		}
		if (!strcmp(a, "and")) {
process_and:;
			data_t op2 = pop();
			data_t op1 = pop();
			push(op1 & op2);
			continue;
		}
		if (!strcmp(a, "or")) {
process_or:;
			data_t op2 = pop();
			data_t op1 = pop();
			push(op1 | op2);
			continue;
		}
		if (!strcmp(a, "nand")) {
			data_t op2 = pop();
			data_t op1 = pop();
			push(~(op1 & op2));
			continue;
		}
		if (!strcmp(a, "nor")) {
			data_t op2 = pop();
			data_t op1 = pop();
			push(~(op1 | op2));
			continue;
		}
		if (!strcmp(a, "xor")) {
process_xor:;
			data_t op2 = pop();
			data_t op1 = pop();
			push(op1 ^ op2);
			continue;
		}
		if (!strcmp(a, "xnor")) {
			data_t op2 = pop();
			data_t op1 = pop();
			push(~(op1 ^ op2));
			continue;
		}
		if (!strcmp(a, "shl")) {
process_shl:;
			data_t op2 = pop();
			data_t op1 = pop();
			push(op1 << op2);
			continue;
		}
		if (!strcmp(a, "shr")) {
process_shr:;
			data_t op2 = pop();
			data_t op1 = pop();
			push(op1 >> op2);
			continue;
		}

		/* unary operations */

		if (!strcmp(a, "neg")) {
			push(-pop());
			continue;
		}
		if (!strcmp(a, "not")) {
			push(~pop());
			continue;
		}
		if (!strcmp(a, "swap")) {
			push(swap(pop(), 64));
			continue;
		}
#ifdef RPN64
		if (!strcmp(a, "swap64")) {
			push(swap(pop(), 64));
			continue;
		}
#endif
		if (!strcmp(a, "swap32")) {
			push(swap(pop(), 32));
			continue;
		}
		if (!strcmp(a, "swap16")) {
			push(swap(pop(), 16));
			continue;
		}
		if (!strcmp(a, "ffs")) {
			data_t op = pop();
#ifdef RPN64
			push(__builtin_ffsll(op));
#else
			push(__builtin_ffs(op));
#endif
			continue;
		}
		if (!strcmp(a, "clz")) {
			data_t op = pop();
#ifdef RPN64
			push(__builtin_clzll(op));
#else
			push(__builtin_clz(op));
#endif
			continue;
		}
		if (!strcmp(a, "ctz")) {
			data_t op = pop();
#ifdef RPN64
			push(__builtin_ctzll(op));
#else
			push(__builtin_ctz(op));
#endif
			continue;
		}
		if (!strcmp(a, "pc")) {
			data_t op = pop();
#ifdef RPN64
			push(__builtin_popcountll(op));
#else
			push(__builtin_popcount(op));
#endif
			continue;
		}

		/* pop-all operations */

		if (!strcmp(a, "all_add")) {
			unsigned res = 0;
			while (stackp > 0)
				res += pop();
			push(res);
			continue;
		}
		if (!strcmp(a, "all_mul")) {
			unsigned res = 1;
			while (stackp > 0)
				res *= pop();
			push(res);
			continue;
		}
		if (!strcmp(a, "all_or")) {
			unsigned res = 0;
			while (stackp > 0)
				res |= pop();
			push(res);
			continue;
		}
		if (!strcmp(a, "all_and")) {
			unsigned res = 0;
			while (stackp > 0)
				res &= pop();
			push(res);
			continue;
		}
		if (!strcmp(a, "all_xor")) {
			unsigned res = 0;
			while (stackp > 0)
				res ^= pop();
			push(res);
			continue;
		}

		/* comment tokens */

		if (!strncmp(a, "//", 2)) {
			if (!a[2])
				i++;
			continue;
		}

		/* options */

		if (!strcmp(a, "-x")) {
			out_fmt = 'x';
			continue;
		}

		if (!strcmp(a, "-d")) {
			out_fmt = 'd';
			continue;
		}

		if (!strcmp(a, "-o")) {
			out_fmt = 'o';
			continue;
		}

		if (!strcmp(a, "-b")) {
			out_fmt = 'b';
			continue;
		}

		if (!strcmp(a, "-i")) {
			out_fmt = 'i';
			continue;
		}

		if (!strcmp(a, "-z")) {
			out_zeros = 1;
			continue;
		}

		help();
	}

	data_t result = pop();

	if (stackp != 0) {
		fprintf(stderr, "Stack unbalanced!\n");
		exit(4);
	}

	switch (out_fmt)
	{
	case 'x':
		if (out_zeros)
			printf("0x%" PRINTF_ZEROS_X PRINTF_64 "x\n", result);
		else
			printf("0x%" PRINTF_64 "x\n", result);
		break;
	case 'd':
		if (out_zeros)
			printf("%" PRINTF_ZEROS_D PRINTF_64 "u\n", result);
		else
			printf("%" PRINTF_64 "u\n", result);
		break;
	case 'o':
		if (out_zeros)
			printf("0%" PRINTF_ZEROS_O PRINTF_64 "o\n", result);
		else
			printf("0%" PRINTF_64 "o\n", result);
		break;
	case 'b':
		for (i=MSB; i>0 && !out_zeros; i--) {
			if (result & BIT(i))
				break;
		}
		for (; i>=0; i--)
			printf("%c", result & BIT(i) ? '1' : '0');
		printf("\n");
		break;
	case 'i':
		if (out_zeros)
			printf("%03" PRINTF_64 "u.%03" PRINTF_64 "u.%03" PRINTF_64 "u.%03" PRINTF_64 "u\n",
					(result & 0xff000000) >> 24, (result & 0x00ff0000) >> 16,
					(result & 0x0000ff00) >>  8, (result & 0x000000ff) >>  0);
		else
			printf("%" PRINTF_64 "u.%" PRINTF_64 "u.%" PRINTF_64 "u.%" PRINTF_64 "u\n",
					(result & 0xff000000) >> 24, (result & 0x00ff0000) >> 16,
					(result & 0x0000ff00) >>  8, (result & 0x000000ff) >>  0);
		break;
	default:
		printf("\n");
		printf("HEX: 0x%0" HEX_DIGITS PRINTF_64 "x,  DEC: %" PRINTF_64 "u,  OCT: 0%" PRINTF_64 "o\n\n", result, result, result);

		printf("GIGA: %.4lf,  MEGA: %.4lf,  KILO: %.4lf\n\nBIN: ",
				(double)result/(1024*1024*1024), (double)result/(1024*1024),
				(double)result/1024);

		for (i=MSB; i>=0; i--)
			printf("%c", result & BIT(i) ? '1' : '0');

		printf("\n\n    ");
		for (i=0; i<OCTETS; i++)
			printf("|%-2d     %2d", MSB-i*8, MSB-(i*8+7));

		printf("|\n    " SEPLINEBITVIEW "\n    ");
		for (i=MSB; i>=0; i--) {
			if (i%4 == 3)
				printf(i%8 == 7 ? "|" : " ");
			printf("%c", result & BIT(i) ? '1' : '0');
		}
		printf("|\n    " SEPLINEBITVIEW "\n    ");
		for (i=0; i<OCTETS; i++)
			printf("|%-2d     %2d", i*8, i*8+7);
		printf("|\n\n");
	}

	return 0;
}

