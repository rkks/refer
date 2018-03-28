/**
 * Sort of a getc for UTF8 data.
 *
 * Written by Tyler Montbriand.  Freeware.
 */
#include <stdio.h>
#include <stdint.h>
#include <string.h>

extern int octet_masks[], octet_res[];

// Nonzero if byte-string X matches the given length of UTF8 octet group
#define OCTET_MATCH(X, N)	(((X)&octet_masks[N])==octet_res[N])
#define OCTET_GET(X, N)		((X)>>((N)*8))

int utf8_getchar(const unsigned char *buf, int *pos_in, int maxpos)
{
	int pos=(*pos_in), octet=0;
	uint32_t acc=0;

	while((buf[pos] != '\0')&&(pos<maxpos))
	{
		acc<<=8;
		acc|=(uint32_t)buf[pos++];

		if(OCTET_MATCH(acc, octet))
		{
			uint32_t out;

			if(octet == 0)
			{
				(*pos_in)=pos;
				return(acc);
			}

			out=OCTET_GET(acc, octet)&(63>>octet);
			octet--;

			while(octet >= 0)
			{
				out<<=6;
				out|=OCTET_GET(acc, octet)&0x3f;
				octet--;
			}

			(*pos_in)=pos;
			return(out);
		}

		octet++;

		if(octet > 3)	return(-1);
	}

	return(-1);
}

int main(void)
{
	const unsigned char str[]={0xd7, 0x90, 0x10, 0x00};
	int pos=0;
	int max=strlen(str);
	int c=utf8_getchar(str, &pos, max);

	while(c >= 0)
	{
		printf("Got:  %d\n", c);
		c=utf8_getchar(str, &pos, max);
	}

	return(0);
}

int octet_masks[]={	0x00000080,	// One-byte octet
                   	0x0000e0c0,	// Two-byte octet
			0x00f0c0c0,	// Three-byte octet
			0xf8c0c0c0 };	// Four-byte octet

int octet_res[]={	0x00000000,
			0x0000c080,
			0x00e08080,
			0xf0808080	};
