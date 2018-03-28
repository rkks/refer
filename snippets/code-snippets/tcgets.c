// Example of configuring a Linux serial port for raw access.
// Written by Tyler Montbriand, 2007.  Freeware.

// See	http://man-wiki.net/index.php/3:termios
//	http://man-wiki.net/index.php/4:tty_ioctl
#include "tcgets.h"

#include <unistd.h>
#include <fcntl.h>
#include <termios.h>

#include <stdio.h>
#include <ctype.h>

// For a standalone test, do:
// gcc -DMAIN_TEST tcgets.c -o tcgets
#ifdef MAIN_TEST
int main(int argc, char *argv[])
{
	const char *settings="19200N81";
	const char *term=DEFAULT_TERM;
	int fd;

	if(argc >= 2)
		term=argv[1];

	if(argc >= 3)
		settings=argv[2];

	fd=termios_open(term, settings);

	if(fd<0)
	{
		fprintf(stderr, "Couldn't open %s\n", term);
		return(1);
	}

	fprintf(stderr, "Opened %s as %s\n", term, settings);

	{
		char in;
		fprintf(stderr, "Attempting blocking read timeout...\n");
		fprintf(stderr, "Returned: %d\n", read(fd, &in, 1));
	}

	close(fd);

	return(0);
}
#endif

static struct termios cur_termios;	// Current terminal settings

int termios_open(const char *fname, const char *cmdstr)
{
	int fd=open(fname, O_RDWR);
	if(fd<0)
		goto TERMIOS_OPEN_ERR;

	if(ttyname(fd) == NULL)
		goto TERMIOS_OPEN_ERR;

	if(termios_set(fd, cmdstr) < 0)
		goto TERMIOS_OPEN_ERR;

	return(fd);

TERMIOS_OPEN_ERR:
	if(fd >= 0)
		close(fd);

	return(-1);
}

struct valid_pairs
{
	int out;
	int in;
};

static const struct valid_pairs valid_parity[], valid_stopbits[], 
	valid_bits[], valid_baud[];

int isvalid(unsigned int in, const struct valid_pairs valid[])
{
	int n;
	for(n=0; valid[n].in >= 0; n++)
	{
		fprintf(stderr,"%d]\t%d\t<=>\t%d\n", n,	valid[n].in, in);
		if(valid[n].in == in)
			return(valid[n].out);
	}

	return(-1);
}

int termios_set(int fd, const char *cmdstr)
{

	int baud=-1, bits=-1, stopbits=-1;
	unsigned char parity=0;
	int values;

	if(cmdstr == NULL)
		cmdstr=DEFAULT_TTY_SETTINGS;

	values=sscanf(cmdstr, "%i%[nNeEoO]%01i%01i",
		&baud, &parity, &bits, &stopbits);
	if(values != 4)
	{
		fprintf(stderr, "Bad spec string '%s'\n", cmdstr);
		return(-1);
	}

	baud=isvalid(baud, valid_baud);
	if(baud < 0)
	{
		fprintf(stderr,"Bad spec string '%s':  Invalid baud rate\n",
			cmdstr);
		return(-1);
	}

	parity=isvalid((char)toupper(parity), valid_parity);
	if(parity ==(unsigned char)(-1))
	{
		fprintf(stderr, "Bad spec string '%s':  "
			"Invalid parity specifier\n", cmdstr);
		return(-1);
	}

	bits=isvalid(bits, valid_bits);
	if(bits < 0)
	{
		fprintf(stderr, "Bad spec string '%s':  "
			"Invalid bits specifier\n", cmdstr);
		return(-1);
	}

	stopbits=isvalid(stopbits, valid_stopbits);
	if(stopbits < 0)
	{
		fprintf(stderr, "Bad spec string %s:  "
			"Invalid stop bits specifier\n", cmdstr);
		return(-1);
	}

	fprintf(stderr, "Configuring for %s\n", cmdstr);

	// Ignore errors, BREAK condition; restart on any character
	cur_termios.c_iflag = IGNBRK | IGNPAR | IXANY;
	cur_termios.c_oflag = 0;
	cur_termios.c_lflag = 0; 

	// Enable reading, ignore modem lines, enable CTS/RTS
	cur_termios.c_cflag = CLOCAL | CREAD | CRTSCTS; 

	cur_termios.c_cflag |= stopbits;
	cur_termios.c_cflag |= bits;
	cur_termios.c_cflag |= parity;

	// Enable error checking when parity enabled
	if(parity != 0)
		cur_termios.c_iflag &= ~(IGNPAR|IGNBRK);

	// Set baud rate
	cfsetispeed(&cur_termios, baud);
	cfsetospeed(&cur_termios, baud);

	cur_termios.c_cc[VMIN]=0;			// Timeout needs 0 chars
	cur_termios.c_cc[VTIME]=DEFAULT_TTY_TIMEOUT;	// Timeout in tenths

	return(tcsetattr(fd, TCSADRAIN, &cur_termios));	
}

int termios_defaults(int fd)
{
	return(termios_set(fd, NULL));
}

static const struct valid_pairs valid_parity[]={
		{ 0,			'N'},	// No parity
		{ PARENB,		'E'},	// Even parity
		{ PARENB|PARODD,	'O'},	// Odd parity
		{ 0,			-1}
	};

static const struct valid_pairs valid_stopbits[]={
		{ 0,		1},
		{CSTOPB,	2},
		{ 0,		-1}
	};

static const struct valid_pairs valid_bits[]={
		{ CS5,	5},
		{ CS6,	6},
		{ CS7,	7},
		{ CS8,	8},
		{ 0,	-1}
	};

static const struct valid_pairs valid_baud[]={
		{B0,		0},
		{B50,		50},
		{B75,		75},
		{B110,		110},
		{B134,		134},
		{B150,		150},
		{B200,		200},
		{B300,		300},
		{B600,		600},
		{B1200,		1200},
		{B1800,		1800},
		{B2400,		2400},
		{B4800,		4800},
		{B9600,		9600},
		{B19200,	19200},
		{B38400,	38400},
		{B57600,	57600},
		{B115200,	115200},
		{B230400,	230400},
		{0,		-1}};
