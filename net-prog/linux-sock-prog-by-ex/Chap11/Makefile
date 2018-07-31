CC=	gcc
STD=	_GNU_SOURCE
WARN=	-Wall -Wreturn-type

.c.o:
	$(CC) -c $(CFLAGS) -D$(STD) $(WARN) $<

all:    rpnsrv rpnsrv2

rpnsrv: rpnsrv.o rpneng.o mkaddr.o
	$(CC) rpnsrv.o rpneng.o mkaddr.o -o rpnsrv -lgmp

rpnsrv2: rpnsrv2.o rpneng2.o mkaddr.o
	$(CC) rpnsrv2.o rpneng2.o mkaddr.o -o rpnsrv2 -lgmp

clean:
	rm -f *.o core a.out

clobber: clean
	rm -f rpnsrv rpnsrv2
