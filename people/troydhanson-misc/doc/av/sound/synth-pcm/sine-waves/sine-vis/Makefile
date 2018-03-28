OBJS=sine-vis 
all: $(OBJS)

CFLAGS=-Wall -Iinclude
CFLAGS+=-g 

tpl.o: tpl.c
	$(CC) $(CFLAGS) -c $< 

sine-vis.o: sine-vis.c
	$(CC) $(CFLAGS) -c $< 

sine-vis: sine-vis.o tpl.o
	$(CC) $(CFLAGS) -o $@ $^ 

.PHONY: clean

clean:
	rm -f $(OBJS) *.o
