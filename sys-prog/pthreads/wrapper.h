#include <stdlib.h> /* prototype fuer exit()  */

static int Exit(int e) { exit(e); return 0; }

#define Pthread_attr_init(a)						\
	(pthread_attr_init(a) && 					\
	(printf("pthread_attr_init error at %s line %d\n", 		\
	 __FILE__, __LINE__), Exit(1)))

#define Pthread_attr_setscope(a, s)					\
	(pthread_attr_setscope(a, s) && 				\
	(printf("pthread_attr_setscope error at %s line %d\n", 		\
	 __FILE__, __LINE__), Exit(1)))

#define Pthread_setconcurrency(a)					\
	(pthread_setconcurrency(a) && 					\
	(printf("pthread_setconcurrency error at %s line %d\n", 	\
	 __FILE__, __LINE__), Exit(1)))

#define Pthread_join(a,b)						\
	(pthread_join(a,b) && 					\
	(printf("pthread_join error at %s line %d\n", 		\
	 __FILE__, __LINE__), Exit(1)))

#define Pthread_mutex_lock(a)						\
	(pthread_mutex_lock(a) && 					\
	(printf("pthread_mutex_lock error at %s line %d\n", 		\
	 __FILE__, __LINE__), Exit(1)))

#define Pthread_mutex_unlock(a)						\
	(pthread_mutex_unlock(a) && 					\
	(printf("pthread_mutex_unlock error at %s line %d\n", 		\
	 __FILE__, __LINE__), Exit(1)))

#define Pthread_cond_broadcast(a)					\
	(pthread_cond_broadcast(a) && 					\
	(printf("pthread_cond_broadcast error at %s line %d\n", 	\
	 __FILE__, __LINE__), Exit(1)))

#define Pthread_cond_signal(a)						\
	(pthread_cond_signal(a) && 					\
	(printf("pthread_cond_signal error at %s line %d\n", 		\
	 __FILE__, __LINE__), Exit(1)))

#define Pthread_cond_wait(a,b)						\
	(pthread_cond_wait(a,b) && 					\
	(printf("pthread_cond_wait error at %s line %d\n", 		\
	 __FILE__, __LINE__), Exit(1)))

#define Pthread_create(a,b,c,d)						\
	(pthread_create(a,b,c,d) && 					\
	(printf("pthread_create error at %s line %d\n", 		\
	 __FILE__, __LINE__), Exit(1)))

#define Pthread_mutex_init(a,b)						\
	(pthread_mutex_init(a,b) && 					\
	(printf("pthread_mutex_init error at %s line %d\n", 		\
	 __FILE__, __LINE__), Exit(1)))

#define Pthread_mutex_destroy(a)                                        \
        (pthread_mutex_destroy(a) &&                                    \
        (printf("pthread_mutex_init error at %s line %d\n",             \
         __FILE__, __LINE__), Exit(1)))

#define Pthread_cond_init(a,b)						\
	(pthread_cond_init(a,b) && 					\
	(printf("pthread_cond_init error at %s line %d\n", 		\
	 __FILE__, __LINE__), Exit(1)))

#define Pthread_cond_destroy(a)						\
	(pthread_cond_destroy(a) && 					\
	(printf("pthread_cond_init error at %s line %d\n", 		\
	 __FILE__, __LINE__), Exit(1)))

#define Pthread_attr_setschedparam(a,b)					\
	(pthread_attr_setschedparam(a,b) && 				\
	(printf("pthread_attr_setschedparam error at %s line %d\n", 	\
	 __FILE__, __LINE__), Exit(1)))

#define Pthread_attr_getschedpolicy(a,b)				\
	(pthread_attr_getschedpolicy(a,b) && 				\
	(printf("pthread_attr_getschedpolicy error at %s line %d\n", 	\
	 __FILE__, __LINE__), Exit(1)))

#define Pthread_attr_setschedpolicy(a,b)				\
	(pthread_attr_setschedpolicy(a,b) && 				\
	(printf("pthread_attr_setschedpolicy error at %s line %d\n", 	\
	 __FILE__, __LINE__), Exit(1)))

#define Pthread_attr_getschedparam(a,b)					\
	(pthread_attr_getschedparam(a,b) && 				\
	(printf("pthread_attr_getschedparam error at %s line %d\n", 	\
	 __FILE__, __LINE__), Exit(1)))

#define Pthread_getschedparam(a,b,c)					\
	(pthread_getschedparam(a,b,c) && 				\
	(printf("pthread_getschedparam error at %s line %d\n", 		\
	 __FILE__, __LINE__), Exit(1)))

#define Pthread_setschedparam(a,b,c)					\
	(pthread_setschedparam(a,b,c) && 				\
	(printf("pthread_setschedparam error at %s line %d\n", 		\
	 __FILE__, __LINE__), Exit(1)))

#define Pthread_sigmask(a,b,c)						\
	(pthread_sigmask(a,b,c) && 					\
	(printf("pthread_sigmask error at %s line %d\n", 		\
	 __FILE__, __LINE__), Exit(1)))

#define Sigwait(a,b)							\
	(sigwait(a,b) && 						\
	(printf("sigwait error at %s line %d\n", 			\
	 __FILE__, __LINE__), Exit(1)))

#define Pthread_cancel(a)						\
	(pthread_cancel(a) && 						\
	(printf("pthread_cancel error at %s line %d\n", 		\
	 __FILE__, __LINE__), Exit(1)))

#define Pthread_attr_getstacksize(a,b)					\
	(pthread_attr_getstacksize(a,b) && 				\
	(printf("pthread_attr_getstacksize error at %s line %d\n", 	\
	 __FILE__, __LINE__), Exit(1)))

#define Pthread_attr_setstacksize(a,b)					\
	(pthread_attr_setstacksize(a,b) && 				\
	(printf("pthread_attr_setstacksize error at %s line %d\n", 	\
	 __FILE__, __LINE__), Exit(1)))

#define Pthread_mutexattr_init(a)					\
	(pthread_mutexattr_init(a) && 					\
	(printf("pthread_mutexattr_init error at %s line %d\n", 	\
	 __FILE__, __LINE__), Exit(1)))

#define Pthread_mutex_consistent_np(a)					\
	(pthread_mutex_consistent_np(a) && 				\
	(printf("pthread_mutex_consistent_np error at %s line %d\n", 	\
	 __FILE__, __LINE__), Exit(1)))

#define Pthread_mutexattr_setrobust_np(a,b)				\
	(pthread_mutexattr_setrobust_np(a,b) && 			\
	(printf("pthread_mutexattr_setrobust_np error at %s line %d\n", \
	 __FILE__, __LINE__), Exit(1)))

#define Pthread_mutexattr_setprotocol(a,b)				\
	(pthread_mutexattr_setrobust_np(a,b) && 			\
	(printf("pthread_mutexattr_setrobust_np error at %s line %d\n", \
	 __FILE__, __LINE__), Exit(1)))

#define Pthread_key_create(a,b)				\
	(pthread_key_create(a,b) && 			\
	(printf("pthread_key_create error at %s line %d\n", \
	 __FILE__, __LINE__), Exit(1)))

#define Pthread_setspecific(a,b)				\
	(pthread_setspecific(a,b) && 			\
	(printf("pthread_setspecific error at %s line %d\n", \
	 __FILE__, __LINE__), Exit(1)))

