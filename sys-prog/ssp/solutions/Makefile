#*******************************************************************************
#
#	Name:			Makefile
#
#	Description:	This is the Makefile for the examples in the Solutions to
#					Selected Exercises appendix of Solaris Systems Programming.
#
#
#			Copyright © 2001-2004 by Rich Teer.  All rights reserved.
#
#*******************************************************************************

include ../Make.defines

S26_TARGETS = door_srv file_size ghost lastlog ltostr max_fd open_fifo pipe_cap pgleader \
	pty_child pty_parent ttypgid unref vfork zombie

S25_TARGETS = file_size ghost lastlog ltostr max_fd open_fifo pipe_cap pgleader \
	pty_child pty_parent ttypgid unref vfork zombie

TARGETS = $(S26_TARGETS)

all sol-2.6 sol-7 sol-8 sol-9: $(TARGETS)

sol-2.5 sol-2.5.1: $(S25_TARGETS)

clean:
	@rm -f $(TARGETS) $(CLEAN_TARGETS)

door_srv: door_srv.c $(SSP_LIB)
	$(CC) $(CFLAGS) $(MTFLAGS) -o $@ $@.c $(SSP_LIB) -ldoor

file_size: file_size.c $(SSP_LIB)
	$(CC) $(CFLAGS) -o $@ $@.c $(SSP_LIB)

ghost: ghost.c $(SSP_LIB)
	$(CC) $(CFLAGS) -o $@ $@.c $(SSP_LIB)

lastlog: lastlog.c $(SSP_LIB)
	$(CC) $(CFLAGS) -o $@ $@.c $(SSP_LIB)

ltostr: ltostr.c $(SSP_LIB)
	$(CC) $(CFLAGS) -o $@ $@.c $(SSP_LIB)

max_fd: max_fd.c $(SSP_LIB)
	$(CC) $(CFLAGS) -o $@ $@.c $(SSP_LIB)

open_fifo: open_fifo.c $(SSP_LIB)
	$(CC) $(CFLAGS) -o $@ $@.c $(SSP_LIB)

pipe_cap: pipe_cap.c $(SSP_LIB)
	$(CC) $(CFLAGS) -o $@ $@.c $(SSP_LIB)

pgleader: pgleader.c $(SSP_LIB)
	$(CC) $(CFLAGS) -o $@ $@.c $(SSP_LIB)

pty_child: pty_child.c $(SSP_LIB)
	$(CC) $(CFLAGS) -o $@ $@.c $(SSP_LIB)

pty_parent: pty_parent.c $(SSP_LIB) ../pseudo_terms/pty_fork.o
	$(CC) $(CFLAGS) -o $@ $@.c $(SSP_LIB) ../pseudo_terms/pty_fork.o

../pseudo_terms/pty_fork.o: ../pseudo_terms/pty_fork.c
	$(CC) $(CFLAGS) -c -o $@ ../pseudo_terms/pty_fork.c

ttypgid: ttypgid.c $(SSP_LIB)
	$(CC) $(CFLAGS) -o $@ $@.c $(SSP_LIB)

unref: unref.c $(SSP_LIB)
	$(CC) $(CFLAGS) -o $@ $@.c $(SSP_LIB)

vfork: vfork.c $(SSP_LIB)
	$(CC) $(CFLAGS) -o $@ $@.c $(SSP_LIB)

zombie: zombie.c $(SSP_LIB)
	$(CC) $(CFLAGS) -o $@ $@.c $(SSP_LIB)
