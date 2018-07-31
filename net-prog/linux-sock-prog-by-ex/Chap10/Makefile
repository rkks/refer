CC=	gcc
STD=	_GNU_SOURCE
WARN=	-Wall -Wreturn-type

.c.o:
	$(CC) -c $(CFLAGS) -D$(STD) $(WARN) $<

all:    rpnsrv

rpnsrv: rpnsrv.o rpneng.o mkaddr.o
	$(CC) rpnsrv.o rpneng.o mkaddr.o -o rpnsrv -lgmp

clean:
	rm -f *.o core a.out

clobber: clean
	rm -f rpnsrv
