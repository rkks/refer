CFLAGS=--ansi --pedantic -Wall



.PHONY : all clean regexp cmdline fork ipc__signals ipc__pipes ipc__nampipes ipc__sockets ipc__msgqueues ipc__semaphores

all: regexp cmdline fork threads ipc__signals ipc__pipes ipc__nampipes ipc__sockets ipc__msgqueues ipc__semaphores ipc__shm

clean:
	${MAKE} -C regexp		clean
	${MAKE} -C cmdline		clean
	${MAKE} -C fork			clean
	${MAKE} -C threads		clean
	${MAKE} -C ipc__signals		clean
	${MAKE} -C ipc__pipes		clean
	${MAKE} -C ipc__nampipes	clean
	${MAKE} -C ipc__sockets		clean
	${MAKE} -C ipc__msgqueues	clean
	${MAKE} -C ipc__semaphores	clean    
	${MAKE} -C ipc__shm		clean

regexp:
	${MAKE} -C regexp

cmdline:
	${MAKE} -C cmdline

fork:
	${MAKE} -C fork

ipc__signals:
	${MAKE} -C ipc__signals

ipc__pipes:
	${MAKE} -C ipc__pipes

ipc__nampipes:
	${MAKE} -C ipc__nampipes

ipc__sockets:
	${MAKE} -C ipc__sockets

ipc__msgqueues:
	${MAKE} -C ipc__msgqueues

ipc__semaphores:
	${MAKE} -C ipc__semaphores 

ipc__shm:
	${MAKE} -C ipc__shm
