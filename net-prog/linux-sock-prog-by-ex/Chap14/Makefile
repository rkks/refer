CC=	gcc
STD=	_GNU_SOURCE
WARN=	-Wall -Wreturn-type

.c.o:
	$(CC) -c $(CFLAGS) -D$(STD) $(WARN) -g $<

OOBRECV= oobrecv.o mkaddr.o bindacpt.o
OOBINLN= oobinline.o mkaddr.o bindacpt.o
OOBSEND= oobsend.o mkaddr.o bindacpt.o

all:    oobrecv oobsend oobinline

oobrecv: $(OOBRECV)
	$(CC) $(OOBRECV) -o oobrecv

oobsend: $(OOBSEND)
	$(CC) $(OOBSEND) -o oobsend

oobinline: $(OOBINLN)
	$(CC) $(OOBINLN) -o oobinline
clean:
	rm -f *.o core a.out

clobber: clean
	rm -f oobrecv oobsend oobinline
