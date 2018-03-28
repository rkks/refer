OBJS=libts.a
all: $(OBJS)
CFLAGS = -I. 
CFLAGS += -g

ts.o: ts.c ts.h

libts.a: ts.o
	ar cr $@ $^

PREFIX=/usr/local
install: libts.a
	cp ts.h $(PREFIX)/include
	cp libts.a $(PREFIX)/lib

.PHONY: clean

clean:
	rm -f *.o $(OBJS)
