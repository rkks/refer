#include	<net/if.h>
#include	<netinet/in.h>
#include	<sys/types.h>
#include	<sys/socket.h>
#include	<sys/ioctl.h>
#include	<errno.h>
#define INET_ADDRSTRLEN     16

static const char *
inet_ntop4(src, dst, size)
	const u_char *src;
	char *dst;
	size_t size;
{
	static const char fmt[] = "%u.%u.%u.%u";
	char tmp[sizeof "255.255.255.255"];

	sprintf(tmp, fmt, src[0], src[1], src[2], src[3]);
	if (strlen(tmp) > size) {
		errno = ENOSPC;
		return (NULL);
	}
	strcpy(dst, tmp);
	return (dst);
}

const char *
inet_ntop(af, src, dst, size)
	int af;
	const void *src;
	char *dst;
	size_t size;
{
	switch (af) {
	case AF_INET:
		return (inet_ntop4(src, dst, size));
	default:
		errno = EAFNOSUPPORT;
		return (NULL);
	}
	/* NOTREACHED */
}

const char *
Inet_ntop(int family, const void *addrptr, char *strptr, size_t len)
{
	const char	*ptr;

	if (strptr == NULL)		/* check for old code */
		printf("NULL 3rd argument to inet_ntop");
	if ( (ptr = inet_ntop(family, addrptr, strptr, len)) == NULL)
		printf("inet_ntop error");		/* sets errno */
	return(ptr);
}

int
main(int argc, char **argv)
{
	int					sockfd, len;
	char				*ptr, buf[2048], addrstr[INET_ADDRSTRLEN];
	struct ifconf		ifc;
	struct ifreq		*ifr;
	struct sockaddr_in	*sinptr;

	sockfd = socket(AF_INET, SOCK_DGRAM, 0);

	ifc.ifc_len = sizeof(buf);
	ifc.ifc_req = (struct ifreq *) buf;
	ioctl(sockfd, SIOCGIFCONF, &ifc);

	for (ptr = buf; ptr < buf + ifc.ifc_len; ) {
		ifr = (struct ifreq *) ptr;
		len = sizeof(struct sockaddr);
		ptr += sizeof(ifr->ifr_name) + len;	/* for next one in buffer */

		switch (ifr->ifr_addr.sa_family) {
		case AF_INET:
			sinptr = (struct sockaddr_in *) &ifr->ifr_addr;
			printf("%s\t%s\n", ifr->ifr_name,
				   Inet_ntop(AF_INET, &sinptr->sin_addr, addrstr, sizeof(addrstr)));
			break;

		default:
			printf("%s\n", ifr->ifr_name);
			break;
		}
	}
	exit(0);
}
