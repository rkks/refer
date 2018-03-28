##
## Makefile for  in /u/ept3/cormie_n/lang/c/backtrace
##
## Made by nicolas cormier
## Login   <cormie_n@epita.fr>
##
## Started on  Mon Jan 23 14:19:44 2006 nicolas cormier
## Last update Mon Jan 23 14:22:37 2006 nicolas cormier
##

NAME		= backtrace
SRC		= backtrace.c brkpoints.c elf.c error.c itrace.c ptrace.c stack.c
OBJ		= $(SRC:.c=.o)
CC		= gcc
RM		= rm -f
CFLAGS		= -Wall

$(NAME):	$(OBJ)
		$(CC) -o $(NAME) $(OBJ)
$(OBJ):		backtrace.h elffprot.h
all:		$(NAME)
clean:
		$(RM) $(OBJ)
fclean:		clean
		$(RM) $(NAME)
re:		fclean all
