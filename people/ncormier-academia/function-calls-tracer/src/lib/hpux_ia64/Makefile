##
## Makefile for  in /home/nico/lang/c/ftrace/lib/freebsd_ia32
## 
## Made by nicolas
## Mail   <n.cormier@gmail.com>
## 
## Started on  Thu Mar 23 13:51:19 2006 nicolas
## Last update Wed Apr 26 16:27:37 2006 julieN barbot
##

NAME		= libftrace_hpux_ia64.a
SRC		= proc/low_layer.c proc/high_layer.c ../../api/types/prim_types.c
OBJ		= $(SRC:.c=.o)
CC		= gcc
AR		= ar cru 
RM		= rm -f
RANLIB		= ranlib
CFLAGS		= -I../../. -Iincludes -Wall -W -Wstrict-prototypes -g

$(NAME):	$(OBJ)
		@echo "run ar"
		$(AR) $(NAME) $(OBJ)
		@echo "run ranlib"
		$(RANLIB) $(NAME)
		@echo "copy lib"
		rm -f ../libftrace.a
		cp $(NAME) ../libftrace.a

all:		$(NAME)
clean:
		$(RM) $(OBJ)
fclean:		clean
		$(RM) $(NAME)
re:		fclean all

.c.o:
		$(CC) $(CFLAGS) -c $< -o $@
