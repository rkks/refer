##
## Makefile for ktrace in /home/mat/dev/ktrace-kdump
##
## Made by fourni_m
## Login   <mat@epita.fr>
##
## Started on  Fri Oct 21 10:43:55 2005 fourni_m
## Last update Mon Nov 21 22:32:52 2005 nicolas cormier
##

NAME	= t3-lsof
SRC	= lsof.c  nfs.c  ufs.c  vdev.c  vdoor.c  vfifo.c  vfs_conf.c  vsock.c

OBJ	= $(SRC:.c=.o)


CC	= gcc
RM	= rm -f
CFLAGS	= -O3 -Wall -pedantic -Wstrict-prototypes -m64

####

$(NAME):	$(OBJ)
		$(CC) -o $(NAME) $(OBJ) -lkvm -m64

$(OBJ):		lsof.h

all:		$(NAME)

clean:
		$(RM) $(OBJ)

fclean:		clean
		$(RM) $(NAME)

re:		fclean all
