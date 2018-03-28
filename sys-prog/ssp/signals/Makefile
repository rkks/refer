#*******************************************************************************
#
#	Name:			Makefile
#
#	Description:	This is the Makefile for the examples in the Signals
#					chapter of Solaris Systems Programming.
#
#
#			Copyright © 2001-2004 by Rich Teer.  All rights reserved.
#
#*******************************************************************************

include ../Make.defines

TARGETS = abort.o altstack bad_handler critical pending print_proc_mask.o procmask \
	psignal run_sys setvar sigcld sigfpe siginfo siglongjmp signal sigset sigtstp \
	sleep_test ssignal ssp_kill ssp_signal.o ssp_sleep1.o ssp_sleep2.o ssp_sleep3.o \
	ssp_system.o system1 system2 tell_wait.o timeout1 timeout2 timeout3

all sol-2.5 sol-2.5.1 sol-2.6 sol-7 sol-8 sol-9: $(TARGETS)

clean:
	@rm -f $(TARGETS) $(CLEAN_TARGETS)

abort.o: abort.c
	$(CC) $(CFLAGS) -c abort.c

altstack: altstack.c $(SSP_LIB)
	$(CC) $(CFLAGS) -o $@ $@.c $(SSP_LIB)

bad_handler: bad_handler.c $(SSP_LIB)
	$(CC) $(CFLAGS) -o $@ $@.c $(SSP_LIB)

critical: critical.c $(SSP_LIB) print_proc_mask.o
	$(CC) $(CFLAGS) -o $@ $@.c $(SSP_LIB) print_proc_mask.o

pending: pending.c $(SSP_LIB)
	$(CC) $(CFLAGS) -o $@ $@.c $(SSP_LIB)

print_proc_mask.o: print_proc_mask.c
	$(CC) $(CFLAGS) -c print_proc_mask.c

procmask: procmask.c $(SSP_LIB)
	$(CC) $(CFLAGS) -o $@ $@.c $(SSP_LIB)

psignal: psignal.c $(SSP_LIB)
	$(CC) $(CFLAGS) -o $@ $@.c $(SSP_LIB)

run_sys: run_sys.c $(SSP_LIB)
	$(CC) $(CFLAGS) -o $@ $@.c $(SSP_LIB)

setvar: setvar.c $(SSP_LIB)
	$(CC) $(CFLAGS) -o $@ $@.c $(SSP_LIB)

sigcld: sigcld.c $(SSP_LIB)
	$(CC) $(CFLAGS) -o $@ $@.c $(SSP_LIB)

sigfpe: sigfpe.c $(SSP_LIB)
	$(CC) $(CFLAGS) -o $@ $@.c $(SSP_LIB)

siginfo: siginfo.c $(SSP_LIB)
	$(CC) $(CFLAGS) -o $@ $@.c $(SSP_LIB)

siglongjmp: siglongjmp.c $(SSP_LIB) print_proc_mask.o
	$(CC) $(CFLAGS) -o $@ $@.c $(SSP_LIB) print_proc_mask.o

signal: signal.c $(SSP_LIB)
	$(CC) $(CFLAGS) -o $@ $@.c $(SSP_LIB)

sigset: sigset.c $(SSP_LIB)
	$(CC) $(CFLAGS) -o $@ $@.c $(SSP_LIB)

sigtstp: sigtstp.c
	$(CC) $(CFLAGS) -o $@ $@.c

sleep_test: sleep_test.c $(SSP_LIB) ssp_sleep2.o
	$(CC) $(CFLAGS) -o $@ $@.c $(SSP_LIB) ssp_sleep2.o

ssignal: ssignal.c
	$(CC) $(CFLAGS) -o $@ $@.c

ssp_kill: ssp_kill.c $(SSP_LIB)
	$(CC) $(CFLAGS) -o $@ $@.c $(SSP_LIB)

ssp_signal.o: ssp_signal.c
	$(CC) $(CFLAGS) -c ssp_signal.c

ssp_sleep1.o: ssp_sleep1.c
	$(CC) $(CFLAGS) -c ssp_sleep1.c

ssp_sleep2.o: ssp_sleep2.c
	$(CC) $(CFLAGS) -c ssp_sleep2.c

ssp_sleep3.o: ssp_sleep3.c
	$(CC) $(CFLAGS) -c ssp_sleep3.c

ssp_system.o: ssp_system.c
	$(CC) $(CFLAGS) -c ssp_system.c

system1: system1.c $(SSP_LIB) ../proc_control/ssp_system.o
	$(CC) $(CFLAGS) -o $@ $@.c $(SSP_LIB) ../proc_control/ssp_system.o

../proc_control/ssp_system.o: ../proc_control/ssp_system.c
	$(CC) $(CFLAGS) -c -o $@ ../proc_control/ssp_system.c

system2: system2.c $(SSP_LIB) ssp_system.o
	$(CC) $(CFLAGS) -o $@ $@.c $(SSP_LIB) ssp_system.o

tell_wait.o: tell_wait.c $(SSP_LIB)
	$(CC) $(CFLAGS) -c -D_POSIX_PTHREAD_SEMANTICS tell_wait.c

timeout1: timeout1.c $(SSP_LIB)
	$(CC) $(CFLAGS) -o $@ $@.c $(SSP_LIB)

timeout2: timeout2.c $(SSP_LIB)
	$(CC) $(CFLAGS) -o $@ $@.c $(SSP_LIB)

timeout3: timeout3.c $(SSP_LIB)
	$(CC) $(CFLAGS) -o $@ $@.c $(SSP_LIB)
