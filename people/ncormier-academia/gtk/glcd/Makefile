##
## Makefile for  in /u/ept2/cormie_n/lang/c/glcd
##
## Made by nicolas cormier
## Login   <cormie_n@epita.fr>
##
## Started on  Tue Mar 22 14:33:25 2005 nicolas cormier
## Last update Wed Mar 23 23:43:52 2005 nicolas
##

NAME_A	=	glcd
SRC_A	=	gtk-lcd-banner.c \
		gtk-lcd.c \
		glcd.c
OBJ_A	=	$(SRC_A:.c=.o)

NAME_B	=	glcd-banner
SRC_B	=	gtk-lcd-banner.c \
		gtk-lcd.c \
		glcd-banner.c
OBJ_B	=	$(SRC_B:.c=.o)

NAME_C	=	glcd-dclock
SRC_C	=	gtk-dclock.c \
		gtk-lcd.c \
		glcd-dclock.c
OBJ_C	=	$(SRC_C:.c=.o)


LIB	=	`gtk-config --libs`
CFLAGS	=	-Wall `gtk-config --cflags`
CC	=	gcc
RM	=	rm -f
##
all:		$(NAME_A) $(NAME_B) $(NAME_C)
$(NAME_A):	$(OBJ_A)
		$(CC) $(LIB) $(OBJ_A) -o $(NAME_A)
$(NAME_B):	$(OBJ_B)
		$(CC) $(LIB) $(OBJ_B) -o $(NAME_B)
$(NAME_C):	$(OBJ_C)
		$(CC) $(LIB) $(OBJ_C) -o $(NAME_C)
clean:
		$(RM) $(OBJ_A) $(OBJ_B) $(OBJ_C)
fclean:		clean
		$(RM) $(NAME_A) $(NAME_B) $(NAME_C)
re:		fclean all

