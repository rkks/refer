CC=	gcc
STD=	_GNU_SOURCE
WARN=	-Wall

.c.o:
	$(CC) -c $(CFLAGS) -D$(STD) $(WARN) $<

all:    dgramsrvr dgramclnt

dgramsrvr: dgramsrvr.o
	$(CC) dgramsrvr.o -o dgramsrvr

dgramclnt: dgramclnt.o
	$(CC) dgramclnt.o -o dgramclnt

clean:
	rm -f *.o core a.out

clobber: clean
	rm -f dgramsrvr dgramclnt
