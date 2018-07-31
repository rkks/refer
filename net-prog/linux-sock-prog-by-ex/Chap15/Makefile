CC=	gcc
STD=	_GNU_SOURCE
WARN=	-Wall -Wreturn-type

.c.o:
	$(CC) -c $(CFLAGS) -D$(STD) $(WARN) $<

all:    inetdserv

inetdserv: inetdserv.o
	$(CC) inetdserv.o -o inetdserv

clean:
	rm -f *.o core a.out

clobber: clean
	rm -f inetdserv

