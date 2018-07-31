CC=	gcc
STD=	_GNU_SOURCE
WARN=	-Wall

.c.o:
	$(CC) -c $(CFLAGS) -D$(STD) $(WARN) $<

all:    netmask inetaddr inetaton inetntoa \
        network makeaddr

netmask: netmask.o
	$(CC) netmask.o -o netmask

inetaddr: inetaddr.o
	$(CC) inetaddr.o -o inetaddr

inetaton: inetaton.o
	$(CC) inetaton.o -o inetaton

inetntoa: inetntoa.o
	$(CC) inetntoa.o -o inetntoa

network:  network.o
	$(CC) network.o -o network

makeaddr: makeaddr.o
	$(CC) makeaddr.o -o makeaddr

clean:
	rm -f *.o core a.out

clobber: clean
	rm -f netmask inetaddr inetaton inetntoa \
            network makeaddr
