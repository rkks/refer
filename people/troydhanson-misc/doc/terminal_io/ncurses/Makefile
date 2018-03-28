SRCS=$(wildcard *.c)
EXES=histogram histo-test histo-test2 slidogram hello 
CFLAGS=-g
LDFLAGS=-lncurses

all: $(EXES)

histogram: histogram.o tpl.o

histo-test: histo-test.o tpl.o

histo-test2: histo-test2.o tpl.o
	$(CC) -o $@ $^ -lpcap


.PHONY: clean
clean: 
	rm -f $(EXES) *.o
