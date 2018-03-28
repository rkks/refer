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

/* static */ void Netdb::sethostent(int stayopen)
{
	::sethostent(stayopen);
}

/* static */ struct hostent * Netdb::gethostent(void)
{
	return ::gethostent();
}

/* static */ void Netdb::endhostent(void)
{
	::endhostent();
}

/* Returns pointer to netent or NULL if not found (errno not set) */

/* static */ void Netdb::setnetent(int stayopen)
{
	::setnetent(stayopen);
}

/* static */ struct netent *Netdb::getnetbyaddr(uint32_t net, int type)
{
	return ::getnetbyaddr(net, type);
}

/* static */ struct netent *Netdb::getnetbyname(const char *name)
{
	return ::getnetbyname(name);
}

/* static */ struct netent * Netdb::getnetent(void)
{
	return ::getnetent();
}

/* static */ void Netdb::endnetent(void)
{
	::endnetent();
}

/* static */ void Netdb::setprotoent(int stayopen)
{
	::setprotoent(stayopen);
}

/* static */ struct protoent *Netdb::getprotobyname(const char *name)
{
	return ::getprotobyname(name);
}

/* static */ struct protoent *Netdb::getprotobynumber(int proto)
{
	return ::getprotobynumber(proto);
}

/* static */ struct protoent * Netdb::getprotoent(void)
{
	return ::getprotoent();
}

/* static */ void Netdb::endprotoent(void)
{
	::endprotoent();
}

/* static */ void Netdb::setservent(int stayopen)
{
	::setservent(stayopen);
}

/* static */ struct servent *Netdb::getservbyname(const char *name, const char *proto)
{
	return ::getservbyname(name, proto);
}

/* static */ struct servent *Netdb::getservbyport(int port, const char *proto)
{
	return ::getservbyport(port, proto);
}

/* static */ struct servent * Netdb::getservent(void)
{
	return ::getservent();
}

/* static */ void Netdb::endservent(void)
{
	::endservent();
}


/* static */ struct if_nameindex *Netdb::if_nameindex(void)
{
	struct if_nameindex *p;

	if ((p = ::if_nameindex()) == NULL)
		throw Error(errno);
	return p;
}

/* static */ void Netdb::if_freenameindex(struct if_nameindex *ptr)
{
	return if_freenameindex(ptr);
}

/* static */ unsigned Netdb::if_nametoindex(const char *ifname)
{
	return if_nametoindex(ifname);
}

/* static */ char *Netdb::if_indextoname(unsigned ifindex, char *ifname)
{
	char *p;

	if ((p = ::if_indextoname(ifindex, ifname)) == NULL)
		throw Error(errno);
	return p;
}
