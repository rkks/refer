#*******************************************************************************
#
#	Name:			Makefile
#
#	Description:	This is the Makefile for the Solaris Systems Programming
#					library.
#
#
#			Copyright © 2001-2004 by Rich Teer.  All rights reserved.
#
#*******************************************************************************

include ../Make.defines

CFLAGS += -D_REENTERANT

TARGETS = ssp.a

S26_OBJS = error.o file_flags.o lock.o read_write.o term_status.o

S25_OBJS = error.o file_flags.o lock.o read_write.o term_status.o snprintf.o

# Comment the following line ONLY if you are building on
# Solaris 2.5 or Solaris 2.5.1.
OBJS = $(S26_OBJS)

# Uncomment the following line ONLY if you are building on
# Solaris 2.5 or Solaris 2.5.1.
# OBJS = $(S25_OBJS)

all sol-2.5 sol-2.5.1 sol-2.6 sol-7 sol-8 sol-9: $(TARGETS)

clean:
	@rm -f $(TARGETS) $(CLEAN_TARGETS)

ssp.a: $(OBJS)
	ar -r $@ $(OBJS)
