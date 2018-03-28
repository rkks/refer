EXES=read_serial
all: $(EXES)

CFLAGS=-g
LDFLAGS=

$(EXES): %: %.c
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

clean:
	rm -f $(EXES) *.o
