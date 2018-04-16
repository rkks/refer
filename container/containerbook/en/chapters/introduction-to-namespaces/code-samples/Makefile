.PHONY: clean

all: examples

unshare:
	$(CC) unshare.c -o unshare

unshare_clone:
	$(CC) unshare_clone.c -o unshare_clone

nsenter:
	$(CC) nsenter.c -o nsenter

examples: unshare nsenter unshare_clone

examples_clean:
	rm -f unshare nsenter unshare_clone
