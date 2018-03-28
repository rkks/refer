#ifndef _SSI_H_
#define _SSI_H_

/*
	Simple Sockets Interface
*/

#ifdef SOLARIS
#define __EXTENSIONS__
#endif
#include <sys/socket.h>
#undef __EXTENSIONS__
/*[SSI]*/
#define SSI_NAME_SIZE 200

typedef struct {
	bool ssi_server;              /* server? (vs. client) */
	int ssi_domain;               /* AF_INET or AF_UNIX */
	int ssi_fd;                   /* socket fd */
	char ssi_name_server[SSI_NAME_SIZE]; /* server name */
	fd_set ssi_fd_set;            /* set for server's select */
	int ssi_fd_hwm;               /* high-water-mark for fds seen */
} SSI;
/*[]*/
SSI *ssi_open(const char *name_server, bool server);
bool ssi_close(SSI *ssip);
int ssi_wait_server(SSI *ssip);
int ssi_get_server_fd(SSI *ssip);
bool ssi_close_fd(SSI *ssip, int fd);
#if 0
bool ssi_send(SSI *ssip, void *buf, size_t bufsize/*, SSI_WHO *whop*/);
bool ssi_receive(SSI *ssip, void *buf, size_t bufsize/*, SSI_WHO *whop*/);
#endif
#endif /* _SSI_H_ */
