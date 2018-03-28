##
## Makefile for  in /home/nico/ftrace
##
## Made by nicolas
## Mail   <n.cormier@gmail.com>
##
## Started on  Wed Apr 26 17:05:46 2006 nicolas
## Last update Wed Apr 26 19:53:02 2006 nicolas
##

SHELL = /bin/sh
NAME=	ftrace
UNAME=	$(shell uname)


$(NAME):	$(UNAME)
		(cp src/ftrace .)
FreeBSD:
		(cd src/lib/freebsd_ia32/ && make)
		(cd src && make -f Makefile.bsd)

SunOS:
		(cd src/lib/solaris9_sparc/ && make)
		(cd src && make -f Makefile.sunos)

OSF1:
		(cd src/lib/tru64_alpha/ && make)
		(cd src && make -f Makefile.alpha)


clean:
		(cd src/lib/freebsd_ia32/ && make clean)
		(cd src/lib/solaris9_sparc/ && make clean)
		(cd src/lib/tru64_alpha/ && make clean)
		(cd src && make -f Makefile.bsd clean)

fclean:
		(cd src/lib/freebsd_ia32/ && make fclean)
		(cd src/lib/solaris9_sparc/ && make fclean)
		(cd src/lib/tru64_alpha/ && make fclean)
		(cd src && make -f Makefile.bsd fclean)
		rm -f ./ftrace

re:		fclean $(NAME)
