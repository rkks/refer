OBJS=fuzz 
BASE=../base
all: $(OBJS)

CFLAGS=-Wall -I$(BASE)/include
CFLAGS+=-g 

fuzz.o: fuzz.c
	$(CC) $(CFLAGS) -c $< 

fuzz: fuzz.o $(BASE)/tpl.o
	$(CC) $(CFLAGS) -o $@ $^ 

.PHONY: clean

clean:
	rm -f $(OBJS) *.o
