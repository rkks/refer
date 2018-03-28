##
## Makefile for ktrace in /home/mat/dev/ktrace-kdump
##
## Made by fourni_m
## Login   <mat@epita.fr>
##
## Started on  Fri Oct 21 10:43:55 2005 fourni_m
## Last update Wed Nov  2 14:33:47 2005 nicolas cormier
##

NAME	= kdump
SRC	= dump_func.c kdump.c opt.c timeval.c

OBJ	= $(SRC:.c=.o)


CC	= gcc
RM	= rm -f
CFLAGS	= -O3 -Wall

####

$(NAME):	$(OBJ)
		$(CC) -o $(NAME) $(OBJ)

$(OBJ):		kdump.h 

all:		$(NAME)

clean:
		$(RM) $(OBJ)

fclean:		clean
		$(RM) $(NAME)

re:		fclean all
