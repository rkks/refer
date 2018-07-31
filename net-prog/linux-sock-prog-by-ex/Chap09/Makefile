CC=	gcc
STD=	_GNU_SOURCE
WARN=	-Wall -Wreturn-type

.c.o:
	$(CC) -c $(CFLAGS) -D$(STD) $(WARN) $<

all:    uname gethostn lookup srvr2 lookup2

uname: uname.o
	$(CC) uname.o -o uname

gethostn: gethostn.o
	$(CC) gethostn.o -o gethostn

lookup: lookup.o
	$(CC) lookup.o -o lookup

srvr2:  srvr2.o
	$(CC) srvr2.o -o srvr2

lookup2: lookup2.o
	$(CC) lookup2.o -o lookup2

clean:
	rm -f *.o core a.out srvr2.log

clobber: clean
	rm -f uname gethostn lookup srvr2 lookup2
