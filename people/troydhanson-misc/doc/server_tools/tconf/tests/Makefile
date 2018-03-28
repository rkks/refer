LIBTCONF=../libtconf.a
CFLAGS = -I..

SRCS=$(wildcard *.c)
OBJS=$(SRCS:.c=)

$(OBJS): $(SRCS) $(LIBTCONF)
	for OBJ in $(OBJS); \
	do $(CC) $(CFLAGS) -DCONF=\"$$OBJ.cf\" -o $$OBJ $$OBJ.c $(LIBTCONF); \
	done

# make the library in the parent directory if needed
$(LIBTCONF): ../tconf.c ../tconf.h
	make -C.. 

all: $(OBJS) tests

tests: $(OBJS)
	perl ./do_tests

clean: 
	rm -f $(OBJS)
