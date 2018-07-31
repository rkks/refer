CC=	gcc
STD=	_GNU_SOURCE
WARN=	-Wall -Wreturn-type

.c.o:
	$(CC) -c $(CFLAGS) -D$(STD) $(WARN) $<

all:    server daytime

server: server.o
	$(CC) server.o -o server

daytime: daytime.o
	$(CC) daytime.o -o daytime

clean:
	rm -f *.o core a.out

clobber: clean
	rm -f server daytime

