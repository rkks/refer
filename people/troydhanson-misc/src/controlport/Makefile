CFLAGS = -I. 
CFLAGS += -g
LIB = libcontrolport.a
SRCS = controlport.c tpl.c
OBJS = $(patsubst %.c,%.o,$(SRCS))

EXESRC = $(wildcard app*.c)
EXES = $(patsubst %.c,%,$(EXESRC))

all: $(LIB) $(EXES) cpcli

$(OBJS): $(SRCS)

$(LIB): $(OBJS)
	ar cr $@ $^

$(EXES): $(EXESRC) $(LIB)
	$(CC) $(CFLAGS) -o $@ $@.c -L. -lcontrolport

cpcli: cpcli.c $(LIB)
	$(CC) $(CFLAGS) -o $@ $< tpl.c -L. -lcontrolport -lreadline

.PHONY: clean

clean:
	rm -f *.o $(LIB) $(EXES) cpcli
