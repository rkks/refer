#*******************************************************************************
#
#	Name:			Makefile
#
#	Description:	This is the Makefile for the example in the Utility
#					Functions chapter of Solaris Systems Programming.
#
#
#			Copyright © 2001-2004 by Rich Teer.  All rights reserved.
#
#*******************************************************************************

include ../Make.defines

S8_TARGETS = assert bubble_sort1 bubble_sort2 bufsplit char_class getopt highlight.o \
	malloc rot13 strchr strlcat strlcpy strlen strpbrk strrchr strrspn strstr strtok \
	strtok_r strtol

S25_TARGETS = assert bubble_sort1 bubble_sort2 bufsplit char_class getopt highlight.o \
	malloc rot13 strchr strlen strpbrk strrchr strrspn strstr strtok strtok_r strtol

TARGETS = $(S8_TARGETS)

all sol-8 sol-9: $(TARGETS)

sol-2.5 sol-2.5.1 sol-2.6 sol-7: $(S25_TARGETS)

clean:
	@rm -f $(TARGETS) $(CLEAN_TARGETS)

assert: assert.c
	$(CC) $(CFLAGS) -o $@ $@.c

bubble_sort1: bubble_sort1.c
	$(CC) $(CFLAGS) -o $@ $@.c

bubble_sort2: bubble_sort2.c
	$(CC) $(CFLAGS) -o $@ $@.c

bufsplit: bufsplit.c $(SSP_LIB)
	$(CC) $(CFLAGS) -o $@ $@.c $(SSP_LIB) -lgen

char_class: char_class.c
	$(CC) $(CFLAGS) -o $@ $@.c

getopt: getopt.c $(SSP_LIB)
	$(CC) $(CFLAGS) -o $@ $@.c $(SSP_LIB)

highlight.o: highlight.c
	$(CC) $(CFLAGS) -c highlight.c

malloc: malloc.c
	$(CC) $(CFLAGS) -o $@ $@.c $(SSP_LIB)

rot13: rot13.c $(SSP_LIB)
	$(CC) $(CFLAGS) -o $@ $@.c $(SSP_LIB) -lgen

strchr: strchr.c $(SSP_LIB) highlight.o
	$(CC) $(CFLAGS) -o $@ $@.c $(SSP_LIB) highlight.o

strlcat: strlcat.c $(SSP_LIB)
	$(CC) $(CFLAGS) -o $@ $@.c $(SSP_LIB)

strlcpy: strlcpy.c $(SSP_LIB)
	$(CC) $(CFLAGS) -o $@ $@.c $(SSP_LIB)

strlen: strlen.c
	$(CC) $(CFLAGS) -o $@ $@.c

strpbrk: strpbrk.c $(SSP_LIB) highlight.o
	$(CC) $(CFLAGS) -o $@ $@.c $(SSP_LIB) highlight.o

strrchr: strrchr.c $(SSP_LIB) highlight.o
	$(CC) $(CFLAGS) -o $@ $@.c $(SSP_LIB) highlight.o

strrspn: strrspn.c $(SSP_LIB) highlight.o
	$(CC) $(CFLAGS) -o $@ $@.c $(SSP_LIB) highlight.o -lgen

strstr: strstr.c $(SSP_LIB) highlight.o
	$(CC) $(CFLAGS) -o $@ $@.c $(SSP_LIB) highlight.o

strtok: strtok.c $(SSP_LIB)
	$(CC) $(CFLAGS) -o $@ $@.c $(SSP_LIB)

strtok_r: strtok_r.c $(SSP_LIB)
	$(CC) $(CFLAGS) -o $@ $@.c $(SSP_LIB)

strtol: strtol.c $(SSP_LIB)
	$(CC) $(CFLAGS) -o $@ $@.c $(SSP_LIB)
