#*******************************************************************************
#
#	Name:			Makefile
#
#	Description:	This is the Makefile for the examples in the Process
#					Control chapter of Solaris Systems Programming.
#
#
#			Copyright © 2001-2004 by Rich Teer.  All rights reserved.
#
#*******************************************************************************

include ../Make.defines

TARGETS = echo_all exec_int execs fork gen_acct no_zombie pracct race1 race2 \
	ssp_system.o term_status test_sys vfork

all sol-2.5 sol-2.5.1 sol-2.6 sol-7 sol-8 sol-9: $(TARGETS)

clean:
	@rm -f $(TARGETS) $(CLEAN_TARGETS)

echo_all: echo_all.c
	$(CC) $(CFLAGS) -o $@ $@.c

exec_int: exec_int.c $(SSP_LIB)
	$(CC) $(CFLAGS) -o $@ $@.c $(SSP_LIB)

execs: execs.c $(SSP_LIB)
	$(CC) $(CFLAGS) -o $@ $@.c $(SSP_LIB)

fork: fork.c $(SSP_LIB)
	$(CC) $(CFLAGS) -o $@ $@.c $(SSP_LIB)

gen_acct: gen_acct.c $(SSP_LIB)
	$(CC) $(CFLAGS) -o $@ $@.c $(SSP_LIB)

no_zombie: no_zombie.c $(SSP_LIB)
	$(CC) $(CFLAGS) -o $@ $@.c $(SSP_LIB)

pracct: pracct.c $(SSP_LIB)
	$(CC) $(CFLAGS) -o $@ $@.c $(SSP_LIB)

race1: race1.c $(SSP_LIB)
	$(CC) $(CFLAGS) -o $@ $@.c $(SSP_LIB)

race2: race2.c $(SSP_LIB) ../pipes_and_fifos/tell_wait.o
	$(CC) $(CFLAGS) -o $@ $@.c ../pipes_and_fifos/tell_wait.o $(SSP_LIB)

# race2: race2.c $(SSP_LIB) ../signals/tell_wait.o
# 	$(CC) $(CFLAGS) -o $@ $@.c $(SSP_LIB) ../signals/tell_wait.o

../pipes_and_fifos/tell_wait.o: ../pipes_and_fifos/tell_wait.c
	$(CC) $(CFLAGS) -c -o $@ ../pipes_and_fifos/tell_wait.c

../signals/tell_wait.o: ../signals/tell_wait.c
	$(CC) $(CFLAGS) -c -D_POSIX_PTHREAD_SEMANTICS -o $@ ../signals/tell_wait.c

ssp_system.o: ssp_system.c
	$(CC) $(CFLAGS) -c ssp_system.c

term_status: term_status.c $(SSP_LIB)
	$(CC) $(CFLAGS) -o $@ $@.c $(SSP_LIB)

test_sys: test_sys.c $(SSP_LIB)
	$(CC) $(CFLAGS) -o $@ $@.c $(SSP_LIB) ssp_system.o

vfork: vfork.c $(SSP_LIB)
	$(CC) $(CFLAGS) -o $@ $@.c $(SSP_LIB)
