get(fd, pos, buf, n) /* read n bytes from position pos */
	int fd, n;
	long pos;
	char *buf;
{
	if (lseek(fd, pos, 0) == -1)	/* get to pos */
		return -1;
	else
		return read(fd, buf, n);
}
