#CFLAGS = -g
OBJS = mkprob entropy1 entropy2 shlimit
ASCIIDOC=/usr/bin/asciidoc
DOC = Entropy.html
MATH = -lm

all: $(OBJS) $(DOC)

entropy1: entropy1.c
	$(CC) $(CFLAGS) -o $@ $< $(MATH)

entropy2: entropy2.c
	$(CC) $(CFLAGS) -o $@ $< $(MATH)

shlimit: shlimit.c
	$(CC) $(CFLAGS) -o $@ $< $(MATH)

Entropy.html: Entropy.txt
	if [ -x $(ASCIIDOC) ]; then $(ASCIIDOC) $<; fi

.PHONY: clean

clean:
	rm -f *.o $(OBJS)
