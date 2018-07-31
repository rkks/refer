CC=	gcc
STD=	_GNU_SOURCE
WARN=	-Wall

.c.o:
	$(CC) -c $(CFLAGS) -D$(STD) $(WARN) $<

all:    servent protoent daytime

servent: servent.o
	$(CC) servent.o -o servent

protoent: protoent.o
	$(CC) protoent.o -o protoent

daytime: daytime.o
	$(CC) daytime.o -o daytime

clean:
	rm -f *.o core a.out

clobber: clean
	rm -f servent protoent

