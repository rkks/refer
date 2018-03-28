## Process this file with automake to produce Makefile.in

lib_LTLIBRARIES		= libhello.la
libhello_la_SOURCES	= hello.c
libhello_la_LDFLAGS	= -no-undefined -version-info 0:0:0

include_HEADERS		= hello.h

bin_PROGRAMS		= hello
hello_SOURCES		= main.c
hello_LDADD		= libhello.la

main.o: main.c
	$(COMPILE) @LIBHELLO_DLL_IMPORT@ -c main.c
