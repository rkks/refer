CC=	gcc
STD=	_GNU_SOURCE
WARN=	-Wall -Wreturn-type

.c.o:
	$(CC) -c $(CFLAGS) -D$(STD) $(WARN) -g $<

all:    stksrv gquotes

stksrv: stksrv.o mkaddr.o
	$(CC) -g stksrv.o mkaddr.o -o stksrv

gquotes: gquotes.o mkaddr.o
	$(CC) -g gquotes.o mkaddr.o -o gquotes

clean:
	rm -f *.o core a.out

clobber: clean
	rm -f stksrv gquotes
