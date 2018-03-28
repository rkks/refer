#CFLAGS = -Wall 
CFLAGS = -g

all: lemon readcfg

lemon: lemon.c

cfg.c: cfg.y
	-./lemon $^

readcfg: readcfg.c tok.c slurp.c cfg.c 

doc: lemon_notes.txt
	asciidoc $<

clean:
	rm -f lemon readcfg *.o cfg.c cfg.h cfg.out
