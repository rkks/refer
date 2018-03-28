#include <stdio.h>

#define IPWORD(A, B, C, D)	((A)<<24)|((B)<<16)|((C)<<8)|(D)
#define IPBYTE(I, B)		(((I) >> (8*(B))) & 0xff)
#define IPALL(I)		IPBYTE(I, 3), IPBYTE(I, 2), IPBYTE(I, 1), IPBYTE(I, 0),

unsigned int strtoip(const char *str);

int main(int argc, char *argv[])
{
	unsigned int ip, mask;
	struct {
		int pos;
		int start[32], end[32];
	} ranges={0};
	int n, s;

	if(argc != 3)
		return(1);

	ip=strtoip(argv[1]);
	mask=strtoip(argv[2]);

	n=s=0;

	while(n < 32)
	{
		while(((mask & (1U<<n)) == 0) && (n < 32))
			n++;

		if(n != s)
		{
			ranges.start[ranges.pos]=s;
			ranges.end[ranges.pos]=n;
			ranges.pos++;
		}

		while(((mask & (1U<<n)) != 0) && (n < 32))
			n++;

		s=n;
	}

	printf("IP's matching %u.%u.%u.%u netmask %u.%u.%u.%u:\n",
		IPBYTE(ip, 3), IPBYTE(ip, 2), IPBYTE(ip,1), IPBYTE(ip, 0),
		IPBYTE(mask, 3), IPBYTE(mask, 2), IPBYTE(mask,1), IPBYTE(mask, 0));

	if(ranges.pos > 0)
	for(n=0; (n < (1 << ranges.pos)); n+=2)
	{
		unsigned int val=ip&mask;
		unsigned int val2=ip&mask;
		int q;
		for(q=0; q<ranges.pos; q++)
		{
			if(n & (1 << q))
			{
				// q is the index of bits to mask in/out
				unsigned int bits=1<< 
					(ranges.end[q]-ranges.start[q]);

				bits--;
				bits <<= ranges.start[q];
				val |= bits;
			}

			if((n+1) & (1 << q))
			{
				// q is the index of bits to mask in/out
				unsigned int bits=1<< 
					(ranges.end[q]-ranges.start[q]);

				bits--;
				bits <<= ranges.start[q];
				val2 |= bits;
			}
		}

		printf("%u.%u.%u.%u-%u.%u.%u.%u\n",
		IPBYTE(val, 3), IPBYTE(val, 2), IPBYTE(val,1), IPBYTE(val, 0),
		IPBYTE(val2, 3), IPBYTE(val2, 2), IPBYTE(val2,1), IPBYTE(val2, 0));
	}

	return(0);
}

unsigned int strtoip(const char *str)
{
	int a, b, c, d;

	if(sscanf(str, "%d.%d.%d.%d", &a, &b, &c, &d) != 4)
		return(-1);

	if((a<0)||(b<0)||(c<0)||(d<0))
		return(-1);

	if((a>255)||(b>255)|(c>255)|(d>255))
		return(-1);

	return(IPWORD(a, b, c, d));
}
