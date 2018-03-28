NAME		= mkernel
SRC		= ./main.c  \
		  ../arch/x86/ia32_addr_space_def.c \
		  ../services/console/console.c \
		  ./memory/_impl/MemoryHAL.c \
		  ./memory/_impl/PhysicalMemoryManager.c \
		  ./memory/_impl/KernelMemoryManager.c \
		  ./memory/_impl/PhysicalPage.c
INCLUDES	= 
BIN_OUTDIR	= ../bin/
OBJ	     	= $(SRC:.c=.o)
OBJ_OUTDIR	= ${BIN_OUTDIR}/obj/
CC	      	= gcc
LD		= ld
RM	      	= rm -f
INCLUDES_PATH	= -I. -I./.. -I./../services
LDFLAGS		= -T ../compilation_tools/ia32_kernel_link.ld
CFLAGS	  	= -O0 -pedantic -Wall -Werror -std=c99 -fno-builtin -nostdlib -ffreestanding $(INCLUDES_PATH)

re:		clean all

$(NAME):	kernel_entry $(OBJ) 
		@echo $(OBJ)
		$(LD) $(LDFLAGS) -o ${BIN_OUTDIR}kernel.bin ./kernel.o $(OBJ)
		chmod -x ${BIN_OUTDIR}kernel.bin
		cat ${BIN_OUTDIR}boot.bin ${BIN_OUTDIR}kernel.bin > ${BIN_OUTDIR}/${NAME}.bin 
		@ls -l ${BIN_OUTDIR}boot.bin ${BIN_OUTDIR}kernel.bin ${BIN_OUTDIR}/${NAME}.bin
		@echo 'check bootstrap + bootloader mapping + compilation tools kernel'
#		$(RM) ${BIN_OUTDIR}boot.bin ${BIN_OUTDIR}kernel.bin
		$(RM) ./kernel.o $(OBJ)

$(OBJ):		$(INCLUDES)

all:		$(NAME)

clean:
		$(RM) $(OBJ)

kernel_entry:
		nasm -f elf kernel.asm -o ./kernel.o
.c.o:
		$(CC) $(CFLAGS) -c $< -o $@
