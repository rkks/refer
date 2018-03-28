SRCS=$(wildcard *.c)
EXES=$(patsubst %.c,%,$(SRCS))
all: $(EXES)

CFLAGS=-I/usr/include/cairo
CFLAGS+=-g
LDFLAGS=-lcairo

$(EXES): $(SRCS)
	$(CC) $(CFLAGS) -o $@ $@.c $(LDFLAGS)

.PHONY: clean

clean:
	rm -f $(EXES) *.png
