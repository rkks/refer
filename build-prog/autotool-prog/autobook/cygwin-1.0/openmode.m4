# openmode.m4 --- differentiate text and binary modes where possible
# --------------------------------------------------------------------
# Author:           Gary V. Vaughan <gvv@techie.com>
# Maintainer:       Gary V. Vaughan <gvv@techie.com>
# Created:          Tue Nov 24 01:07:30 1998
# Last Modified:    Thu Jun 22 00:24:37 2000
#            by:    Gary V. Vaughan <gvv@techie.com>
# --------------------------------------------------------------------
# @(#) $Id: openmode.m4,v 1.11 2000/07/07 20:27:00 gvv Exp $
# --------------------------------------------------------------------
#
# Code:

# serial 7

# @start 1
# _AB_AC_FUNC_FOPEN(b | t, USE_FOPEN_BINARY | USE_FOPEN_TEXT)
# -----------------------------------------------------------
define([_AB_AC_FUNC_FOPEN],
[AC_CACHE_CHECK([whether fopen accepts "$1" mode], [ab_cv_func_fopen_$1],
[AC_TRY_RUN([#include <stdio.h>
int
main ()
{
   FILE *fp = fopen ("conftest.bin", "w$1");
   fprintf (fp, "\n");
   fclose (fp);
   return 0;
}],
            [ab_cv_func_fopen_$1=yes],
            [ab_cv_func_fopen_$1=no],
            [ab_cv_func_fopen_$1=no])])
if test x$ab_cv_func_fopen_$1 = xyes; then
  AC_DEFINE([$2], 1,
	    [Define this if we can use the "$1" mode for fopen safely.])
fi[]dnl
])# _AB_AC_FUNC_FOPEN

# AB_AC_FUNC_FOPEN_BINARY
# -----------------------
# Test whether fopen accepts a "" in the mode string for binary file
# opening.  This makes no difference on most unices, but some OSes
# convert every newline written to a file to two bytes (CR LF), and
# every CR LF read from a file is silently converted to a newline.
AC_DEFUN([AB_AC_FUNC_FOPEN_BINARY], [_AB_AC_FUNC_FOPEN(b, USE_FOPEN_BINARY)])

# AB_AC_FUNC_FOPEN_TEXT
# ---------------------
# Test whether open accepts a "t" in the mode string for text file
# opening.  This makes no difference on most unices, but other OSes
# use it to assert that every newline written to a file writes two
# bytes (CR LF), and every CR LF read from a file are silently
# converted to a newline.
AC_DEFUN([AB_AC_FUNC_FOPEN_TEXT],   [_AB_AC_FUNC_FOPEN(t, USE_FOPEN_TEXT)])


# _AB_AC_FUNC_OPEN(O_BINARY|O_TEXT)
# ---------------------------------
AC_DEFUN([_AB_AC_FUNC_OPEN],
[AC_CACHE_CHECK([whether fcntl.h defines $1], [ab_cv_header_fcntl_h_$1],
[AC_EGREP_CPP([$1],
              [#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
$1
],
              [ab_cv_header_fcntl_h_$1=no],
              [ab_cv_header_fcntl_h_$1=yes])
if test "x$ab_cv_header_fcntl_h_$1" = xno; then
  AC_EGREP_CPP([_$1],
               [#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
_$1
],
                [ab_cv_header_fcntl_h_$1=0],
                [ab_cv_header_fcntl_h_$1=_$1])
fi])
if test "x$ab_cv_header_fcntl_h_$1" != xyes; then
  AC_DEFINE_UNQUOTED([$1], [$ab_cv_header_fcntl_h_$1],
    [Define this to a usable value if the system provides none])
fi[]dnl
])# _AB_AC_FUNC_OPEN


# AB_AC_FUNC_OPEN_BINARY
# ----------------------
# Test whether open accepts O_BINARY in the mode string for binary
# file opening.  This makes no difference on most unices, but some
# OSes convert every newline written to a file to two bytes (CR LF),
# and every CR LF read from a file is silently converted to a newline.
#
# @end 1
# There are three possible outcomes (in config.h) depending on which
# macros are defined by the system headers:
#
# O_BINARY  _O_BINARY  result
# --------  ---------  ------
# undefined undefined  #define O_BINARY 0
# undefined   defined  #define O_BINARY _O_BINARY
#   defined   either   /* #undef O_BINARY */
#
# @start 1
AC_DEFUN([AB_AC_FUNC_OPEN_BINARY], [_AB_AC_FUNC_OPEN([O_BINARY])])


# AB_AC_FUNC_OPEN_TEXT
# --------------------
# Test whether open accepts O_TEXT in the mode string for text file
# opening.  This makes no difference on most unices, but other OSes
# use it to assert that every newline written to a file writes two
# bytes (CR LF), and every CR LF read from a file are silently
# converted to a newline.
#
# @end 1
# There are three possible outcomes (in config.h) depending on which
# macros are defined by the system headers:
#
# O_TEXT    _O_TEXT    result
# --------  ---------  ------
# undefined undefined  #define O_TEXT 0
# undefined   defined  #define O_TEXT _O_TEXT
#   defined   either   /* #undef O_TEXT */
#
# @start 1
AC_DEFUN([AB_AC_FUNC_OPEN_TEXT],   [_AB_AC_FUNC_OPEN([O_TEXT])])

# @end 1
# openmode.m4 ends here
