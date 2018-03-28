#
# Makefile for example programs in Chapter 2 of "Systems Programming
# for SVR4-Based UNIX Systems" by David A. Curry.
#

#
# Pull in os-specific definitions.
#
include Makedefs.$(OSNAME)

#
# Programs to build.
#
# STD have all their functions defined in the standard C library.
# GEN have some of their functions defined in the -lgen library.
#
STD=	bsort-alpha bsort-length bsort-malloc caseconv make-a-list \
	parse-cmdline search-char search-charset search-string \
	search-token

GEN=	translate

#
# Build all the programs.  To build a single program, issue the command
# "../build-examples name-of-program".
#
all:	$(STD) $(GEN)

$(STD):	$$@.o iosubs.o markline.o
	$(CC) -o $@ $@.o iosubs.o markline.o

$(GEN): $$@.o iosubs.o markline.o
	$(CC) -o $@ $@.o iosubs.o markline.o $(GENLIBS)

#
# Issue the command "../build-examples clean" to get rid of object files,
# core files, and executables.
#
clean:
	rm -f $(STD) $(GEN) a.out core *.o \#*

#
# Specific dependencies for building each object.
#
bsort-alpha.o:		common/bsort-alpha.c
	$(CC) $(CFLAGS) -c -o $@ $?
bsort-length.o:		common/bsort-length.c
	$(CC) $(CFLAGS) -c -o $@ $?
bsort-malloc.o:		common/bsort-malloc.c
	$(CC) $(CFLAGS) -c -o $@ $?
caseconv.o:		common/caseconv.c
	$(CC) $(CFLAGS) -c -o $@ $?
iosubs.o:		common/iosubs.c
	$(CC) $(CFLAGS) -c -o $@ $?
make-a-list.o:		common/make-a-list.c
	$(CC) $(CFLAGS) -c -o $@ $?
markline.o:		common/markline.c
	$(CC) $(CFLAGS) -c -o $@ $?
parse-cmdline.o:	common/parse-cmdline.c
	$(CC) $(CFLAGS) -c -o $@ $?
search-char.o:		common/search-char.c
	$(CC) $(CFLAGS) -c -o $@ $?
search-charset.o:	common/search-charset.c
	$(CC) $(CFLAGS) -c -o $@ $?
search-string.o:	common/search-string.c
	$(CC) $(CFLAGS) -c -o $@ $?
search-token.o:		common/search-token.c
	$(CC) $(CFLAGS) -c -o $@ $?
translate.o:		$(OSNAME)/translate.c
	$(CC) $(CFLAGS) -c -o $@ $?
