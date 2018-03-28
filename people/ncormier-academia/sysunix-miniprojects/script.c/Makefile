##
## Makefile for  in /u/ept2/cormie_n/lang/c/script
##
## Made by nicolas cormier
## Login   <cormie_n@epita.fr>
##
## Started on  Wed Feb  2 15:36:14 2005 nicolas cormier
## Last update Sun Feb  6 14:29:50 2005 nicolas cormier
##

SRC= main.c util.c child.c end.c term.c
OBJ=$(SRC:.c=.o)
NAME=my_script
CFLAGS=-Wall -pedantic
LIB=
CC=cc
RM=rm -f

##

$(NAME):	$(OBJ)
		$(CC) $(OBJ) -o $(NAME) $(LIB)

all:		$(NAME)

clean:
		$(RM) *~
		$(RM) $(OBJ)

fclean:		clean
		$(RM) $(NAME)

re:		fclean all
