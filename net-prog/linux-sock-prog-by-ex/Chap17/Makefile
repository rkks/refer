CC=	gcc
STD=	_GNU_SOURCE
WARN=	-Wall -Wreturn-type

.c.o:
	$(CC) -g -c $(CFLAGS) -D$(STD) $(WARN) $<

WEBOBJ= web80.o misc.o reqport.o recvfd.o
SRVOBJ= sockserv.o misc.o recvcred.o sendfd.o

all:    writev web80 sockserv

writev: writev.o
	$(CC) writev.o -o writev

web80:  $(WEBOBJ)
	$(CC) $(WEBOBJ) -o web80

sockserv: $(SRVOBJ)
	$(CC) $(SRVOBJ) -o sockserv

clean:
	rm -f *.o core a.out

clobber: clean
	rm -f writev web80 sockserv
