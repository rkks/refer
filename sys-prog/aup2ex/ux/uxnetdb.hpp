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
#ifndef _UXNETDB_HPP_
#define _UXNETDB_HPP_

#ifdef SOLARIS
#define __EXTENSIONS__
#endif
#include <netdb.h>
#include <net/if.h>
#ifdef SOLARIS
#undef _EXTENSIONS_
#endif

namespace Ux {

/**
	\ingroup Ux
*/
class Netdb : public Base {

public:
	static struct hostent *	gethostent(void);
	static void				endhostent(void);
	static void				sethostent(int stayopen);
	static struct netent *	getnetbyaddr(uint32_t net, int type);
	static struct netent *	getnetbyname(const char *name);
	static struct netent *	getnetent(void);
	static void				endnetent(void);
	static void				setnetent(int stayopen);
	static struct protoent *getprotobyname(const char *name);
	static struct protoent *getprotobynumber(int proto);
	static struct protoent *getprotoent(void);
	static void				endprotoent(void);
	static void				setprotoent(int stayopen);
	static struct servent *	getservbyname(const char *name, const char *proto);
	static struct servent *	getservbyport(int port, const char *proto);
	static struct servent *	getservent(void);
	static void				endservent(void);
	static void				setservent(int stayopen);

	static uint16_t htons(uint16_t hostnum)
		{ return ::htons(hostnum); }
	static uint32_t htonl(uint32_t hostnum)
		{ return ::htonl(hostnum); }
	static uint16_t ntohs(uint16_t netnum)
		{ return ::ntohs(netnum); }
	static uint32_t ntohl(uint32_t netnum)
		{ return ::ntohl(netnum); }
	
	struct if_nameindex *if_nameindex(void);
	void if_freenameindex(struct if_nameindex *ptr);
	unsigned if_nametoindex(const char *ifname);
	char *if_indextoname(unsigned ifindex, char *ifname);
};

} // namespace

#endif // _UXNETDB_HPP_
