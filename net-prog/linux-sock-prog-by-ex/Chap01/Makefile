CC=	gcc
STD=	_GNU_SOURCE
WARN=	-Wall

TARGTS= 01lst01 01lst02 01lst03

.c.o:
	$(CC) -c $(CFLAGS) -D$(STD) $(WARN) $<

all:	$(TARGTS)

01lst01: 01LST01.o
	$(CC) 01LST01.o -o 01lst01

01lst02: 01LST02.o
	$(CC) 01LST02.o -o 01lst02

01lst03: 01LST03.o
	$(CC) 01LST03.o -o 01lst03

clean:
	rm -f *.o core a.out

clobber: clean
	rm -f $(TARGTS)
