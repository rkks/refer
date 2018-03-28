#include <sys/timeb.h>
#include <unistd.h>
#include <stdint.h>
#include <stdio.h>

uint64_t getticks(void)
{
	uint64_t out;
	struct timeb tb;

	ftime(&tb);
	out=((uint64_t)tb.time*(uint64_t)1000)+((uint64_t)tb.millitm);
	return(out);
}

int main(int argc, char *argv[])
{
	uint8_t prop=128;	// 128/256 = 50%
	uint64_t start, end;

	if(argc > 1)
	{
		int v;

		if(sscanf(argv[1], "%d", &v) != 1)
		{
			fprintf(stderr, "Invalid prop %s\n", argv[1]);
			exit(1);
		}

		if((v < 0) || (v > 255))
		{
			fprintf(stderr, "Invalid prop %s\n", argv[1]);
			exit(1);
		}

		prop=v;	
		fprintf(stderr, "Prop %d/255(%d%%)\n", prop,
			(prop*100)/255);
	}

	while(1)
	{
		start=getticks();
		while((getticks() - start) < (uint64_t)(255-prop))
			usleep(1000);

		// I'm spinnin'!
		while((getticks() - start) < 255)	{}
	}
}
