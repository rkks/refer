#*******************************************************************************
#
#	Name:			Makefile
#
#	Description:	This is the Makefile for the Solaris Systems Programming
#					example source code.
#
#
#			Copyright © 2001-2004 by Rich Teer.  All rights reserved.
#
#*******************************************************************************

CHAPTERS = lib advanced_io advanced_ipc daemons date_and_time doors file_io \
	file_systems files_and_dirs i18n_and_l10n intro ipc pipes_and_fifos \
	proc_control proc_env proc_rels pseudo_terms secure_c signals solutions \
	standard_io sys_info terminal_io users_and_groups utils

all sol-2.5 sol-2.5.1 sol-2.6 sol-7 sol-8 sol-9:
	@for i in $(CHAPTERS); do \
		echo Making all in $${i}; \
		cd $${i}; \
		if [ "$${VERBOSE}" != "y" ]; then \
			make -s $@; \
		else \
			make $@; \
		fi; \
		cd ..; \
	done

clean:
	@for i in $(CHAPTERS); do \
		echo Cleaning $${i}; \
		cd $${i}; \
		if [ "$${VERBOSE}" != "y" ]; then \
			make -s clean; \
		else \
			make clean; \
		fi; \
		cd ..; \
	done

#
# Tribute to a long-forgotten hack...
#
love:
	@echo Not war?
