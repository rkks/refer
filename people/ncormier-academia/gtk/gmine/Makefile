##
## Makefile for  in /u/ept2/cormie_n/lang/c/gmine
##
## Made by nicolas cormier
## Login   <cormie_n@epita.fr>
##
## Started on  Sun Feb  6 15:21:57 2005 nicolas cormier
## Last update Wed Mar  9 15:30:25 2005 nicolas cormier
##

NAME=		gmine
SRC=		flag.c \
		menu.c \
		open_game.c \
		text.c \
		buttons.c \
		game.c \
		mine.c \
		option.c \
		save_game.c \
		init_gtk.c \
		mine_init.c \
		option_valid.c \
		screen.c \
		undo.c \
		error.c \
		main.c \
		new_game.c \
		pixmap.c \
		table.c \
		unhide_game.c \
		timer.c
OBJ= 		$(SRC:.c=.o)
CFLAGS=		-Wall `gtk-config --cflags`
LIB=		`gtk-config --libs`
CC=		gcc
RM=		rm -f

all:		$(NAME)
$(NAME):	$(OBJ)
		$(CC) $(LIB) $(OBJ) -o $(NAME)
clean:
		$(RM) $(OBJ)
fclean:		clean
		$(RM) $(NAME)
re:		fclean all
