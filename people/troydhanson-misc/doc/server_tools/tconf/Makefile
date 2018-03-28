all: libtconf.a

libtconf.a: tconf.c
	$(CC) $(CFLAGS) -c $<
	ar cru libtconf.a tconf.o

clean:
	rm -f libtconf.a *.o
