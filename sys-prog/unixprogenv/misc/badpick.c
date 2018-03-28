pick(s)	/* offer choice of s */
	char *s;
{
	fprintf("%s? ", s);
	if (ttyin() == 'y')
		printf("%s\n", s);
}
