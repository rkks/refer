OBJS=sines 
BASE=../base
all: $(OBJS)

CFLAGS=-Wall -I$(BASE)/include
CFLAGS+=-g 

sines.o: sines.c
	$(CC) $(CFLAGS) -c $< 

sines: sines.o $(BASE)/tpl.o
	$(CC) $(CFLAGS) -o $@ $^ -lm

.PHONY: clean

clean:
	rm -f $(OBJS) *.o
