SRCS = $(wildcard test*.c) 
PROGS = $(patsubst %.c,%,$(SRCS))

LIBDIR = ..
LIB = $(LIBDIR)/libzcontrol.a
LDFLAGS = -L$(LIBDIR) -lzcontrol -lzmq

CFLAGS = -I$(LIBDIR)
CFLAGS += -g
#CFLAGS += -DZMQ_MAKE_VALGRIND_HAPPY=1
CFLAGS += -Wall 
CFLAGS += ${EXTRA_CFLAGS}

all: $(PROGS)

$(PROGS): $(SRCS) $(LIB) 
	$(CC) $(CFLAGS) -o $@ $(@).c $(LDFLAGS)

.PHONY: clean

clean:	
	rm -f $(PROGS) 
