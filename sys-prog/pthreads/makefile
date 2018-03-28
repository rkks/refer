CFLAGS = -D_POSIX_C_SOURCE=199506L
#CFLAGS = -D_POSIX_C_SOURCE=200112L

#CC = c99	# for _POSIX_C_SOURCE=200112L
CC = cc

LIBS = -lrt # -lpthread not needed in Solaris 10
# LIBS = -lpthread -lrt 	# pre Solaris 10

.c:
		$(CC) $(CFLAGS) -o $@ $< $(LIBS)

.C:
		CC -g $(CFLAGS) -o $@ $< $(LIBS)

all:		th_concurr2 cache_line.good prod_consC \
		a_thread bad_main bad_threadname barrier barrier_rahmen \
		cache_line cnt cnt_fix cond_wait_cancel condition \
		countdown countdown2 deadlock debugit default_prio \
		join_main liste no_prod_cons other_prio pause \
		pause0 prod_cons prod_cons2 prod_cons_int prod_cons_rahmen \
		prprio random_add random_add_ser rdtimeout rw2 \
		sched_rr_fifo sigpipe_test sigtest sigwait spurious_wakeup \
		stacksize th_concurr th_scope turn unfair_mutex wrapper

prprio:		prprio.c
		$(CC) $(CFLAGS) -o $@ prprio.c $(LIBS) 

bad_threadname:	bad_threadname.c
	$(CC) $(CFLAGS) -g -o $@ bad_threadname.c $(LIBS)

# 1-stufiges scheduling Modell (Solaris 8)
th_concurr2:	th_concurr.c
		$(CC) $(CFLAGS) -o $@ th_concurr.c $(LIBS) \
		-R/usr/lib/lwp -lthread 

no_prod_cons:	no_prod_cons.c
		$(CC) $(CFLAGS) -o $@ no_prod_cons.c

cache_line.good:cache_line.c
		$(CC) $(CFLAGS) -DGOOD -o $@ cache_line.c $(LIBS) 

cache_line:	cache_line.c
		$(CC) $(CFLAGS) -o $@ cache_line.c $(LIBS) 


prod_consC:	prod_consC.C
		CC -mt prod_consC.C -o prod_consC
