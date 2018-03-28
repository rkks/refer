#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <time.h>

// Element count of a statically allocated array
#define ELEMENTS(X)	((sizeof(X)/sizeof((X)[0]))-1)

#define KBD_MAX_X		13
#define KBD_MAX_Y		4
#define RANGE(X,MIN,MAX)	(((X)<=(MAX))&&((X)>=(MIN)))
#define KEY_VALID(X,Y)		(RANGE(X, 0, KBD_MAX_X) &&	\
					RANGE(Y, 0, KBD_MAX_Y))

// Arrays to find keys in
extern const char *kbd_ucase[], *kbd_lcase[];

// Find a key in the arrays
int find_key(int key, int *x, int *y, int *upper);
// Screw up typing
void pick_random_key(int *ix, int *iy);

int main(void)
{
	int drunkenness=20;
	srand(time(NULL));

	while(!feof(stdin))
	{
		int x, y;
		int shift=0;
		static int caps=0;
		int c=fgetc(stdin);
		if(c<0)	continue;

		if(find_key(c, &x, &y, &shift) < 0)
		{
			printf("%c", c);
			continue;
		}

		// Screw up shift key
		if(shift && ((rand() % drunkenness)==0))
			shift^=1;
		// Screw up other keys
		if((rand() % drunkenness)==0)
			pick_random_key(&x, &y);

		switch(kbd_lcase[y][x])
		{
		case '\x11':	// capslock
			caps=caps^1;
			/* FALL THROUGH */
		case '\x10':	// Do-nothing key
			break;

		case '\b':	// Print blank over backspaced char
			printf("^G");
//			printf("\b \b");
			break;

		default:	// printing key
			if(shift ^ caps)
				printf("%c", kbd_ucase[y][x]);
			else
				printf("%c", kbd_lcase[y][x]);

			break;
		}
	}

	return(0);
}

struct
{
  int x, y;
} noff[]={ {1, 0}, {0, 1}, {-1,1}, {-1,0}, {0,-1}, {1,-1}, {0, 0} };

int find_key(int key, int *x, int *y, int *shift)
{
	int n;

	if((key >= 'A') && (key <= 'Z'))
	{
		(*shift)=1;
		key=tolower(key);
	}

	for(n=0; kbd_lcase[n] != NULL; n++)
	{
		const char *s=strchr(kbd_lcase[n], key);
		if(s != NULL)
		{
			(*y)=n;
			(*x)=((long int)s)-((long int)kbd_lcase[n]);
			return(0);
		}
	}

	for(n=0; kbd_ucase[n] != NULL; n++)
	{
		const char *s=strchr(kbd_ucase[n], key);
		if(s != NULL)
		{
			(*y)=n;
			(*x)=((long int)s)-((long int)kbd_ucase[n]);
			(*shift)=1;
			return(0);
		}
	}

	return(-1);
}

void pick_random_key(int *ix, int *iy)
{
	int tries=100;
	int r=rand()%ELEMENTS(noff);
	int x=(*ix)+noff[r].x;
	int y=(*iy)+noff[r].y;

	while(! KEY_VALID(x, y))
	{
		tries--;
		if(tries<0)
			return;

		r=rand()%ELEMENTS(noff);
		x=(*ix)+noff[r].x;
		y=(*ix)+noff[r].y;
	}

	(*ix)=x;
	(*iy)=y;
}

const char *kbd_ucase[]={
		"~!@#$%^&*()_+\b",
		"\tQWERTYUIOP{}|",
		"\x11" "ASDFGHJKL:\"\n\n",
		"\x10" "ZXCVBNM<>?\x10\x10\x10",
		"              ",
		NULL
	};

const char *kbd_lcase[]={
		"`1234567890-=\b",
		"\tqwertyuiop[]\\",
		"\x11" "asdfghjkl;'\n\n",
		"\x10" "zxcvbnm,./\x10\x10\x10",
		"              ",
		NULL
	};
