OBJS=libts.a
all: $(OBJS)
CFLAGS = -I. 
CFLAGS += -g

ts.o: ts.c ts.h

libts.a: ts.o
	ar cr $@ $^

.PHONY: clean

clean:
	rm -f *.o $(OBJS)
