# Makefile for:  vis - repeatedly run command, refreshing screen

DEFINES		= -DBSD
CFLAGS		= -O $(DEFINES)
LIBS		= -lcurses -ltermcap
ROFF		= nroff
RFLAGS		= -man
LDFLAGS		= -s
DESTBIN		= ${HOME}
DESTMAN		= ${HOME}
DESTCAT		= ${HOME}
DESTOWN		= root
DESTGRP		= staff
SHELL		= /bin/sh
MAKEFILE	= Makefile
PROGRAM		= vis
MANPAGES	= vis.man
CATPAGES	= vis.cat
SRCS		= vis.c
OBJS		= vis.o

.man.cat:
		$(ROFF) $(RFLAGS) $< > $@

.SUFFIXES:	.man .cat

# Compile and load the program and format its manual pages.
all:		$(PROGRAM) $(CATPAGES)

$(PROGRAM):	$(OBJS)
		$(CC) $(LDFLAGS) $(OBJS) $(LIBS) -o $(PROGRAM)

# Run lint on source files, put results on standard output.
lint:
		lint -u $(DEFINES) $(SRCS)

# Create a tags file for use by a source code editor.
tags:		$(SRCS)
		ctags $(SRCS)

# Edit the makefile and regenerate the dependency information.
depend:
		mkmf -f $(MAKEFILE) PROGRAM=$(PROGRAM) DESTBIN=$(DESTBIN)

# Print an index of functions on standard output.
index:
		ctags -wx $(SRCS)

# Print source code files on standard output.
print:
		lpr -p $(SRCS) $(MANPAGES)

# Compile and load the program, format manual pages, and move them
# to their destination directories.
install:	$(PROGRAM) $(CATPAGES)
		cp $(PROGRAM) $(DESTBIN)
		chmod 755 $(DESTBIN)/$(PROGRAM)
		chown $(DESTOWN) $(DESTBIN)/$(PROGRAM)
		chgrp $(DESTGRP) $(DESTBIN)/$(PROGRAM)
		-for manpage in $(MANPAGES); do \
			basepage=`basename $$manpage .man`; \
			cp $$basepage.man $(DESTMAN)/$$basepage.l; \
			chmod 644 $(DESTMAN)/$$basepage.l; \
			chown $(DESTOWN) $(DESTMAN)/$$basepage.l; \
			chgrp $(DESTGRP) $(DESTMAN)/$$basepage.l; \
			cp $$basepage.cat $(DESTCAT)/$$basepage.l; \
			chmod 644 $(DESTCAT)/$$basepage.l; \
			chown $(DESTOWN) $(DESTCAT)/$$basepage.l; \
			chgrp $(DESTGRP) $(DESTCAT)/$$basepage.l; \
		done

# Remove the program and its formatted manual pages from their
# destination directories.
uninstall:
		rm -f $(DESTBIN)/$(PROGRAM)
		-for manpage in $(MANPAGES); do \
			basepage=`basename $$manpage .man`; \
			rm -f $(DESTMAN)/$$basepage.l; \
			rm -f $(DESTCAT)/$$basepage.l; \
		done

# Remove all target and intermediate files.
clean:
		-rm -f $(PROGRAM) $(CATPAGES) $(OBJS)
		-rm -f core a.out made *.o
