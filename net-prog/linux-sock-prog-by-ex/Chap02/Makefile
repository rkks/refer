CC=	gcc
STD=	_GNU_SOURCE
WARN=	-Wall

.c.o:
	$(CC) -c $(CFLAGS) -D$(STD) $(WARN) $<

all:    af_unix af_unix2 af_inet af_x25 af_ax25

af_unix: af_unix.o
	$(CC) af_unix.o -o af_unix

af_unix2: af_unix2.o
	$(CC) af_unix2.o -o af_unix2

af_inet: af_inet.o
	$(CC) af_inet.o -o af_inet

af_x25: af_x25.o
	$(CC) af_x25.o -o af_x25

af_ax25: af_ax25.o
	$(CC) af_ax25.o -o af_ax25

run_all: af_unix af_unix2 af_inet af_inet2 \
	af_x25 af_ax25
	./af_unix
	./af_unix2
	./af_inet
	./af_inet2
	./af_x25
	./af_ax25
	
clean:
	rm -f *.o core a.out

clobber: clean
	rm -f af_unix af_inet af_inet2 af_x25 af_ax25
