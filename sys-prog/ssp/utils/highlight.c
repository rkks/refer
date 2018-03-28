void highlight (char *line, char *start, char *finish)
{
	char *c;

	for (c = line; c < start; c++)
		*c = ' ';

	for (; c <= finish; c++)
		*c = '^';

	for (c = finish + 1; *c != '\0'; c++)
		*c = ' ';
}
