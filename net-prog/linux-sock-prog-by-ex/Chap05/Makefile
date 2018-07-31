CC=	gcc
STD=	_GNU_SOURCE
WARN=	-Wall -Wreturn-type

.c.o:
	$(CC) -c $(CFLAGS) -D$(STD) $(WARN) $<

all:    bind sckname getpeer.o

bind:   bind.o
	$(CC) bind.o -o bind

sckname: sckname.o
	$(CC) sckname.o -o sckname

clean:
	rm -f *.o core a.out

clobber: clean
	rm -f bind sckname getpeer.o
