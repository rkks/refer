##
## Makefile for  in /home/nico/lang/c/strace
##
## Made by nicolas
## Login   <nico@epita.fr>
##
##

NAME		= strace
SRC		= error.c  param.c  strace.c
OBJ		= $(SRC:.c=.o)
CC		= gcc
RM		= rm -f
CFLAGS		= -W -Wall -Wstrict-prototypes -Ios-depends
DEPENDS 	= os-depends/ioctl.c os-depends/syscalls.h #os-depends/ioctl/`uname`/ioctl.c os-depends/syscalls/`uname`/syscalls.h
IOCTL		= (cd os-depends/ioctl/`uname` && make && cp ioctl.c ../../.)
SYSCL		= (cd os-depends/syscalls/`uname` && make && cp syscalls.h ../../.)


####

all:		$(DEPENDS) $(NAME)

$(NAME):	$(OBJ)
		$(CC) -o $(NAME) $(OBJ)

$(OBJ):		strace.h os-depends/syscalls.h

clean:
		$(RM) $(OBJ)
		$(RM) $(DEPENDS)

fclean:		clean
		$(RM) $(NAME)

re:		fclean all

$(DEPENDS):
		$(IOCTL)
		$(SYSCL)
