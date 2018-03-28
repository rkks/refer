#*******************************************************************************
#
#	Name:			Makefile
#
#	Description:	This is the Makefile for the example in the System Information
#					and Resource Limits chapter of Solaris Systems Programming.
#
#
#			Copyright © 2001-2004 by Rich Teer.  All rights reserved.
#
#*******************************************************************************

include ../Make.defines

S9_TARGETS = getpagesizes getprusage1 getprusage2 getrlimit loadav pathconf \
	print_rctl.o rctl_walk setrctl ssp_time sysconf sysinfo

S7_TARGETS = getprusage1 getprusage2 getrlimit loadav pathconf ssp_time sysconf \
	sysinfo

S26_TARGETS = getprusage1 getprusage2 getrlimit pathconf ssp_time sysconf sysinfo

S25_TARGETS = getprusage2 getrlimit ssp_time sysconf sysinfo

TARGETS = $(S9_TARGETS)

all sol-9: $(TARGETS)

sol-7 sol-8: $(S7_TARGETS)

sol-2.6: $(S26_TARGETS)

sol-2.5 sol-2.5.1: $(S25_TARGETS)

clean:
	@rm -f $(TARGETS) $(CLEAN_TARGETS)

getpagesizes: getpagesizes.c $(SSP_LIB)
	$(CC) $(CFLAGS) -o $@ $@.c $(SSP_LIB)

getprusage1: getprusage1.c $(SSP_LIB)
	$(CC) $(CFLAGS) -o $@ $@.c $(SSP_LIB)

getprusage2: getprusage2.c $(SSP_LIB)
	$(CC) $(CFLAGS) -o $@ $@.c $(SSP_LIB)

getrlimit: getrlimit.c $(SSP_LIB)
	$(CC) $(CFLAGS) -o $@ $@.c $(SSP_LIB)

loadav: loadav.c $(SSP_LIB)
	$(CC) $(CFLAGS) -o $@ $@.c $(SSP_LIB)

pathconf: pathconf.c $(SSP_LIB)
	$(CC) $(CFLAGS) -o $@ $@.c $(SSP_LIB)

print_rctl.o: print_rctl.c
	$(CC) $(CFLAGS) -c print_rctl.c

rctl_walk: rctl_walk.c $(SSP_LIB) print_rctl.o
	$(CC) $(CFLAGS) -o $@ $@.c $(SSP_LIB) print_rctl.o

setrctl: setrctl.c $(SSP_LIB) print_rctl.o
	$(CC) $(CFLAGS) -o $@ $@.c $(SSP_LIB) print_rctl.o

ssp_time: ssp_time.c $(SSP_LIB)
	$(CC) $(CFLAGS) -o $@ $@.c $(SSP_LIB)

sysconf: sysconf.c $(SSP_LIB)
	$(CC) $(CFLAGS) -o $@ $@.c $(SSP_LIB)

sysinfo: sysinfo.c $(SSP_LIB)
	$(CC) $(CFLAGS) -o $@ $@.c $(SSP_LIB)
