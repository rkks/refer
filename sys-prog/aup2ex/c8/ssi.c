/*
	Simple Socket Interface
	AUP2, Sec. 8.04.5

	Copyright 2003 by Marc J. Rochkind. All rights reserved.
	May be copied only for purposes and under conditions described
	on the Web page www.basepath.com/aup/copyright.htm.

	The Example Files are provided "as is," without any warranty;
	without even the implied warranty of merchantability or fitness
	for a particular purpose. The author and his publisher are not
	responsible for any damages, direct or incidental, resulting
	from the use or non-use of these Example Files.

	The Example Files may contain defects, and some contain deliberate
	coding mistakes that were included for educational reasons.
	You are responsible for determining if and how the Example Files
	are to be used.

*/
#ifdef DARWIN
/* Following needed to get socklen_t defined. */
#define _BSD_SOCKLEN_T_ int
#endif /* DARWIN */
#include "defs.h"
#include "ssi.h"
#ifdef SOLARIS
#define __EXTENSIONS__
#endif
#include <netdb.h>
#undef __EXTENSIONS__
#include <sys/un.h>
#include <netinet/in.h>
#include <arpa/inet.h>
/*[set_fd_hwm]*/
static void set_fd_hwm(SSI *ssip, int fd)
{
	if (fd > ssip->ssi_fd_hwm)
		ssip->ssi_fd_hwm = fd;
}

static void reset_fd_hwm(SSI *ssip, int fd)
{
	if (fd == ssip->ssi_fd_hwm)
		ssip->ssi_fd_hwm--;
}
/*[make_sockaddr]*/
bool make_sockaddr(struct sockaddr *sa, socklen_t *len, const char *name,
  int domain, bool will_bind)
{
	struct addrinfo *infop = NULL;

	if (domain == AF_UNIX) {
		struct sockaddr_un *sunp = (struct sockaddr_un *)sa;

		if (strlen(name) >= sizeof(sunp->sun_path)) {
			errno = ENAMETOOLONG;
			EC_FAIL
		}
		strcpy(sunp->sun_path, name);
		sunp->sun_family = AF_UNIX;
		*len = sizeof(*sunp);
	}
	else {
		struct addrinfo hint;
		char nodename[SSI_NAME_SIZE], *servicename;

		memset(&hint, 0, sizeof(hint));
		hint.ai_family = domain;
		hint.ai_socktype = SOCK_STREAM;
		if (will_bind)
			hint.ai_flags = AI_PASSIVE;
		strcpy(nodename, name);
		servicename = strchr(nodename, ':');
		if (servicename == NULL) {
			errno = EINVAL;
			EC_FAIL
		}
		*servicename = '\0';
		servicename++;
		ec_ai( getaddrinfo(nodename, servicename, &hint, &infop) )
		memcpy(sa, infop->ai_addr, infop->ai_addrlen);
		*len = infop->ai_addrlen;
		freeaddrinfo(infop);
	}
	return true;

EC_CLEANUP_BGN
	if (infop != NULL)
		freeaddrinfo(infop);
	return false;
EC_CLEANUP_END
}
/*[]*/
/*
	Name of the form "//host:port" uses AF_INET to access that host and port. Otherwise, AF_UNIX is used, and the name has no slashes, colon, or port. (E.g., "someservice".) Because numeric port numbers are used, there need be no entry in /etc/services. host can be a name with an entry in /etc/hosts (or via DNS), or it can be a numeric IP address (e.g., 192.168,0,1).
*/
/*[ssi_open]*/
SSI *ssi_open(const char *name_server, bool server)
{
	SSI *ssip = NULL;
	struct sockaddr_storage sa;
	socklen_t sa_len;

	ec_null(  ssip = (SSI *)calloc(1, sizeof(SSI)) )
	ssip->ssi_server = server;
	if (strncmp(name_server, "//", 2) == 0) {
		ssip->ssi_domain = AF_INET;
		name_server += 2;
	}
	else {
		ssip->ssi_domain = AF_UNIX;
		if (ssip->ssi_server)
			(void)unlink(name_server);
	}
	if (strlen(name_server) >= sizeof(ssip->ssi_name_server)) {
		errno = ENAMETOOLONG;
		EC_FAIL
	}
	strcpy(ssip->ssi_name_server, name_server);
	ec_false( make_sockaddr((struct sockaddr *)&sa, &sa_len,
	  ssip->ssi_name_server, ssip->ssi_domain, ssip->ssi_server) )
	ec_neg1(  ssip->ssi_fd = socket(ssip->ssi_domain, SOCK_STREAM, 0) )
	set_fd_hwm(ssip, ssip->ssi_fd);
	if (ssip->ssi_domain == AF_INET) {
		/*
			SO_REUSEADDR supposedly makes the connection less reliable,
			but it's too inconvenient to do development without it set.
		*/
		int socket_option_value = 1;

		ec_neg1(  setsockopt(ssip->ssi_fd, SOL_SOCKET, SO_REUSEADDR,
		  &socket_option_value, sizeof(socket_option_value)) )
	}
	if (ssip->ssi_server) {
		FD_ZERO(&ssip->ssi_fd_set);
		ec_neg1( bind(ssip->ssi_fd, (struct sockaddr *)&sa, sa_len) )
		ec_neg1( listen(ssip->ssi_fd, SOMAXCONN) )
		FD_SET(ssip->ssi_fd, &ssip->ssi_fd_set);
	}
	else
		ec_neg1( connect(ssip->ssi_fd, (struct sockaddr *)&sa, sa_len) )
	return ssip;

EC_CLEANUP_BGN
	free(ssip);
	return NULL;
EC_CLEANUP_END
}
/*[ssi_close]*/
bool ssi_close(SSI *ssip)
{
	if (ssip->ssi_server) {
		int fd;

		for (fd = 0; fd <= ssip->ssi_fd_hwm; fd++)
			if (FD_ISSET(fd, &ssip->ssi_fd_set))
				(void)close(fd);
		if (ssip->ssi_domain == AF_UNIX)
			(void)unlink(ssip->ssi_name_server);
	}
	else
		(void)close(ssip->ssi_fd);
	free(ssip);
	return true;
}
/*[ssi_close_fd]*/
bool ssi_close_fd(SSI *ssip, int fd)
{
	ec_neg1( close(fd) );
	FD_CLR(fd, &ssip->ssi_fd_set);
	reset_fd_hwm(ssip, fd);
	return true;

EC_CLEANUP_BGN
	return false;
EC_CLEANUP_END
}
/*[ssi_wait_server]*/
int ssi_wait_server(SSI *ssip)
{
	if (ssip->ssi_server) {
		fd_set fd_set_read;
		int fd, clientfd;
		struct sockaddr_un from;
		socklen_t from_len = sizeof(from);

		while (true) {
			fd_set_read = ssip->ssi_fd_set;
			ec_neg1( select(ssip->ssi_fd_hwm + 1, &fd_set_read, NULL, NULL,
			  NULL) )
			for (fd = 0; fd <= ssip->ssi_fd_hwm; fd++)
				if (FD_ISSET(fd, &fd_set_read)) {
					if (fd == ssip->ssi_fd) {
						ec_neg1( clientfd = accept(ssip->ssi_fd,
						  (struct sockaddr *)&from, &from_len) );
						FD_SET(clientfd, &ssip->ssi_fd_set);
						set_fd_hwm(ssip, clientfd);
						continue;
					}
					else
						return fd;
				}
		}
	}
	else {
		errno = ENOTSUP;
		EC_FAIL
	}

EC_CLEANUP_BGN
	return -1;
EC_CLEANUP_END
}
/*[ssi_get_server_fd]*/
int ssi_get_server_fd(SSI *ssip)
{
	return ssip->ssi_fd;
}
/*[]*/
#if 0
bool ssi_send(SSI *ssip, void *buf, size_t bufsize)
{
	if (ssip->ssi_server) {
		ec_neg1( sendto(ssip->ssi_fd_client, buf, bufsize, 0,
		  (struct sockaddr *)&ssip->ssi_sa, ssip->ssi_sa_len) )
	}
	else
		ec_neg1( send(ssip->ssi_fd, buf, bufsize, 0) )
	return true;

EC_CLEANUP_BGN
	return false;
EC_CLEANUP_END
}

bool ssi_receive(SSI *ssip, void *buf, size_t bufsize)
{
	if (ssip->ssi_server) {
		ec_neg1( ssip->ssi_fd_client = ssi_wait_server(ssip) )
		ssip->ssi_sa_len = sizeof(ssip->ssi_sa);
// check on short count... go back for more?
		ec_neg1( recvfrom(ssip->ssi_fd_client, buf, bufsize,
		  0, (struct sockaddr *)&ssip->ssi_sa, &ssip->ssi_sa_len) )
	}
	else
		ec_neg1( recv(ssip->ssi_fd, buf, bufsize, 0) )
	return true;

EC_CLEANUP_BGN
	return false;
EC_CLEANUP_END
}
#endif
