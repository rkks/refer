STD=	-D_GNU_SOURCE
INCL=
WARN=	-Wall -Wreturn-type
DEBUG= -g
CCOPTS=	-c $(DEBUG) $(WARN) $(STD) $(INCL)
CC=	gcc

COOBJ=	mkaddr.o
QSOBJ=	qserve.o csvparse.o msgf.o load.o gettick.o bcast.o connect.o misc.o $(COOBJ)
QCOBJ=	mktwatch.o $(COOBJ)

.c.o:
	$(CC) $(CCOPTS) $<

all:	qserve mktwatch

qserve:	$(QSOBJ)
	$(CC) -o qserve $(QSOBJ)

mktwatch: $(QCOBJ)
	$(CC) -o mktwatch $(QCOBJ)

clean:
	rm -f *.o core

clobber: clean
	rm -f qserve mktwatch

mktwatch.o: quotes.h
mkaddr.o:   quotes.h
connect.o:  quotes.h
misc.o:     quotes.h
qserve.o:   quotes.h
csvparse.o: quotes.h
msgf.o:     quotes.h
load.o:     quotes.h
gettick.o:  quotes.h
