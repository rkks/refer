# Do not edit this makefile. It was generated automatically 
# with commands like
#
#     $ AUPSRC=/aup # (for example)
#     $ export AUPSRC
#     $ awk -f $AUPSRC/makebuild.awk $AUPSRC/c1/makebuild.spec >$AUPSRC/c1/Makefile
#
# It is incomplete in the sense that there are no header-file
# dependencies.
#
# Use the shell-procedure makebuild to run awk. Then compile
# with a shell-procedure like this one for Linux:
#
# 	AUPSRC=/aup # (for example)
# 	OS=LINUX
# 	LIBS="-lncurses -lutil"
# 	TLIBS=-pthread
# 	export OS LIBS TLIBS
# 	make $2 -f $AUPSRC/$1/Makefile $3
#

defaulttarget: all
# Following must be defined in the environment before you run make:
#
#	OS = SOLARIS or LINUX or FREEBSD
#	LIBS = regular (non-thread) libraries (eg, "-lncurses -lutil")
#	TLIBS = thread libraries (eg, "-llthread -llgcc_r")
#	PTHREADSTUB = stub for thread functions (eg, pthreadstub.o), if needed
#	INCLUDES = additional include directory (eg, "-I/usr/local/include/pthread/linuxthreads")
#	Don't forget:
#		export OS LIBS TLIBS PTHREADSTUB INCLUDES
#	Suggestions:
#		Solaris:
#			OS=SOLARIS
#			LIBS="-lrt -lcurses -lsocket -lnsl"
#			TLIBS="-lpthread"
#
#		FreeBSD:
#			OS=FREEBSD
#			LIBS="-lncurses -lutil"
#			TLIBS="-llthread -llgcc_r"
#			PTHREADSTUB=pthreadstub.o
#			INCLUDES="-I/usr/local/include/pthread/linuxthreads"
#
#		Linux:
#			OS=LINUX
#			LIBS="-lncurses -lutil"
#			TLIBS="-pthread"
CFLAGS = $(INCLUDES) -I$(AUPSRC)/include -I$(AUPSRC)/common -I$(AUPSRC)/c6 -Wimplicit -Wstrict-prototypes -Wall -D$(OS) -D_REENTRANT -D_THREAD_SAFE
CC = gcc
# Same objects are used all the time, sometimes linking in unnecessary files.
AUPOBJS = syserr.o time.o extio.o getblksize.o setblock.o ec.o logf.o options.o macrostr.o

mms: mkmacrostr.o $(AUPOBJS) $(PTHREADSTUB)
	$(CC) -o mms mkmacrostr.o $(AUPOBJS) $(LIBS) $(PTHREADSTUB)
#############################################
# Utility to build macro-string program
#############################################
macrostr.o: $(AUPSRC)/common/macrostr.c $(AUPSRC)/common/macrostr.incl
	$(CC) $(CFLAGS) -c  -o $*.o $(AUPSRC)/common/macrostr.c

mkmacrostr.o: /aup/common/mkmacrostr.c
	$(CC) $(CFLAGS)  -c -o mkmacrostr.o /aup/common/mkmacrostr.c

options.o: /aup/common/options.c
	$(CC) $(CFLAGS)  -c -o options.o /aup/common/options.c

pthreadstub.o: /aup/common/pthreadstub.c
	$(CC) $(CFLAGS)  -c -o pthreadstub.o /aup/common/pthreadstub.c

ec.o: /aup/common/ec.c
	$(CC) $(CFLAGS)  -c -o ec.o /aup/common/ec.c

logf.o: /aup/common/logf.c
	$(CC) $(CFLAGS)  -c -o logf.o /aup/common/logf.c

setblock.o: /aup/c4/setblock.c
	$(CC) $(CFLAGS)  -c -o setblock.o /aup/c4/setblock.c

extio.o: /aup/c2/extio.c
	$(CC) $(CFLAGS)  -c -o extio.o /aup/c2/extio.c

syserr.o: /aup/c1/syserr.c
	$(CC) $(CFLAGS)  -c -o syserr.o /aup/c1/syserr.c

time.o: /aup/c1/time.c
	$(CC) $(CFLAGS)  -c -o time.o /aup/c1/time.c

getblksize.o: /aup/c2/getblksize.c
	$(CC) $(CFLAGS)  -c -o getblksize.o /aup/c2/getblksize.c

f1: f1.o $(AUPOBJS) $(PTHREADSTUB)
	$(CC) -o f1 f1.o $(AUPOBJS) $(LIBS) $(PTHREADSTUB)

f2: f2.o SimpleSemSysV.o $(AUPOBJS) $(PTHREADSTUB)
	$(CC) -o f2 f2.o SimpleSemSysV.o $(AUPOBJS) $(LIBS) $(PTHREADSTUB)

f3: f3.o SimpleSemSysV.o $(AUPOBJS) $(PTHREADSTUB)
	$(CC) -o f3 f3.o SimpleSemSysV.o $(AUPOBJS) $(LIBS) $(PTHREADSTUB)

ipcobjs: ipcobjs.o $(AUPOBJS) $(PTHREADSTUB)
	$(CC) -o ipcobjs ipcobjs.o $(AUPOBJS) $(LIBS) $(PTHREADSTUB)

lockftest: lockftest.o $(AUPOBJS) $(PTHREADSTUB)
	$(CC) -o lockftest lockftest.o $(AUPOBJS) $(LIBS) $(PTHREADSTUB)

mt: msg_time.o pcsync_sig.o smi_fifo.o smi_mq.o smi_msg.o smi_skt.o ssi.o smi_shm.o smi_pshm.o $(AUPOBJS) $(PTHREADSTUB)
	$(CC) -o mt msg_time.o pcsync_sig.o smi_fifo.o smi_mq.o smi_msg.o smi_skt.o ssi.o smi_shm.o smi_pshm.o $(AUPOBJS) $(LIBS) $(PTHREADSTUB)

shmex: shmex.o $(AUPOBJS) $(PTHREADSTUB)
	$(CC) -o shmex shmex.o $(AUPOBJS) $(LIBS) $(PTHREADSTUB)

shmex2: shmex2.o SimpleSemSysV.o $(AUPOBJS) $(PTHREADSTUB)
	$(CC) -o shmex2 shmex2.o SimpleSemSysV.o $(AUPOBJS) $(LIBS) $(PTHREADSTUB)

shmex3: shmex3.o SimpleSemSysV.o $(AUPOBJS) $(PTHREADSTUB)
	$(CC) -o shmex3 shmex3.o SimpleSemSysV.o $(AUPOBJS) $(LIBS) $(PTHREADSTUB)

smsg_client: smsg_client.o smsg_makefifoname.o $(AUPOBJS) $(PTHREADSTUB)
	$(CC) -o smsg_client smsg_client.o smsg_makefifoname.o $(AUPOBJS) $(LIBS) $(PTHREADSTUB)

smsg_client_smi_fifo: smsg_client_smi_a.o smi_fifo.o smi_fifo_wrap.o $(AUPOBJS) $(PTHREADSTUB)
	$(CC) -o smsg_client_smi_fifo smsg_client_smi_a.o smi_fifo.o smi_fifo_wrap.o $(AUPOBJS) $(LIBS) $(PTHREADSTUB)

smsg_client_smi_mq: smsg_client_smi_a.o smi_mq.o smi_mq_wrap.o $(AUPOBJS) $(PTHREADSTUB)
	$(CC) -o smsg_client_smi_mq smsg_client_smi_a.o smi_mq.o smi_mq_wrap.o $(AUPOBJS) $(LIBS) $(PTHREADSTUB)

smsg_client_smi_msg: smsg_client_smi_a.o smi_msg.o smi_msg_wrap.o $(AUPOBJS) $(PTHREADSTUB)
	$(CC) -o smsg_client_smi_msg smsg_client_smi_a.o smi_msg.o smi_msg_wrap.o $(AUPOBJS) $(LIBS) $(PTHREADSTUB)

smsg_client_smi_pshm: smsg_client_smi_a.o smi_pshm.o smi_pshm_wrap.o $(AUPOBJS) $(PTHREADSTUB)
	$(CC) -o smsg_client_smi_pshm smsg_client_smi_a.o smi_pshm.o smi_pshm_wrap.o $(AUPOBJS) $(LIBS) $(PTHREADSTUB)

smsg_client_smi_shm: smsg_client_smi_a.o smi_shm.o smi_shm_wrap.o $(AUPOBJS) $(PTHREADSTUB)
	$(CC) -o smsg_client_smi_shm smsg_client_smi_a.o smi_shm.o smi_shm_wrap.o $(AUPOBJS) $(LIBS) $(PTHREADSTUB)

smsg_client_smi_skt: smsg_client_smi_a.o smi_skt.o smi_skt_wrap.o ssi.o $(AUPOBJS) $(PTHREADSTUB)
	$(CC) -o smsg_client_smi_skt smsg_client_smi_a.o smi_skt.o smi_skt_wrap.o ssi.o $(AUPOBJS) $(LIBS) $(PTHREADSTUB)

smsg_server: smsg_server.o smsg_makefifoname.o $(AUPOBJS) $(PTHREADSTUB)
	$(CC) -o smsg_server smsg_server.o smsg_makefifoname.o $(AUPOBJS) $(LIBS) $(PTHREADSTUB)

smsg_server_smi_fifo: smsg_server_smi_a.o smi_fifo.o smi_fifo_wrap.o $(AUPOBJS) $(PTHREADSTUB)
	$(CC) -o smsg_server_smi_fifo smsg_server_smi_a.o smi_fifo.o smi_fifo_wrap.o $(AUPOBJS) $(LIBS) $(PTHREADSTUB)

smsg_server_smi_mq: smsg_server_smi_a.o smi_mq.o smi_mq_wrap.o $(AUPOBJS) $(PTHREADSTUB)
	$(CC) -o smsg_server_smi_mq smsg_server_smi_a.o smi_mq.o smi_mq_wrap.o $(AUPOBJS) $(LIBS) $(PTHREADSTUB)

smsg_server_smi_msg: smsg_server_smi_a.o smi_msg.o smi_msg_wrap.o $(AUPOBJS) $(PTHREADSTUB)
	$(CC) -o smsg_server_smi_msg smsg_server_smi_a.o smi_msg.o smi_msg_wrap.o $(AUPOBJS) $(LIBS) $(PTHREADSTUB)

smsg_server_smi_pshm: smsg_server_smi_a.o smi_pshm.o smi_pshm_wrap.o $(AUPOBJS) $(PTHREADSTUB)
	$(CC) -o smsg_server_smi_pshm smsg_server_smi_a.o smi_pshm.o smi_pshm_wrap.o $(AUPOBJS) $(LIBS) $(PTHREADSTUB)

smsg_server_smi_shm: smsg_server_smi_a.o smi_shm.o smi_shm_wrap.o $(AUPOBJS) $(PTHREADSTUB)
	$(CC) -o smsg_server_smi_shm smsg_server_smi_a.o smi_shm.o smi_shm_wrap.o $(AUPOBJS) $(LIBS) $(PTHREADSTUB)

smsg_server_smi_skt: smsg_server_smi_a.o smi_skt.o smi_skt_wrap.o ssi.o $(AUPOBJS) $(PTHREADSTUB)
	$(CC) -o smsg_server_smi_skt smsg_server_smi_a.o smi_skt.o smi_skt_wrap.o ssi.o $(AUPOBJS) $(LIBS) $(PTHREADSTUB)

ssp: sstest.o SimpleSemPOSIX.o $(AUPOBJS) $(PTHREADSTUB)
	$(CC) -o ssp sstest.o SimpleSemPOSIX.o $(AUPOBJS) $(LIBS) $(PTHREADSTUB)

ssv: sstest.o SimpleSemSysV.o $(AUPOBJS) $(PTHREADSTUB)
	$(CC) -o ssv sstest.o SimpleSemSysV.o $(AUPOBJS) $(LIBS) $(PTHREADSTUB)

uemtst: uemtst.o uem.o $(AUPOBJS) 
	$(CC) -o uemtst uemtst.o uem.o $(AUPOBJS) $(LIBS) $(TLIBS) 

semtest: semtest.o $(AUPOBJS) $(PTHREADSTUB)
	$(CC) -o semtest semtest.o $(AUPOBJS) $(LIBS) $(PTHREADSTUB)

smsg_server_smi_a.o: /aup/c7/smsg_server_smi_a.c
	$(CC) $(CFLAGS)  -c -o smsg_server_smi_a.o /aup/c7/smsg_server_smi_a.c

smi_pshm_wrap.o: /aup/c7/smi_pshm_wrap.c
	$(CC) $(CFLAGS)  -c -o smi_pshm_wrap.o /aup/c7/smi_pshm_wrap.c

smi_skt.o: /aup/c7/smi_skt.c
	$(CC) $(CFLAGS)  -c -o smi_skt.o /aup/c7/smi_skt.c

smi_msg_wrap.o: /aup/c7/smi_msg_wrap.c
	$(CC) $(CFLAGS)  -c -o smi_msg_wrap.o /aup/c7/smi_msg_wrap.c

shmex.o: /aup/c7/shmex.c
	$(CC) $(CFLAGS)  -c -o shmex.o /aup/c7/shmex.c

smi_pshm.o: /aup/c7/smi_pshm.c
	$(CC) $(CFLAGS)  -c -o smi_pshm.o /aup/c7/smi_pshm.c

f1.o: /aup/c7/f1.c
	$(CC) $(CFLAGS)  -c -o f1.o /aup/c7/f1.c

smsg_server.o: /aup/c7/smsg_server.c
	$(CC) $(CFLAGS)  -c -o smsg_server.o /aup/c7/smsg_server.c

smi_mq_wrap.o: /aup/c7/smi_mq_wrap.c
	$(CC) $(CFLAGS)  -c -o smi_mq_wrap.o /aup/c7/smi_mq_wrap.c

uemtst.o: /aup/c7/uemtst.c
	$(CC) $(CFLAGS)  -c -o uemtst.o /aup/c7/uemtst.c

SimpleSemPOSIX.o: /aup/c7/SimpleSemPOSIX.c
	$(CC) $(CFLAGS)  -c -o SimpleSemPOSIX.o /aup/c7/SimpleSemPOSIX.c

smsg_client.o: /aup/c7/smsg_client.c
	$(CC) $(CFLAGS)  -c -o smsg_client.o /aup/c7/smsg_client.c

shmex2.o: /aup/c7/shmex2.c
	$(CC) $(CFLAGS)  -c -o shmex2.o /aup/c7/shmex2.c

smi_fifo.o: /aup/c7/smi_fifo.c
	$(CC) $(CFLAGS)  -c -o smi_fifo.o /aup/c7/smi_fifo.c

pcsync_sig.o: /aup/c6/pcsync_sig.c
	$(CC) $(CFLAGS)  -c -o pcsync_sig.o /aup/c6/pcsync_sig.c

ipcobjs.o: /aup/c7/ipcobjs.c
	$(CC) $(CFLAGS)  -c -o ipcobjs.o /aup/c7/ipcobjs.c

smsg_makefifoname.o: /aup/c7/smsg_makefifoname.c
	$(CC) $(CFLAGS)  -c -o smsg_makefifoname.o /aup/c7/smsg_makefifoname.c

smi_msg.o: /aup/c7/smi_msg.c
	$(CC) $(CFLAGS)  -c -o smi_msg.o /aup/c7/smi_msg.c

SimpleSemSysV.o: /aup/c7/SimpleSemSysV.c
	$(CC) $(CFLAGS)  -c -o SimpleSemSysV.o /aup/c7/SimpleSemSysV.c

f2.o: /aup/c7/f2.c
	$(CC) $(CFLAGS)  -c -o f2.o /aup/c7/f2.c

uem.o: /aup/c7/uem.c
	$(CC) $(CFLAGS)  -c -o uem.o /aup/c7/uem.c

smi_shm.o: /aup/c7/smi_shm.c
	$(CC) $(CFLAGS)  -c -o smi_shm.o /aup/c7/smi_shm.c

msg_time.o: /aup/c7/msg_time.c
	$(CC) $(CFLAGS)  -c -o msg_time.o /aup/c7/msg_time.c

shmex3.o: /aup/c7/shmex3.c
	$(CC) $(CFLAGS)  -c -o shmex3.o /aup/c7/shmex3.c

smi_skt_wrap.o: /aup/c7/smi_skt_wrap.c
	$(CC) $(CFLAGS)  -c -o smi_skt_wrap.o /aup/c7/smi_skt_wrap.c

semtest.o: /aup/c7/semtest.c
	$(CC) $(CFLAGS)  -c -o semtest.o /aup/c7/semtest.c

smsg_client_smi_a.o: /aup/c7/smsg_client_smi_a.c
	$(CC) $(CFLAGS)  -c -o smsg_client_smi_a.o /aup/c7/smsg_client_smi_a.c

sstest.o: /aup/c7/sstest.c
	$(CC) $(CFLAGS)  -c -o sstest.o /aup/c7/sstest.c

smi_shm_wrap.o: /aup/c7/smi_shm_wrap.c
	$(CC) $(CFLAGS)  -c -o smi_shm_wrap.o /aup/c7/smi_shm_wrap.c

smi_fifo_wrap.o: /aup/c7/smi_fifo_wrap.c
	$(CC) $(CFLAGS)  -c -o smi_fifo_wrap.o /aup/c7/smi_fifo_wrap.c

ssi.o: /aup/c8/ssi.c
	$(CC) $(CFLAGS)  -c -o ssi.o /aup/c8/ssi.c

smi_mq.o: /aup/c7/smi_mq.c
	$(CC) $(CFLAGS)  -c -o smi_mq.o /aup/c7/smi_mq.c

lockftest.o: /aup/c7/lockftest.c
	$(CC) $(CFLAGS)  -c -o lockftest.o /aup/c7/lockftest.c

f3.o: /aup/c7/f3.c
	$(CC) $(CFLAGS)  -c -o f3.o /aup/c7/f3.c

all:  uemtst smsg_server_smi_skt smsg_client_smi_shm shmex ssv smsg_client_smi_msg semtest smsg_server_smi_shm smsg_server_smi_msg smsg_server mt ipcobjs lockftest shmex2 smsg_client shmex3 f1 ssp smsg_server_smi_pshm smsg_server_smi_fifo smsg_client_smi_pshm smsg_client_smi_fifo f2 f3 smsg_client_smi_skt smsg_server_smi_mq smsg_client_smi_mq
clean:
	rm  uemtst smsg_server_smi_skt smsg_client_smi_shm shmex ssv smsg_client_smi_msg semtest smsg_server_smi_shm smsg_server_smi_msg smsg_server mt ipcobjs lockftest shmex2 smsg_client shmex3 f1 ssp smsg_server_smi_pshm smsg_server_smi_fifo smsg_client_smi_pshm smsg_client_smi_fifo f2 f3 smsg_client_smi_skt smsg_server_smi_mq smsg_client_smi_mq *.o
