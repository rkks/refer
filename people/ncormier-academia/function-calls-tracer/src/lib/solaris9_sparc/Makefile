##
## Makefile for  in /home/nico/lang/c/ftrace/lib/freebsd_ia32
##
## Made by nicolas
## Mail   <n.cormier@gmail.com>
##
## Started on  Thu Mar 23 13:51:19 2006 nicolas
## Last update Wed Apr 12 21:29:45 2006 nicolas cormier
##

NAME		= libftrace_solaris9_sparc.a
SRC		= bin/low_layer.c bin/high_layer/bin.c bin/high_layer/debug.c \
		  bin/high_layer/symbols.c core/low_layer.c core/high_layer.c \
		  proc/low_layer.c proc/high_layer.c ../../api/types/prim_types.c
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
		rm -f ../libftrace.a
		cp libftrace_solaris9_sparc.a ../libftrace.a

all:		$(NAME)
clean:
		$(RM) $(OBJ)
fclean:		clean
		$(RM) $(NAME)
re:		fclean all

.c.o:
		$(CC) $(CFLAGS) -c $< -o $@
