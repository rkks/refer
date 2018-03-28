!include common/makebuild.spec

f1 c7/f1.c
f2 c7/f2.c c7/SimpleSemSysV.c
f3 c7/f3.c c7/SimpleSemSysV.c
ipcobjs c7/ipcobjs.c
lockftest c7/lockftest.c
mt c7/msg_time.c c6/pcsync_sig.c c7/smi_fifo.c c7/smi_mq.c c7/smi_msg.c c7/smi_skt.c c7/ssi.o c7/smi_shm.c c7/smi_pshm.c
shmex c7/shmex.c
shmex2 c7/shmex2.c c7/SimpleSemSysV.c
shmex3 c7/shmex3.c c7/SimpleSemSysV.c
smsg_client c7/smsg_client.c c7/smsg_makefifoname.c
smsg_client_smi_fifo c7/smsg_client_smi_a.c c7/smi_fifo.c c7/smi_fifo_wrap.c
smsg_client_smi_mq c7/smsg_client_smi_a.c c7/smi_mq.c c7/smi_mq_wrap.c
smsg_client_smi_msg c7/smsg_client_smi_a.c c7/smi_msg.c c7/smi_msg_wrap.c
smsg_client_smi_pshm c7/smsg_client_smi_a.c c7/smi_pshm.c c7/smi_pshm_wrap.c
smsg_client_smi_shm c7/smsg_client_smi_a.c c7/smi_shm.c c7/smi_shm_wrap.c
smsg_client_smi_skt c7/smsg_client_smi_a.c c7/smi_skt.c c7/smi_skt_wrap.c c7/ssi.c
smsg_server c7/smsg_server.c c7/smsg_makefifoname.c
smsg_server_smi_fifo c7/smsg_server_smi_a.c c7/smi_fifo.c c7/smi_fifo_wrap.c
smsg_server_smi_mq c7/smsg_server_smi_a.c c7/smi_mq.c c7/smi_mq_wrap.c
smsg_server_smi_msg c7/smsg_server_smi_a.c c7/smi_msg.c c7/smi_msg_wrap.c
smsg_server_smi_pshm c7/smsg_server_smi_a.c c7/smi_pshm.c c7/smi_pshm_wrap.c
smsg_server_smi_shm c7/smsg_server_smi_a.c c7/smi_shm.c c7/smi_shm_wrap.c
smsg_server_smi_skt c7/smsg_server_smi_a.c c7/smi_skt.c c7/smi_skt_wrap.c c8/ssi.c
ssp c7/sstest.c c7/SimpleSemPOSIX.c
ssv c7/sstest.c c7/SimpleSemSysV.c
uemtst -t c7/uemtst.c c7/uem.c
semtest c7/semtest.c
