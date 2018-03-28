OBJS=layout-text-file 
all: $(OBJS)

CFLAGS=-Wall -I./include
CFLAGS+=-g 

tpl.o: tpl.c
	$(CC) $(CFLAGS) -c $< 

layout.o: layout.c
	$(CC) $(CFLAGS) -c $< 

layout-text-file.o: layout-text-file.c
	$(CC) $(CFLAGS) -c $< 

layout-text-file: layout-text-file.o layout.o tpl.o
	$(CC) $(CFLAGS) -o $@ $^ 

.PHONY: clean

clean:
	rm -f $(OBJS) *.o
