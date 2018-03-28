OBJS=tpl.o
CFLAGS=-Wall
CFLAGS+=-g

all: $(OBJS)

tpl.o: tpl.c include/tpl.h
	$(CC) $(CFLAGS) -c tpl.c

.PHONY: clean

clean:
	rm -f tpl.o
