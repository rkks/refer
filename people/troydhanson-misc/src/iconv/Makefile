all: iconv_8to32

iconv_8to32: iconv_8to32.c
	$(CC) -o $@ $< -liconv

clean:
	rm -f iconv_8to32 *.o
