NAME		= boot
SRC		= ./main.c \
		  ../../arch/x86/ia32_addr_space_def.c \
		  ../../arch/x86/x86_paging.c \
		  ../../services/console/console.c
INCLUDES	= 
BIN_OUTDIR	= ./../../bin/
OBJ	     	= $(SRC:.c=.o)
OBJ_OUTDIR	= ${BIN_OUTDIR}
CC	      	= gcc
LD		= ld
RM	      	= rm -f
INCLUDES_PATH	= -I. -I../.. -I./../../services
LDFLAGS		= -T ../../compilation_tools/ia32_bootloader_link.ld
CFLAGS	  	= -O0 -pedantic -Wall -Werror -fomit-frame-pointer -std=c99 -fno-builtin -nostdlib -ffreestanding $(INCLUDES_PATH)

re:		clean all

$(NAME):	bootstrap $(OBJ) 
		@echo $(OBJ)
		$(LD) $(LDFLAGS) -o ${BIN_OUTDIR}/bootloader.bin ./bootloader.o $(OBJ)
		chmod -x ${BIN_OUTDIR}/bootloader.bin
		cat ${BIN_OUTDIR}bootstrap.bin ${BIN_OUTDIR}/bootloader.bin > ${BIN_OUTDIR}/${NAME}.bin 
		rm -f ${BIN_OUTDIR}/bootstrap.bin ${BIN_OUTDIR}/bootloader.bin
		$(RM) ./bootloader.o
		$(RM) $(OBJ)


$(OBJ):		$(INCLUDES)

all:		$(NAME)

clean:
		$(RM) ./bootloader.o
		$(RM) $(OBJ)

bootstrap:
		nasm -f bin bootstrap.asm -o ${BIN_OUTDIR}/bootstrap.bin
		nasm -f elf bootloader.asm -o ./bootloader.o

.c.o:
		$(CC) $(CFLAGS) -c $< -o $@
