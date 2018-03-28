OBJS=render-png
all: $(OBJS)

CFLAGS=-Wall -I./include
CFLAGS+=-g 

tpl.o: tpl.c
	$(CC) $(CFLAGS) -c $< 

render-png.o: render-png.c
	$(CC) $(CFLAGS) -c $< 

render-png: render-png.o tpl.o
	$(CC) $(CFLAGS) -o $@ $^ -lcairo

.PHONY: clean

clean:
	rm -f $(OBJS) *.o
