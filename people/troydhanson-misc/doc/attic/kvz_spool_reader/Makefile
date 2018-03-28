OBJS=kvz 
all: $(OBJS)
CFLAGS=-g -I. -I./include

# edit to reflect locations, must be built
CFLAGS += -I/usr/local/include
LDFLAGS = -L/usr/local/lib
LIBS=-lzmq -lkvspool -lts -lzcontrol

kvz: kvz.c kvz_cmds.c 
	$(CC) $(CFLAGS) -pthread -c kvz.c
	$(CC) $(CFLAGS) -pthread -c kvz_cmds.c
	$(CC) $(CFLAGS) -pthread -o kvz kvz.o kvz_cmds.o $(LDFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f *.o $(OBJS)
