## Makefile.am -- Process this file with automake to produce Makefile.in
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

EXTRA_DIST		= defs boilerplate.test $(TESTS)
CLEANFILES		= *.exp *.out
MAINTAINERCLEANFILES	= Makefile.in

testsubdir		= testSubDir

TESTS_ENVIRONMENT 	= top_builddir=$(top_builddir)

TESTS                   = 			\
			builtin-unload.test	\
			cmd-subst.test 		\
			cmd-subst-2.test	\
			empty-eval.test		\
			empty-eval-2.test	\
			empty-eval-3.test	\
			incomplete.test		\
			multicmd.test		\
			multicmd-subst.test	\
			syntax-unload.test	\
			syntax-unload-2.test	\
			var-subst.test		\
			var-subst-2.test

distclean-local:
	-rm -rf $(testsubdir)
