#*******************************************************************************
#
#	Name:			Makefile
#
#	Description:	This is the Makefile for the examples in the Interprocess
#					Communication Using Pipes and FIFOs chapter of Solaris
#					Systems Programming.
#
#
#			Copyright © 2001-2004 by Rich Teer.  All rights reserved.
#
#*******************************************************************************

include ../Make.defines

TARGETS = coproc fifo_client fifo_server pipe pipemail popen.o popenmail rot13 \
	sqrt sqrt_stdio tell_wait.o test_popen

all sol-2.5 sol-2.5.1 sol-2.6 sol-7 sol-8 sol-9: $(TARGETS)

clean:
	@rm -f $(TARGETS) $(CLEAN_TARGETS)

coproc: coproc.c $(SSP_LIB)
	$(CC) $(CFLAGS) -o $@ $@.c $(SSP_LIB)

fifo_client: fifo_client.c $(SSP_LIB)
	$(CC) $(CFLAGS) -o $@ $@.c $(SSP_LIB)

fifo_server: fifo_server.c $(SSP_LIB)
	$(CC) $(CFLAGS) -o $@ $@.c $(SSP_LIB) -lm

pipe: pipe.c $(SSP_LIB)
	$(CC) $(CFLAGS) -o $@ $@.c $(SSP_LIB)

pipemail: pipemail.c $(SSP_LIB)
	$(CC) $(CFLAGS) -o $@ $@.c $(SSP_LIB)

popen.o: popen.c
	$(CC) $(CFLAGS) -c popen.c

popenmail: popenmail.c $(SSP_LIB)
	$(CC) $(CFLAGS) -o $@ $@.c $(SSP_LIB)

rot13: rot13.c $(SSP_LIB)
	$(CC) $(CFLAGS) -o $@ $@.c $(SSP_LIB) -lgen

sqrt: sqrt.c $(SSP_LIB)
	$(CC) $(CFLAGS) -o $@ $@.c $(SSP_LIB) -lm

sqrt_stdio: sqrt_stdio.c $(SSP_LIB)
	$(CC) $(CFLAGS) -o $@ $@.c $(SSP_LIB) -lm

tell_wait.o: tell_wait.c
	$(CC) $(CFLAGS) -c tell_wait.c

test_popen: test_popen.c $(SSP_LIB) popen.o
	$(CC) $(CFLAGS) -o $@ $@.c $(SSP_LIB) popen.o
