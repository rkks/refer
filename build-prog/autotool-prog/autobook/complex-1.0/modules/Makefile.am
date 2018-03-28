## @start 1
## Makefile.am -- Process this file with automake to produce Makefile.in
## @end 1
## Copyright (C) 2000 Gary V. Vaughan
##
## This program is free software; you can redistribute it and/or modify
## it under the terms of the GNU General Public License as published by
## the Free Software Foundation; either version 2, or (at your option)
## any later version.
##
## This program is distributed in the hope that it will be useful,
## but WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
## GNU General Public License for more details.
##
## You should have received a copy of the GNU General Public License
## along with this program; if not, write to the Free Software
## Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

MAINTAINERCLEANFILES = Makefile.in

## @start 1
INCLUDES        = -I$(top_builddir) -I$(top_srcdir) \
                -I$(top_builddir)/sic -I$(top_srcdir)/sic \
                -I$(top_builddir)/src -I$(top_srcdir)/src

pkglib_LTLIBRARIES = unknown.la

## @end 1
pkglib_LTLIBRARIES += cd.la dollar.la eval.la sqrt.la

cd_la_SOURCES = cd.c
cd_la_LDFLAGS = -no-undefined -module -avoid-version
cd_la_LIBADD  = $(top_builddir)/sic/libsic.la

dollar_la_SOURCES = dollar.c
dollar_la_LDFLAGS = -no-undefined -module -avoid-version
dollar_la_LIBADD  = $(top_builddir)/sic/libsic.la

eval_la_SOURCES = eval.c
eval_la_LDFLAGS = -no-undefined -module -avoid-version
eval_la_LIBADD  = $(top_builddir)/sic/libsic.la

sqrt_la_SOURCES = sqrt.c
sqrt_la_LDFLAGS = -no-undefined -module -avoid-version
sqrt_la_LIBADD  = $(top_builddir)/sic/libsic.la -lm

## @start 2
unknown_la_SOURCES = unknown.c
unknown_la_LDFLAGS = -no-undefined -module -avoid-version
unknown_la_LIBADD  = $(top_builddir)/sic/libsic.la
## @end 2
