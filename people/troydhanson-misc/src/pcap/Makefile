SRCS=$(wildcard *.c)
OBJS=$(patsubst %.c,%,$(SRCS))
all: $(OBJS)
LDFLAGS=-lpcap

.PHONY: clean

clean:
	rm -f $(OBJS) *.o
