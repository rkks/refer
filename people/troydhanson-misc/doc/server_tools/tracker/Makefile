OBJS=libtracker.a 
all: $(OBJS)
CFLAGS= -I. -Iinclude
#CFLAGS+=-O2
CFLAGS+=-g 

tracker.o: tracker.c tracker.h
	$(CC) $(CFLAGS) -c $<

libtracker.a: tracker.o
	ar cr $@ $^

.PHONY: clean

clean:
	rm -f *.o $(OBJS)
