/**
 *	grab.c
 *	grabs an evdev device to prevent it from sending events
 * 	to /dev/input/mice.
 */
#include <sys/types.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>

#include <linux/input.h>

#include <stdio.h>

int main(int argc, char *argv[])
{
	int fd;

	if(argc != 2)
	{
		fprintf(stderr, "Syntax:  %s device-to-lock\n", argv[0]);
		return(1);
	}

	fd=open(argv[1], O_RDONLY);
	if(fd < 0)
	{
		perror("Couldn't open");
		return(1);
	}

	if(ioctl(fd, EVIOCGRAB, (void *)1))
	{
		perror("Couldn't grab");
		close(fd);
		return(1);
	}

	while(1)	sleep(60*60);
	close(fd);
}
