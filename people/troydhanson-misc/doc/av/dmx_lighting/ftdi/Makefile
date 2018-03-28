EXES=rgb rgb-mmap
all: $(EXES)

CFLAGS=-g
CFLAGS+=-I/usr/include/libftdi
LDFLAGS=-lftdi

$(EXES): %: %.c
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

clean:
	rm -f $(EXES) *.o
