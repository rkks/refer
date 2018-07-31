CC=	gcc
STD=	_GNU_SOURCE
WARN=	-Wall -Wreturn-type

.c.o:
	$(CC) -c $(CFLAGS) -D$(STD) $(WARN) $<

all:    getsndrcv setsndrcv gettype 

getsndrcv: getsndrcv.o
	$(CC) getsndrcv.o -o getsndrcv

setsndrcv: setsndrcv.o
	$(CC) setsndrcv.o -o setsndrcv

gettype: gettype.o
	$(CC) gettype.o -o gettype

clean:
	rm -f *.o core a.out

clobber: clean
	rm -f getsndrcv setsndrcv gettype
