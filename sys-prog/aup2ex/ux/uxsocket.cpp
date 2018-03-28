/*
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
#include "ux.hpp"

#include <arpa/inet.h>

using namespace Ux;

/**
	Calls inet_pton. Throws errors from inet_pton, or EINVAL if inet_pton returns 0.
*/
void SockIPv4::set(const char *str)
{
	switch (::inet_pton(AF_INET, str, &ipv4)) {
	case 0:
		throw Error(EINVAL);
	case -1:
		throw Error(errno);
	}
}

/**
	Calls inet_ntop.
*/
const char *SockIPv4::get_string(char *buf, socklen_t bufsize) const
{
	if (::inet_ntop(AF_INET, &ipv4, buf, bufsize) == NULL)
		throw Error(errno);
	return buf;
}

/**
	Sets object to zeros.
*/
void SockIPv6::set(uint8_t *ip = NULL)
{
	if (ip == NULL)
		memset(ipv6, 0, sizeof(ipv6));
	else
		memcpy(ipv6, ip, sizeof(ipv6));
}

/**
	Calls inet_pton. Throws errors from inet_pton, or EINVAL if inet_pton returns 0.
*/
void SockIPv6::set(const char *str)
{
	switch (::inet_pton(AF_INET6, str, ipv6)) {
	case 0:
		throw Error(EINVAL);
	case -1:
		throw Error(errno);
	}
}

/**
	Calls inet_ntop.
*/
const char *SockIPv6::get_string(char *buf, socklen_t bufsize) const
{
	if (::inet_ntop(AF_INET6, ipv6, buf, bufsize) == NULL)
		throw Error(errno);
	return buf;
}

/**
	Sets up socet address directly from a path. Can throw ENAMETOOLONG.
*/
void SockAddrUn::set(const char *path)
{
	struct sockaddr_un *p = (struct sockaddr_un *)&sa_storage;

	p->sun_family = AF_UNIX;
	if (strlen(path) >= sizeof(p->sun_path))
		throw Error(ENAMETOOLONG);
	strcpy(p->sun_path, path);
	set_len(sizeof(struct sockaddr_un));
}

/**
	Sets up socet address directly from a struct in_addr.
*/
void SockAddrIn::set(in_port_t port, struct in_addr& sa)
{
	struct sockaddr_in *p = (struct sockaddr_in *)&sa_storage;

	p->sin_family = AF_INET;
	p->sin_port = port;
	p->sin_addr = sa;
	set_len(sizeof(struct sockaddr_in));
}

/**
	Sets up socet address directly from an IPv4 as a 32-bit number.
*/
void SockAddrIn::set(in_port_t port, in_addr_t ipv4)
{
	struct in_addr sa;

	sa.s_addr = ipv4;
	set(port, sa);
}

/**
	Sets up socet address directly from dotted string.
*/
void SockAddrIn::set(in_port_t port, const char *dotted)
{
	set(port, SockIPv4(dotted).get_ipv4());
}

/**
	Calls getaddrinfo.
*/
/* static */ void SockAddr::getaddrinfo(const char *nodename, const char *servname,
  const struct addrinfo *hint, struct addrinfo **infop)
{
	int r;

	if ((r = ::getaddrinfo(nodename, servname, hint, infop)) != 0)
		throw Error(r, EC_EAI);
}

/**
	Calls getaddrinfo, but doesn't require setting up a separate hint structure.
*/
/* static */ void SockAddr::getaddrinfo(const char *nodename, const char *servname, int family, int type,
  int flags, struct addrinfo **infop)
{
	struct addrinfo hint;

	memset(&hint, 0, sizeof(hint));
	hint.ai_family = family;
	hint.ai_socktype = type;
	hint.ai_flags = flags;
	getaddrinfo(nodename, servname, &hint, infop);
}

/**
	Calls getaddrinfo and then sets up the socket address from a nodename and servname (port).
*/
void SockAddr::set_server(const char *nodename, const char *servname)
{
	struct addrinfo *infop;

	getaddrinfo(nodename, servname, AF_INET, SOCK_STREAM, AI_PASSIVE, &infop);
	memcpy(&sa_storage, infop->ai_addr,	infop->ai_addrlen);
	set_len(infop->ai_addrlen);
	set_protocol(infop->ai_protocol);
}

/**
	Calls getnameinfo.
*/
/* static */ void SockAddr::getnameinfo(const struct sockaddr *sa, socklen_t sa_len, char *nodename,
	  socklen_t nodelen, char *servname, socklen_t servlen, unsigned flags)
{
	int r;

	if ((r = ::getnameinfo(sa, sa_len, nodename, nodelen, servname, servlen, flags)) != 0)
		throw Error(r, EC_EAI);
}

/**
	Calls socket.
*/
void Socket::socket(int domain, int type, int protocol)
{
	if ((fd = ::socket(domain, type, protocol)) == -1)
		throw Error(errno);
}

/**
	Calls bind.
*/
void Socket::bind(const SockAddr& sa)
{
	if (::bind(fd, sa.get_addr(), sa.get_len()) == -1)
		throw Error(errno);
}

/**
	Calls listen.
*/
void Socket::listen(int backlog)
{
	if (::listen(fd, backlog) == -1)
		throw Error(errno);
}

/**
	Calls accept.
*/
Socket Socket::accept(SockAddr *sa)
{
	int r;

	if (sa == NULL)
		r = ::accept(fd, NULL, NULL);
	else
		r = ::accept(fd, sa->get_addr(), sa->get_len_ptr());
	if (r == -1)
		throw Error(errno);
	return Socket(r);
}

/**
	Calls connect.
*/
void Socket::connect(const SockAddr& sa)
{
	if (::connect(fd, sa.get_addr(), sa.get_len()) == -1)
		throw Error(errno);
}

/**
	Calls setsockopt.
*/
void Socket::setsockopt(int level, int option, const void *value, socklen_t value_len)
{
	if (::setsockopt(fd, level, option, value, value_len) == -1)
		throw Error(errno);
}

/**
	Calls getsockopt.
*/
void Socket::getsockopt(int level, int option, void *value, socklen_t& value_len)
{
	if (::getsockopt(fd, level, option, value, &value_len) == -1)
		throw Error(errno);
}

/**
	Calls sendto.
*/
ssize_t Socket::sendto(const void *message, size_t length, int flags, const struct sockaddr *sa,
	  socklen_t sa_len)
{
	ssize_t n;

	if ((n = ::sendto(fd, message, length, flags, sa, sa_len)) == -1)
		throw Error(errno);
	return n;
}

/**
	Calls recvfrom.
*/
ssize_t Socket::recvfrom(void *buffer, size_t length, int flags, struct sockaddr *sa,
	  socklen_t *sa_len)
{
	ssize_t n;

	if ((n = ::recvfrom(fd, buffer, length, flags, sa, sa_len)) == -1)
		throw Error(errno);
	return n;
}

/**
	Calls sendmsg.
*/
ssize_t Socket::sendmsg(const struct msghdr *message, int flags)
{
	ssize_t n;

	if ((n = ::sendmsg(fd, message, flags)) == -1)
		throw Error(errno);
	return n;
}

/**
	Calls recvmsg.
*/
ssize_t Socket::recvmsg(struct msghdr *message, int flags)
{
	ssize_t n;

	if ((n = ::recvmsg(fd, message, flags)) == -1)
		throw Error(errno);
	return n;
}

/**
	Calls send.
*/
ssize_t Socket::send(const void *data, size_t length, int flags)
{
	ssize_t n;

	if ((n = ::send(fd, data, length, flags)) == -1)
		throw Error(errno);
	return n;
}

/**
	Calls recv.
*/
ssize_t Socket::recv(void *buffer, size_t length, int flags)
{
	ssize_t n;

	if ((n = ::recv(fd, buffer, length, flags)) == -1)
		throw Error(errno);
	return n;
}
/**
	Calls getpeername.
*/
void Socket::getpeername(SockAddr& sa)
{
	if (::getpeername(fd, sa.get_addr(), sa.get_len_ptr()) == -1)
		throw Error(errno);
}

/**
	Calls getsockname.
*/
void Socket::getsockname(SockAddr& sa)
{
	if (::getsockname(fd, sa.get_addr(), sa.get_len_ptr()) == -1)
		throw Error(errno);
}

/**
	Calls socketpair. Arguments rearranged so array comes first, which allows default arguments.
*/
/* static */ void Socket::socketpair(Socket sv[2], int domain, int type, int protocol)
{
	int fds[2];

	if (::socketpair(domain, type, protocol, fds) == -1)
		throw Error(errno);
	sv[0].set(fds[0]);
	sv[1].set(fds[1]);
}

/**
	Calls shutdown.
*/
void Socket::shutdown(int how)
{
	if (::shutdown(fd, how) == -1)
		throw Error(errno);
}

/**
	Calls sockatmark; return converted to bool.
*/
bool Socket::sockatmark(void)
{
#if _XOPEN_VERSION >= 600
	int r;

	if ((r = ::sockatmark(fd)) == -1)
		throw Error(errno);
	return r == 1;
#else
	throw Error(ENOSYS);
#endif
}
