SRCS=$(wildcard *.c)
EXES=$(patsubst %.c,%,$(SRCS))
CFLAGS=-g
LDFLAGS=-lncurses

all: $(EXES)

.PHONY: clean
clean: 
	rm -f $(EXES) *.o
