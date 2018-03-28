#*******************************************************************************
#
#	Name:			Makefile
#
#	Description:	This is the Makefile for the example in the Basic File I/O
#					chapter of Solaris Systems Programming.
#
#
#			Copyright © 2001-2004 by Rich Teer.  All rights reserved.
#
#*******************************************************************************

include ../Make.defines

S9_TARGETS = closefrom copy copy_test directio file_flags file_xflags sparse

S26_TARGETS = copy copy_test directio file_flags sparse

S25_TARGETS = copy copy_test file_flags sparse

TARGETS = $(S9_TARGETS)

all sol-9: $(TARGETS)

sol-2.6 sol-7 sol-8: $(S26_TARGETS)

sol-2.5 sol-2.5.1: $(S25_TARGETS)

clean:
	@rm -f $(TARGETS) $(CLEAN_TARGETS)

closefrom: closefrom.c $(SSP_LIB)
	$(CC) $(CFLAGS) -o $@ $@.c $(SSP_LIB)

copy: copy.c $(SSP_LIB)
	$(CC) $(CFLAGS) -o $@ $@.c $(SSP_LIB)

copy_test: copy_test.c $(SSP_LIB)
	$(CC) $(CFLAGS) -o $@ $@.c $(SSP_LIB)

directio: directio.c $(SSP_LIB)
	$(CC) $(CFLAGS) -o $@ $@.c $(SSP_LIB)

file_flags: file_flags.c print_fsflags.o $(SSP_LIB)
	$(CC) $(CFLAGS) -o $@ $@.c print_fsflags.o $(SSP_LIB)

file_xflags: file_xflags.c print_fsflags.o $(SSP_LIB)
	$(CC) $(CFLAGS) -o $@ $@.c print_fsflags.o $(SSP_LIB)

print_fsflags.o: print_fsflags.c $(SSP_LIB)
	$(CC) $(CFLAGS) -c -o $@ print_fsflags.c

sparse: sparse.c $(SSP_LIB)
	$(CC) $(CFLAGS) -o $@ $@.c $(SSP_LIB)
