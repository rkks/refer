#include <stdio.h>

FILE *gulp(FILE *fin)
{
	char buf[65536];
	size_t bytes;
	FILE *fout=tmpfile();
	if(fout == NULL)
		return(NULL);

	while(!feof(fin))
	{
		size_t bytes=fread(buf, 1, 65536, fin);
		//fprintf(stderr, "Read %d bytes\n", (int)bytes);
		if(bytes != fwrite(buf, 1, bytes, fout))
		{
			fprintf(stderr, "Error writing temp file\n");
			fclose(fout);
			return(NULL);
		}
	}

	fclose(fin);
	fseek(fout, 0, SEEK_SET);
	return(fout);
}
