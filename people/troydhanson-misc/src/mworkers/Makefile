SRCS=$(wildcard *.c)
OBJS=$(patsubst %.c,%,$(SRCS))
all: $(OBJS)

# the threaded programs need the -pthread flag
TSRCS=$(wildcard tworkers*.c)
TOBJS=$(patsubst %.c,%,$(TSRCS))
$(TOBJS): $(TSRCS)
	$(CC) $(CFLAGS) -pthread -o $@ $@.c

.PHONY: clean

clean:
	rm -f $(OBJS) *.o
