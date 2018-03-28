## @start 1
## Process this file with automake to produce Makefile.in

SUBDIRS                 = replace lib .

CPPFLAGS                = -I$(top_srcdir)/lib

include_HEADERS         = error.h

lib_LTLIBRARIES         = liberror.la
liberror_la_SOURCES     = error.c
liberror_la_LDFLAGS     = -no-undefined -version-info 0:0:0
liberror_la_LIBADD      = replace/libreplace.la lib/library.la

bin_PROGRAMS            = convenience
convenience_SOURCES     = main.c
convenience_LDADD       = liberror.la
## @end 1
