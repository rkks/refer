/* Automatically generated file, do not edit! */
#define const
struct ap_control { int _xxx; };
#define COMPAT_43
struct lwp;
struct videomemory_t { int _xxx; };
typedef struct videomemory_t videomemory_t;
#include <sys/types.h>
#include <sys/param.h>
#include <sys/disk.h>
#include <sys/dkbad.h>
#include <sys/mount.h>
#include <sys/termios.h>
#include <sys/disklabel.h>
#include <net/if.h>
#include <net/route.h>
#include <net/ppp_defs.h>
#include <netinet/in.h>
#include <netinet/in_systm.h>
#include <netinet/ip.h>
#include <netinet/ip_mroute.h>
#include <netinet/ip_compat.h>
#include <net80211/ieee80211.h>
#include <netiso/iso.h>
#include <altq/altq.h>
#include <altq/altq_blue.h>
#include <altq/altq_cbq.h>
#include <altq/altq_cdnr.h>
#include <altq/altq_fifoq.h>
#include <altq/altq_hfsc.h>
#include <altq/altq_priq.h>
#include <altq/altq_red.h>
#include <altq/altq_rio.h>
#include <altq/altq_wfq.h>
#include <crypto/cryptodev.h>
#include <dev/ccdvar.h>
#include <dev/cgdvar.h>
#include <dev/fssvar.h>
#include <dev/md.h>
#include <dev/vndvar.h>
#include <i386/pccons.h>
#include <krb5/kafs.h>
#include <machine/pccons.h>
#include <net/bpf.h>
#include <net/if_gre.h>
#include <net/if_ppp.h>
#include <net/if_pppoe.h>
#include <net/if_sppp.h>
#include <net/if_tap.h>
#include <net/if_tun.h>
#include <net/pfvar.h>
#include <net/slip.h>
#include <net80211/ieee80211_ioctl.h>
#include <netatalk/phase2.h>
#include <netccitt/x25.h>
#include <netinet/ip_fil.h>
#include <netinet/ip_lookup.h>
#include <netinet/ip_nat.h>
#include <netinet/ip_state.h>
#include <netinet6/in6_var.h>
#include <netisdn/i4b_debug.h>
#include <netisdn/i4b_ioctl.h>
#include <netisdn/i4b_rbch_ioctl.h>
#include <netisdn/i4b_tel_ioctl.h>
#include <netisdn/i4b_trace.h>
#include <netiso/iso_snpac.h>
#include <netiso/iso_var.h>
#include <sys/ataio.h>
#include <sys/audioio.h>
#include <sys/cdio.h>
#include <sys/chio.h>
#include <sys/clockctl.h>
#include <sys/dkio.h>
#include <sys/drvctlio.h>
#include <sys/envsys.h>
#include <sys/fdio.h>
#include <sys/filio.h>
#include <sys/ioccom.h>
#include <sys/ioctl.h>
#include <sys/ioctl_compat.h>
#include <sys/joystick.h>
#include <sys/ksyms.h>
#include <sys/lkm.h>
#include <sys/mtio.h>
#include <sys/power.h>
#include <sys/radioio.h>
#include <sys/rnd.h>
#include <sys/scanio.h>
#include <sys/scsiio.h>
#include <sys/sockio.h>
#include <sys/soundcard.h>
#include <sys/tablet.h>
#include <sys/timepps.h>
#include <sys/ttycom.h>
#include <sys/verified_exec.h>
#include <sys/wdog.h>

char *ioctlname __P((long));

char *
ioctlname(val)
	long val;
{

	switch (val) {

/* #define BLKDEV_IOSIZE 2048 [BLKDEV_IOSIZE] */
	case BLKDEV_IOSIZE:
		return "BLKDEV_IOSIZE";
/* #define _IOWR(g,n,t) _IOC(IOC_INOUT, (g), (n), sizeof(t)) [_IOWR(g,n,t)] */
/* #define ALTQTBRGET _IOWR('q', 15, struct tbrreq) [ALTQTBRGET] */
	case ALTQTBRGET:
		return "ALTQTBRGET";
/* #define ALTQGTYPE _IOWR('q', 0, struct altqreq) [ALTQGTYPE] */
	case ALTQGTYPE:
		return "ALTQGTYPE";
/* #define ALTQTBRSET _IOW('q', 14, struct tbrreq) [ALTQTBRSET] */
	case ALTQTBRSET:
		return "ALTQTBRSET";
/* #define _IO(g,n) _IOC(IOC_VOID, (g), (n), 0) [_IO(g,n)] */
/* #define _IOC(inout,group,num,len) (inout | ((len & IOCPARM_MASK) << 16) | ((group) << 8) | (num)) [_IOC(inout,group,num,len)] */
/* #define _IOR(g,n,t) _IOC(IOC_OUT, (g), (n), sizeof(t)) [_IOR(g,n,t)] */
/* #define _IOW(g,n,t) _IOC(IOC_IN, (g), (n), sizeof(t)) [_IOW(g,n,t)] */
/* #define BLKDEV_IOSIZE 2048 [BLKDEV_IOSIZE] */
/* #define _IOWR(g,n,t) _IOC(IOC_INOUT, (g), (n), sizeof(t)) [_IOWR(g,n,t)] */
/* #define ALTQTBRGET _IOWR('q', 15, struct tbrreq) [ALTQTBRGET] */
/* #define ALTQGTYPE _IOWR('q', 0, struct altqreq) [ALTQGTYPE] */
/* #define ALTQTBRSET _IOW('q', 14, struct tbrreq) [ALTQTBRSET] */
/* #define _IO(g,n) _IOC(IOC_VOID, (g), (n), 0) [_IO(g,n)] */
/* #define _IOC(inout,group,num,len) (inout | ((len & IOCPARM_MASK) << 16) | ((group) << 8) | (num)) [_IOC(inout,group,num,len)] */
/* #define _IOR(g,n,t) _IOC(IOC_OUT, (g), (n), sizeof(t)) [_IOR(g,n,t)] */
/* #define _IOW(g,n,t) _IOC(IOC_IN, (g), (n), sizeof(t)) [_IOW(g,n,t)] */
/* #define BLKDEV_IOSIZE 2048 [BLKDEV_IOSIZE] */
/* #define _IOWR(g,n,t) _IOC(IOC_INOUT, (g), (n), sizeof(t)) [_IOWR(g,n,t)] */
/* #define ALTQTBRGET _IOWR('q', 15, struct tbrreq) [ALTQTBRGET] */
/* #define ALTQGTYPE _IOWR('q', 0, struct altqreq) [ALTQGTYPE] */
/* #define ALTQTBRSET _IOW('q', 14, struct tbrreq) [ALTQTBRSET] */
/* #define _IO(g,n) _IOC(IOC_VOID, (g), (n), 0) [_IO(g,n)] */
/* #define _IOC(inout,group,num,len) (inout | ((len & IOCPARM_MASK) << 16) | ((group) << 8) | (num)) [_IOC(inout,group,num,len)] */
/* #define _IOR(g,n,t) _IOC(IOC_OUT, (g), (n), sizeof(t)) [_IOR(g,n,t)] */
/* #define _IOW(g,n,t) _IOC(IOC_IN, (g), (n), sizeof(t)) [_IOW(g,n,t)] */
/* #define BLKDEV_IOSIZE 2048 [BLKDEV_IOSIZE] */
/* #define _IOWR(g,n,t) _IOC(IOC_INOUT, (g), (n), sizeof(t)) [_IOWR(g,n,t)] */
/* #define ALTQTBRGET _IOWR('q', 15, struct tbrreq) [ALTQTBRGET] */
/* #define ALTQGTYPE _IOWR('q', 0, struct altqreq) [ALTQGTYPE] */
/* #define ALTQTBRSET _IOW('q', 14, struct tbrreq) [ALTQTBRSET] */
/* #define _IO(g,n) _IOC(IOC_VOID, (g), (n), 0) [_IO(g,n)] */
/* #define _IOC(inout,group,num,len) (inout | ((len & IOCPARM_MASK) << 16) | ((group) << 8) | (num)) [_IOC(inout,group,num,len)] */
/* #define _IOR(g,n,t) _IOC(IOC_OUT, (g), (n), sizeof(t)) [_IOR(g,n,t)] */
/* #define _IOW(g,n,t) _IOC(IOC_IN, (g), (n), sizeof(t)) [_IOW(g,n,t)] */
/* #define BLKDEV_IOSIZE 2048 [BLKDEV_IOSIZE] */
/* #define _IOWR(g,n,t) _IOC(IOC_INOUT, (g), (n), sizeof(t)) [_IOWR(g,n,t)] */
/* #define ALTQTBRGET _IOWR('q', 15, struct tbrreq) [ALTQTBRGET] */
/* #define ALTQGTYPE _IOWR('q', 0, struct altqreq) [ALTQGTYPE] */
/* #define ALTQTBRSET _IOW('q', 14, struct tbrreq) [ALTQTBRSET] */
/* #define _IO(g,n) _IOC(IOC_VOID, (g), (n), 0) [_IO(g,n)] */
/* #define _IOC(inout,group,num,len) (inout | ((len & IOCPARM_MASK) << 16) | ((group) << 8) | (num)) [_IOC(inout,group,num,len)] */
/* #define _IOR(g,n,t) _IOC(IOC_OUT, (g), (n), sizeof(t)) [_IOR(g,n,t)] */
/* #define _IOW(g,n,t) _IOC(IOC_IN, (g), (n), sizeof(t)) [_IOW(g,n,t)] */
/* #define BLKDEV_IOSIZE 2048 [BLKDEV_IOSIZE] */
/* #define _IOWR(g,n,t) _IOC(IOC_INOUT, (g), (n), sizeof(t)) [_IOWR(g,n,t)] */
/* #define ALTQTBRGET _IOWR('q', 15, struct tbrreq) [ALTQTBRGET] */
/* #define ALTQGTYPE _IOWR('q', 0, struct altqreq) [ALTQGTYPE] */
/* #define ALTQTBRSET _IOW('q', 14, struct tbrreq) [ALTQTBRSET] */
/* #define _IO(g,n) _IOC(IOC_VOID, (g), (n), 0) [_IO(g,n)] */
/* #define _IOC(inout,group,num,len) (inout | ((len & IOCPARM_MASK) << 16) | ((group) << 8) | (num)) [_IOC(inout,group,num,len)] */
/* #define _IOR(g,n,t) _IOC(IOC_OUT, (g), (n), sizeof(t)) [_IOR(g,n,t)] */
/* #define _IOW(g,n,t) _IOC(IOC_IN, (g), (n), sizeof(t)) [_IOW(g,n,t)] */
/* #define CIOCASYMFEAT _IOR('c', 105, u_int32_t) [CIOCASYMFEAT] */
	case CIOCASYMFEAT:
		return "CIOCASYMFEAT";
/* #define CIOCFSESSION _IOW('c', 102, u_int32_t) [CIOCFSESSION] */
	case CIOCFSESSION:
		return "CIOCFSESSION";
/* #define CIOCCRYPT _IOWR('c', 103, struct crypt_op) [CIOCCRYPT] */
	case CIOCCRYPT:
		return "CIOCCRYPT";
/* #define CRIOGET _IOWR('c', 100, u_int32_t) [CRIOGET] */
	case CRIOGET:
		return "CRIOGET";
/* #define _IOWR(g,n,t) _IOC(IOC_INOUT, (g), (n), sizeof(t)) [_IOWR(g,n,t)] */
/* #define CIOCKEY _IOWR('c', 104, struct crypt_kop) [CIOCKEY] */
	case CIOCKEY:
		return "CIOCKEY";
/* #define CIOCGSESSION _IOWR('c', 101, struct session_op) [CIOCGSESSION] */
	case CIOCGSESSION:
		return "CIOCGSESSION";
/* #define _IO(g,n) _IOC(IOC_VOID, (g), (n), 0) [_IO(g,n)] */
/* #define _IOC(inout,group,num,len) (inout | ((len & IOCPARM_MASK) << 16) | ((group) << 8) | (num)) [_IOC(inout,group,num,len)] */
/* #define _IOR(g,n,t) _IOC(IOC_OUT, (g), (n), sizeof(t)) [_IOR(g,n,t)] */
/* #define _IOW(g,n,t) _IOC(IOC_IN, (g), (n), sizeof(t)) [_IOW(g,n,t)] */
/* #define CCDIOCCLR _IOW('F', 17, struct ccd_ioctl) [CCDIOCCLR] */
	case CCDIOCCLR:
		return "CCDIOCCLR";
/* #define CCDIOCSET _IOWR('F', 16, struct ccd_ioctl) [CCDIOCSET] */
	case CCDIOCSET:
		return "CCDIOCSET";
/* #define CGDIOCSET _IOWR('F', 18, struct cgd_ioctl) [CGDIOCSET] */
	case CGDIOCSET:
		return "CGDIOCSET";
/* #define CGDIOCCLR _IOW('F', 19, struct cgd_ioctl) [CGDIOCCLR] */
	case CGDIOCCLR:
		return "CGDIOCCLR";
/* #define FSSIOCSET _IOW('F', 0, struct fss_set) [FSSIOCSET] */
	case FSSIOCSET:
		return "FSSIOCSET";
/* #define FSSIOFSET _IOW('F', 3, int) [FSSIOFSET] */
	case FSSIOFSET:
		return "FSSIOFSET";
/* #define FSSIOCCLR _IO('F', 2) [FSSIOCCLR] */
	case FSSIOCCLR:
		return "FSSIOCCLR";
/* #define FSSIOCGET _IOR('F', 1, struct fss_get) [FSSIOCGET] */
	case FSSIOCGET:
		return "FSSIOCGET";
/* #define FSSIOFGET _IOR('F', 4, int) [FSSIOFGET] */
	case FSSIOFGET:
		return "FSSIOFGET";
/* #define _IOWR(g,n,t) _IOC(IOC_INOUT, (g), (n), sizeof(t)) [_IOWR(g,n,t)] */
/* #define _IO(g,n) _IOC(IOC_VOID, (g), (n), 0) [_IO(g,n)] */
/* #define _IOC(inout,group,num,len) (inout | ((len & IOCPARM_MASK) << 16) | ((group) << 8) | (num)) [_IOC(inout,group,num,len)] */
/* #define _IOR(g,n,t) _IOC(IOC_OUT, (g), (n), sizeof(t)) [_IOR(g,n,t)] */
/* #define _IOW(g,n,t) _IOC(IOC_IN, (g), (n), sizeof(t)) [_IOW(g,n,t)] */
/* #define VNDIOCSET _IOWR('F', 0, struct vnd_ioctl) [VNDIOCSET] */
	case VNDIOCSET:
		return "VNDIOCSET";
/* #define VNDIOCCLR _IOW('F', 1, struct vnd_ioctl) [VNDIOCCLR] */
	case VNDIOCCLR:
		return "VNDIOCCLR";
/* #define VNDIOCGET _IOWR('F', 2, struct vnd_user) [VNDIOCGET] */
	case VNDIOCGET:
		return "VNDIOCGET";
/* #define DIOCSRETRIES _IOW('d', 108, int) [DIOCSRETRIES] */
	case DIOCSRETRIES:
		return "DIOCSRETRIES";
/* #define FIONBIO _IOW('f', 126, int) [FIONBIO] */
	case FIONBIO:
		return "FIONBIO";
/* #define DIOCLWEDGES _IOWR('d', 124, struct dkwedge_list) [DIOCLWEDGES] */
	case DIOCLWEDGES:
		return "DIOCLWEDGES";
/* #define TIOCGLINED _IOR('t', 66, linedn_t) [TIOCGLINED] */
	case TIOCGLINED:
		return "TIOCGLINED";
/* #define FIONWRITE _IOR('f', 121, int) [FIONWRITE] */
	case FIONWRITE:
		return "FIONWRITE";
/* #define TIOCMSET _IOW('t', 109, int) [TIOCMSET] */
	case TIOCMSET:
		return "TIOCMSET";
/* #define TIOCRCVFRAME _IOW('t', 69, struct mbuf *) [TIOCRCVFRAME] */
	case TIOCRCVFRAME:
		return "TIOCRCVFRAME";
/* #define TIOCDRAIN _IO('t', 94) [TIOCDRAIN] */
	case TIOCDRAIN:
		return "TIOCDRAIN";
/* #define TIOCEXCL _IO('t', 13) [TIOCEXCL] */
	case TIOCEXCL:
		return "TIOCEXCL";
/* #define TIOCPKT_IOCTL 0x40 [TIOCPKT_IOCTL] */
	case TIOCPKT_IOCTL:
		return "TIOCPKT_IOCTL";
/* #define FIOGETOWN _IOR('f', 123, int) [FIOGETOWN] */
	case FIOGETOWN:
		return "FIOGETOWN";
/* #define SIOCGPGRP _IOR('s', 9, int) [SIOCGPGRP] */
	case SIOCGPGRP:
		return "SIOCGPGRP";
/* #define DIOCGCACHE _IOR('d', 116, int) [DIOCGCACHE] */
	case DIOCGCACHE:
		return "DIOCGCACHE";
/* #define TIOCUCNTL _IOW('t', 102, int) [TIOCUCNTL] */
	case TIOCUCNTL:
		return "TIOCUCNTL";
/* #define SIOCGIFADDR _IOWR('i', 33, struct ifreq) [SIOCGIFADDR] */
	case SIOCGIFADDR:
		return "SIOCGIFADDR";
/* #define SIOCGIFMETRIC _IOWR('i', 23, struct ifreq) [SIOCGIFMETRIC] */
	case SIOCGIFMETRIC:
		return "SIOCGIFMETRIC";
/* #define OSIOCGIFADDR _IOWR('i', 13, struct ifreq) [OSIOCGIFADDR] */
	case OSIOCGIFADDR:
		return "OSIOCGIFADDR";
/* #define SIOCGIFGENERIC _IOWR('i', 58, struct ifreq) [SIOCGIFGENERIC] */
	case SIOCGIFGENERIC:
		return "SIOCGIFGENERIC";
/* #define SIOCGIFPSRCADDR _IOWR('i', 71, struct ifreq) [SIOCGIFPSRCADDR] */
	case SIOCGIFPSRCADDR:
		return "SIOCGIFPSRCADDR";
/* #define SIOCSIFDSTADDR _IOW('i', 14, struct ifreq) [SIOCSIFDSTADDR] */
	case SIOCSIFDSTADDR:
		return "SIOCSIFDSTADDR";
/* #define SIOCGIFPDSTADDR _IOWR('i', 72, struct ifreq) [SIOCGIFPDSTADDR] */
	case SIOCGIFPDSTADDR:
		return "SIOCGIFPDSTADDR";
/* #define DIOCGDEFLABEL _IOR('d', 114, struct disklabel) [DIOCGDEFLABEL] */
	case DIOCGDEFLABEL:
		return "DIOCGDEFLABEL";
/* #define FIOASYNC _IOW('f', 125, int) [FIOASYNC] */
	case FIOASYNC:
		return "FIOASYNC";
/* #define SIOCSHIWAT _IOW('s', 0, int) [SIOCSHIWAT] */
	case SIOCSHIWAT:
		return "SIOCSHIWAT";
/* #define SIOCSIFMEDIA _IOWR('i', 53, struct ifreq) [SIOCSIFMEDIA] */
	case SIOCSIFMEDIA:
		return "SIOCSIFMEDIA";
/* #define TIOCSBRK _IO('t', 123) [TIOCSBRK] */
	case TIOCSBRK:
		return "TIOCSBRK";
/* #define DIOCSSTEP _IOW('d', 107, int) [DIOCSSTEP] */
	case DIOCSSTEP:
		return "DIOCSSTEP";
/* #define SIOCDIFPHYADDR _IOW('i', 73, struct ifreq) [SIOCDIFPHYADDR] */
	case SIOCDIFPHYADDR:
		return "SIOCDIFPHYADDR";
/* #define SIOCGLIFADDR _IOWR('i', 29, struct if_laddrreq) [SIOCGLIFADDR] */
	case SIOCGLIFADDR:
		return "SIOCGLIFADDR";
/* #define SIOCGDRVSPEC _IOWR('i', 123, struct ifdrv) [SIOCGDRVSPEC] */
	case SIOCGDRVSPEC:
		return "SIOCGDRVSPEC";
/* #define SIOCDIFADDR _IOW('i', 25, struct ifreq) [SIOCDIFADDR] */
	case SIOCDIFADDR:
		return "SIOCDIFADDR";
/* #define TIOCGRANTPT _IO('t', 71) [TIOCGRANTPT] */
	case TIOCGRANTPT:
		return "TIOCGRANTPT";
/* #define SIOCGIFMTU _IOWR('i', 126, struct ifreq) [SIOCGIFMTU] */
	case SIOCGIFMTU:
		return "SIOCGIFMTU";
/* #define SIOCGIFDLT _IOWR('i', 119, struct ifreq) [SIOCGIFDLT] */
	case SIOCGIFDLT:
		return "SIOCGIFDLT";
/* #define TIOCSIG _IO('t', 95) [TIOCSIG] */
	case TIOCSIG:
		return "TIOCSIG";
/* #define PTIOCNETBSD _IOW('Z', 0, struct ioctl_pt) [PTIOCNETBSD] */
	case PTIOCNETBSD:
		return "PTIOCNETBSD";
/* #define _IOWR(g,n,t) _IOC(IOC_INOUT, (g), (n), sizeof(t)) [_IOWR(g,n,t)] */
/* #define TIOCGETA _IOR('t', 19, struct termios) [TIOCGETA] */
	case TIOCGETA:
		return "TIOCGETA";
/* #define TIOCGETD _IOR('t', 26, int) [TIOCGETD] */
	case TIOCGETD:
		return "TIOCGETD";
/* #define PTIOCULTRIX _IOW('Z', 6, struct ioctl_pt) [PTIOCULTRIX] */
	case PTIOCULTRIX:
		return "PTIOCULTRIX";
/* #define SIOCSIFNETMASK _IOW('i', 22, struct ifreq) [SIOCSIFNETMASK] */
	case SIOCSIFNETMASK:
		return "SIOCSIFNETMASK";
/* #define SIOCAIFADDR _IOW('i', 26, struct ifaliasreq) [SIOCAIFADDR] */
	case SIOCAIFADDR:
		return "SIOCAIFADDR";
/* #define DIOCCACHESYNC _IOW('d', 118, int) [DIOCCACHESYNC] */
	case DIOCCACHESYNC:
		return "DIOCCACHESYNC";
/* #define TIOCSDTR _IO('t', 121) [TIOCSDTR] */
	case TIOCSDTR:
		return "TIOCSDTR";
/* #define SIOCDLIFADDR _IOW('i', 30, struct if_laddrreq) [SIOCDLIFADDR] */
	case SIOCDLIFADDR:
		return "SIOCDLIFADDR";
/* #define DIOCGWEDGEINFO _IOR('d', 122, struct dkwedge_info) [DIOCGWEDGEINFO] */
	case DIOCGWEDGEINFO:
		return "DIOCGWEDGEINFO";
/* #define TIOCXMTFRAME _IOW('t', 68, struct mbuf *) [TIOCXMTFRAME] */
	case TIOCXMTFRAME:
		return "TIOCXMTFRAME";
/* #define TIOCSFLAGS _IOW('t', 92, int) [TIOCSFLAGS] */
	case TIOCSFLAGS:
		return "TIOCSFLAGS";
/* #define DIOCWLABEL _IOW('d', 109, int) [DIOCWLABEL] */
	case DIOCWLABEL:
		return "DIOCWLABEL";
/* #define TIOCSTI _IOW('t', 114, char) [TIOCSTI] */
	case TIOCSTI:
		return "TIOCSTI";
/* #define SIOCSIFBRDADDR _IOW('i', 19, struct ifreq) [SIOCSIFBRDADDR] */
	case SIOCSIFBRDADDR:
		return "SIOCSIFBRDADDR";
/* #define SIOCGLIFPHYADDR _IOWR('i', 75, struct if_laddrreq) [SIOCGLIFPHYADDR] */
	case SIOCGLIFPHYADDR:
		return "SIOCGLIFPHYADDR";
/* #define DIOCWFORMAT _IOWR('d', 106, struct format_op) [DIOCWFORMAT] */
	case DIOCWFORMAT:
		return "DIOCWFORMAT";
/* #define SIOCSPGRP _IOW('s', 8, int) [SIOCSPGRP] */
	case SIOCSPGRP:
		return "SIOCSPGRP";
/* #define TIOCPTSNAME _IOR('t', 72, struct ptmget) [TIOCPTSNAME] */
	case TIOCPTSNAME:
		return "TIOCPTSNAME";
/* #define PTIOCSUNOS _IOW('Z', 1, struct ioctl_pt) [PTIOCSUNOS] */
	case PTIOCSUNOS:
		return "PTIOCSUNOS";
/* #define SIOCADDMULTI _IOW('i', 49, struct ifreq) [SIOCADDMULTI] */
	case SIOCADDMULTI:
		return "SIOCADDMULTI";
/* #define SIOCZIFDATA _IOWR('i', 129, struct ifdatareq) [SIOCZIFDATA] */
	case SIOCZIFDATA:
		return "SIOCZIFDATA";
/* #define SIOCGETSGCNT _IOWR('u', 52, struct sioc_sg_req) [SIOCGETSGCNT] */
	case SIOCGETSGCNT:
		return "SIOCGETSGCNT";
/* #define SIOCALIFADDR _IOW('i', 28, struct if_laddrreq) [SIOCALIFADDR] */
	case SIOCALIFADDR:
		return "SIOCALIFADDR";
/* #define TIOCSLINED _IOW('t', 67, linedn_t) [TIOCSLINED] */
	case TIOCSLINED:
		return "TIOCSLINED";
/* #define SIOCGETVIFCNT _IOWR('u', 51, struct sioc_vif_req) [SIOCGETVIFCNT] */
	case SIOCGETVIFCNT:
		return "SIOCGETVIFCNT";
/* #define TIOCSTART _IO('t', 110) [TIOCSTART] */
	case TIOCSTART:
		return "TIOCSTART";
/* #define DIOCAWEDGE _IOWR('d', 121, struct dkwedge_info) [DIOCAWEDGE] */
	case DIOCAWEDGE:
		return "DIOCAWEDGE";
/* #define SIOCIFGCLONERS _IOWR('i', 120, struct if_clonereq) [SIOCIFGCLONERS] */
	case SIOCIFGCLONERS:
		return "SIOCIFGCLONERS";
/* #define SIOCDELRT _IOW('r', 11, struct ortentry) [SIOCDELRT] */
	case SIOCDELRT:
		return "SIOCDELRT";
/* #define FIOSETOWN _IOW('f', 124, int) [FIOSETOWN] */
	case FIOSETOWN:
		return "FIOSETOWN";
/* #define DIOCSCACHE _IOW('d', 117, int) [DIOCSCACHE] */
	case DIOCSCACHE:
		return "DIOCSCACHE";
/* #define DIOCRFORMAT _IOWR('d', 105, struct format_op) [DIOCRFORMAT] */
	case DIOCRFORMAT:
		return "DIOCRFORMAT";
/* #define SIOCGIFDSTADDR _IOWR('i', 34, struct ifreq) [SIOCGIFDSTADDR] */
	case SIOCGIFDSTADDR:
		return "SIOCGIFDSTADDR";
/* #define FIONREAD _IOR('f', 127, int) [FIONREAD] */
	case FIONREAD:
		return "FIONREAD";
/* #define TIOCSTAT _IOW('t', 101, int) [TIOCSTAT] */
	case TIOCSTAT:
		return "TIOCSTAT";
/* #define SIOCGHIWAT _IOR('s', 1, int) [SIOCGHIWAT] */
	case SIOCGHIWAT:
		return "SIOCGHIWAT";
/* #define DIOCGPART _IOW('d', 104, struct partinfo) [DIOCGPART] */
	case DIOCGPART:
		return "DIOCGPART";
/* #define TIOCMGET _IOR('t', 106, int) [TIOCMGET] */
	case TIOCMGET:
		return "TIOCMGET";
/* #define TIOCNXCL _IO('t', 14) [TIOCNXCL] */
	case TIOCNXCL:
		return "TIOCNXCL";
/* #define SIOCSIFFLAGS _IOW('i', 16, struct ifreq) [SIOCSIFFLAGS] */
	case SIOCSIFFLAGS:
		return "SIOCSIFFLAGS";
/* #define SIOCGLOWAT _IOR('s', 3, int) [SIOCGLOWAT] */
	case SIOCGLOWAT:
		return "SIOCGLOWAT";
/* #define SIOCSIFMTU _IOW('i', 127, struct ifreq) [SIOCSIFMTU] */
	case SIOCSIFMTU:
		return "SIOCSIFMTU";
/* #define SIOCGIFMEDIA _IOWR('i', 54, struct ifmediareq) [SIOCGIFMEDIA] */
	case SIOCGIFMEDIA:
		return "SIOCGIFMEDIA";
/* #define TIOCFLUSH _IOW('t', 16, int) [TIOCFLUSH] */
	case TIOCFLUSH:
		return "TIOCFLUSH";
/* #define DIOCEJECT _IOW('d', 112, int) [DIOCEJECT] */
	case DIOCEJECT:
		return "DIOCEJECT";
/* #define TIOCGWINSZ _IOR('t', 104, struct winsize) [TIOCGWINSZ] */
	case TIOCGWINSZ:
		return "TIOCGWINSZ";
/* #define SIOCGIFCAP _IOWR('i', 118, struct ifcapreq) [SIOCGIFCAP] */
	case SIOCGIFCAP:
		return "SIOCGIFCAP";
/* #define TIOCGPGRP _IOR('t', 119, int) [TIOCGPGRP] */
	case TIOCGPGRP:
		return "TIOCGPGRP";
/* #define SIOCGIFNETMASK _IOWR('i', 37, struct ifreq) [SIOCGIFNETMASK] */
	case SIOCGIFNETMASK:
		return "SIOCGIFNETMASK";
/* #define TIOCPKT _IOW('t', 112, int) [TIOCPKT] */
	case TIOCPKT:
		return "TIOCPKT";
/* #define OSIOCGIFDSTADDR _IOWR('i', 15, struct ifreq) [OSIOCGIFDSTADDR] */
	case OSIOCGIFDSTADDR:
		return "OSIOCGIFDSTADDR";
/* #define DIOCBSFLUSH _IO('d', 120) [DIOCBSFLUSH] */
	case DIOCBSFLUSH:
		return "DIOCBSFLUSH";
/* #define FIONCLEX _IO('f', 2) [FIONCLEX] */
	case FIONCLEX:
		return "FIONCLEX";
/* #define SIOCSLIFPHYADDR _IOW('i', 74, struct if_laddrreq) [SIOCSLIFPHYADDR] */
	case SIOCSLIFPHYADDR:
		return "SIOCSLIFPHYADDR";
/* #define FIOCLEX _IO('f', 1) [FIOCLEX] */
	case FIOCLEX:
		return "FIOCLEX";
/* #define SIOCGIFBRDADDR _IOWR('i', 35, struct ifreq) [SIOCGIFBRDADDR] */
	case SIOCGIFBRDADDR:
		return "SIOCGIFBRDADDR";
/* #define OSIOCGIFCONF _IOWR('i', 20, struct ifconf) [OSIOCGIFCONF] */
	case OSIOCGIFCONF:
		return "OSIOCGIFCONF";
/* #define TIOCPTMGET _IOR('t', 70, struct ptmget) [TIOCPTMGET] */
	case TIOCPTMGET:
		return "TIOCPTMGET";
/* #define TIOCEXT _IOW('t', 96, int) [TIOCEXT] */
	case TIOCEXT:
		return "TIOCEXT";
/* #define SIOCIFDESTROY _IOW('i', 121, struct ifreq) [SIOCIFDESTROY] */
	case SIOCIFDESTROY:
		return "SIOCIFDESTROY";
/* #define DIOCGDINFO _IOR('d', 101, struct disklabel) [DIOCGDINFO] */
	case DIOCGDINFO:
		return "DIOCGDINFO";
/* #define SIOCGIFCONF _IOWR('i', 36, struct ifconf) [SIOCGIFCONF] */
	case SIOCGIFCONF:
		return "SIOCGIFCONF";
/* #define OFIOGETBMAP _IOWR('f', 122, uint32_t) [OFIOGETBMAP] */
	case OFIOGETBMAP:
		return "OFIOGETBMAP";
/* #define TIOCSCTTY _IO('t', 97) [TIOCSCTTY] */
	case TIOCSCTTY:
		return "TIOCSCTTY";
/* #define FIONSPACE _IOR('f', 120, int) [FIONSPACE] */
	case FIONSPACE:
		return "FIONSPACE";
/* #define DIOCCLRLABEL _IO('d', 115) [DIOCCLRLABEL] */
	case DIOCCLRLABEL:
		return "DIOCCLRLABEL";
/* #define FIOGETBMAP _IOWR('f', 122, daddr_t) [FIOGETBMAP] */
	case FIOGETBMAP:
		return "FIOGETBMAP";
/* #define TIOCOUTQ _IOR('t', 115, int) [TIOCOUTQ] */
	case TIOCOUTQ:
		return "TIOCOUTQ";
/* #define TIOCCBRK _IO('t', 122) [TIOCCBRK] */
	case TIOCCBRK:
		return "TIOCCBRK";
/* #define OSIOCGIFNETMASK _IOWR('i', 21, struct ifreq) [OSIOCGIFNETMASK] */
	case OSIOCGIFNETMASK:
		return "OSIOCGIFNETMASK";
/* #define TIOCREMOTE _IOW('t', 105, int) [TIOCREMOTE] */
	case TIOCREMOTE:
		return "TIOCREMOTE";
/* #define SIOCADDRT _IOW('r', 10, struct ortentry) [SIOCADDRT] */
	case SIOCADDRT:
		return "SIOCADDRT";
/* #define SIOCGIFDATA _IOWR('i', 128, struct ifdatareq) [SIOCGIFDATA] */
	case SIOCGIFDATA:
		return "SIOCGIFDATA";
/* #define DIOCWDINFO _IOW('d', 103, struct disklabel) [DIOCWDINFO] */
	case DIOCWDINFO:
		return "DIOCWDINFO";
/* #define SIOCDELMULTI _IOW('i', 50, struct ifreq) [SIOCDELMULTI] */
	case SIOCDELMULTI:
		return "SIOCDELMULTI";
/* #define OSIOCGIFBRDADDR _IOWR('i', 18, struct ifreq) [OSIOCGIFBRDADDR] */
	case OSIOCGIFBRDADDR:
		return "OSIOCGIFBRDADDR";
/* #define TIOCMBIC _IOW('t', 107, int) [TIOCMBIC] */
	case TIOCMBIC:
		return "TIOCMBIC";
/* #define TIOCMBIS _IOW('t', 108, int) [TIOCMBIS] */
	case TIOCMBIS:
		return "TIOCMBIS";
/* #define SIOCSIFGENERIC _IOW('i', 57, struct ifreq) [SIOCSIFGENERIC] */
	case SIOCSIFGENERIC:
		return "SIOCSIFGENERIC";
/* #define SIOCSDRVSPEC _IOW('i', 123, struct ifdrv) [SIOCSDRVSPEC] */
	case SIOCSDRVSPEC:
		return "SIOCSDRVSPEC";
/* #define TIOCSETA _IOW('t', 20, struct termios) [TIOCSETA] */
	case TIOCSETA:
		return "TIOCSETA";
/* #define TIOCSETD _IOW('t', 27, int) [TIOCSETD] */
	case TIOCSETD:
		return "TIOCSETD";
/* #define TIOCCONS _IOW('t', 98, int) [TIOCCONS] */
	case TIOCCONS:
		return "TIOCCONS";
/* #define TIOCSPGRP _IOW('t', 118, int) [TIOCSPGRP] */
	case TIOCSPGRP:
		return "TIOCSPGRP";
/* #define TIOCGSID _IOR('t', 99, int) [TIOCGSID] */
	case TIOCGSID:
		return "TIOCGSID";
/* #define _IO(g,n) _IOC(IOC_VOID, (g), (n), 0) [_IO(g,n)] */
/* #define TIOCCDTR _IO('t', 120) [TIOCCDTR] */
	case TIOCCDTR:
		return "TIOCCDTR";
/* #define SIOCATMARK _IOR('s', 7, int) [SIOCATMARK] */
	case SIOCATMARK:
		return "SIOCATMARK";
/* #define SIOCSLOWAT _IOW('s', 2, int) [SIOCSLOWAT] */
	case SIOCSLOWAT:
		return "SIOCSLOWAT";
/* #define PTIOCLINUX _IOW('Z', 3, struct ioctl_pt) [PTIOCLINUX] */
	case PTIOCLINUX:
		return "PTIOCLINUX";
/* #define DIOCSBAD _IOW('d', 110, struct dkbad) [DIOCSBAD] */
	case DIOCSBAD:
		return "DIOCSBAD";
/* #define SIOCSIFADDR _IOW('i', 12, struct ifreq) [SIOCSIFADDR] */
	case SIOCSIFADDR:
		return "SIOCSIFADDR";
/* #define TIOCSWINSZ _IOW('t', 103, struct winsize) [TIOCSWINSZ] */
	case TIOCSWINSZ:
		return "TIOCSWINSZ";
/* #define SIOCSIFCAP _IOW('i', 117, struct ifcapreq) [SIOCSIFCAP] */
	case SIOCSIFCAP:
		return "SIOCSIFCAP";
/* #define SIOCGIFFLAGS _IOWR('i', 17, struct ifreq) [SIOCGIFFLAGS] */
	case SIOCGIFFLAGS:
		return "SIOCGIFFLAGS";
/* #define TIOCSETAF _IOW('t', 22, struct termios) [TIOCSETAF] */
	case TIOCSETAF:
		return "TIOCSETAF";
/* #define TIOCSETAW _IOW('t', 21, struct termios) [TIOCSETAW] */
	case TIOCSETAW:
		return "TIOCSETAW";
/* #define _IOC(inout,group,num,len) (inout | ((len & IOCPARM_MASK) << 16) | ((group) << 8) | (num)) [_IOC(inout,group,num,len)] */
/* #define SIOCSIFMETRIC _IOW('i', 24, struct ifreq) [SIOCSIFMETRIC] */
	case SIOCSIFMETRIC:
		return "SIOCSIFMETRIC";
/* #define _IOR(g,n,t) _IOC(IOC_OUT, (g), (n), sizeof(t)) [_IOR(g,n,t)] */
/* #define _IOW(g,n,t) _IOC(IOC_IN, (g), (n), sizeof(t)) [_IOW(g,n,t)] */
/* #define ODIOCEJECT _IO('d', 112) [ODIOCEJECT] */
	case ODIOCEJECT:
		return "ODIOCEJECT";
/* #define SIOCGIFALIAS _IOWR('i', 27, struct ifaliasreq) [SIOCGIFALIAS] */
	case SIOCGIFALIAS:
		return "SIOCGIFALIAS";
/* #define TIOCGFLAGS _IOR('t', 93, int) [TIOCGFLAGS] */
	case TIOCGFLAGS:
		return "TIOCGFLAGS";
/* #define DIOCKLABEL _IOW('d', 119, int) [DIOCKLABEL] */
	case DIOCKLABEL:
		return "DIOCKLABEL";
/* #define DIOCDWEDGE _IOW('d', 123, struct dkwedge_info) [DIOCDWEDGE] */
	case DIOCDWEDGE:
		return "DIOCDWEDGE";
/* #define TIOCNOTTY _IO('t', 113) [TIOCNOTTY] */
	case TIOCNOTTY:
		return "TIOCNOTTY";
/* #define TIOCSTOP _IO('t', 111) [TIOCSTOP] */
	case TIOCSTOP:
		return "TIOCSTOP";
/* #define PTIOCFREEBSD _IOW('Z', 4, struct ioctl_pt) [PTIOCFREEBSD] */
	case PTIOCFREEBSD:
		return "PTIOCFREEBSD";
/* #define DIOCSDINFO _IOW('d', 102, struct disklabel) [DIOCSDINFO] */
	case DIOCSDINFO:
		return "DIOCSDINFO";
/* #define TIOCDCDTIMESTAMP _IOR('t', 88, struct timeval) [TIOCDCDTIMESTAMP] */
	case TIOCDCDTIMESTAMP:
		return "TIOCDCDTIMESTAMP";
/* #define SIOCIFCREATE _IOW('i', 122, struct ifreq) [SIOCIFCREATE] */
	case SIOCIFCREATE:
		return "SIOCIFCREATE";
/* #define DIOCLOCK _IOW('d', 113, int) [DIOCLOCK] */
	case DIOCLOCK:
		return "DIOCLOCK";
/* #define SIOCSIFPHYADDR _IOW('i', 70, struct ifaliasreq) [SIOCSIFPHYADDR] */
	case SIOCSIFPHYADDR:
		return "SIOCSIFPHYADDR";
/* #define DIOCBSLIST _IOWR('d', 119, struct disk_badsecinfo) [DIOCBSLIST] */
	case DIOCBSLIST:
		return "DIOCBSLIST";
/* #define DIOCSRETRIES _IOW('d', 108, int) [DIOCSRETRIES] */
/* #define FIONBIO _IOW('f', 126, int) [FIONBIO] */
/* #define DIOCLWEDGES _IOWR('d', 124, struct dkwedge_list) [DIOCLWEDGES] */
/* #define TIOCGLINED _IOR('t', 66, linedn_t) [TIOCGLINED] */
/* #define FIONWRITE _IOR('f', 121, int) [FIONWRITE] */
/* #define TIOCMSET _IOW('t', 109, int) [TIOCMSET] */
/* #define TIOCRCVFRAME _IOW('t', 69, struct mbuf *) [TIOCRCVFRAME] */
/* #define TIOCDRAIN _IO('t', 94) [TIOCDRAIN] */
/* #define TIOCEXCL _IO('t', 13) [TIOCEXCL] */
/* #define TIOCPKT_IOCTL 0x40 [TIOCPKT_IOCTL] */
/* #define FIOGETOWN _IOR('f', 123, int) [FIOGETOWN] */
/* #define SIOCGPGRP _IOR('s', 9, int) [SIOCGPGRP] */
/* #define DIOCGCACHE _IOR('d', 116, int) [DIOCGCACHE] */
/* #define TIOCUCNTL _IOW('t', 102, int) [TIOCUCNTL] */
/* #define SIOCGIFADDR _IOWR('i', 33, struct ifreq) [SIOCGIFADDR] */
/* #define SIOCGIFMETRIC _IOWR('i', 23, struct ifreq) [SIOCGIFMETRIC] */
/* #define OSIOCGIFADDR _IOWR('i', 13, struct ifreq) [OSIOCGIFADDR] */
/* #define SIOCGIFGENERIC _IOWR('i', 58, struct ifreq) [SIOCGIFGENERIC] */
/* #define SIOCGIFPSRCADDR _IOWR('i', 71, struct ifreq) [SIOCGIFPSRCADDR] */
/* #define SIOCSIFDSTADDR _IOW('i', 14, struct ifreq) [SIOCSIFDSTADDR] */
/* #define SIOCGIFPDSTADDR _IOWR('i', 72, struct ifreq) [SIOCGIFPDSTADDR] */
/* #define DIOCGDEFLABEL _IOR('d', 114, struct disklabel) [DIOCGDEFLABEL] */
/* #define FIOASYNC _IOW('f', 125, int) [FIOASYNC] */
/* #define SIOCSHIWAT _IOW('s', 0, int) [SIOCSHIWAT] */
/* #define SIOCSIFMEDIA _IOWR('i', 53, struct ifreq) [SIOCSIFMEDIA] */
/* #define TIOCSBRK _IO('t', 123) [TIOCSBRK] */
/* #define DIOCSSTEP _IOW('d', 107, int) [DIOCSSTEP] */
/* #define SIOCDIFPHYADDR _IOW('i', 73, struct ifreq) [SIOCDIFPHYADDR] */
/* #define SIOCGLIFADDR _IOWR('i', 29, struct if_laddrreq) [SIOCGLIFADDR] */
/* #define SIOCGDRVSPEC _IOWR('i', 123, struct ifdrv) [SIOCGDRVSPEC] */
/* #define SIOCDIFADDR _IOW('i', 25, struct ifreq) [SIOCDIFADDR] */
/* #define TIOCGRANTPT _IO('t', 71) [TIOCGRANTPT] */
/* #define SIOCGIFMTU _IOWR('i', 126, struct ifreq) [SIOCGIFMTU] */
/* #define SIOCGIFDLT _IOWR('i', 119, struct ifreq) [SIOCGIFDLT] */
/* #define TIOCSIG _IO('t', 95) [TIOCSIG] */
/* #define PTIOCNETBSD _IOW('Z', 0, struct ioctl_pt) [PTIOCNETBSD] */
/* #define _IOWR(g,n,t) _IOC(IOC_INOUT, (g), (n), sizeof(t)) [_IOWR(g,n,t)] */
/* #define TIOCGETA _IOR('t', 19, struct termios) [TIOCGETA] */
/* #define TIOCGETD _IOR('t', 26, int) [TIOCGETD] */
/* #define PTIOCULTRIX _IOW('Z', 6, struct ioctl_pt) [PTIOCULTRIX] */
/* #define SIOCSIFNETMASK _IOW('i', 22, struct ifreq) [SIOCSIFNETMASK] */
/* #define SIOCAIFADDR _IOW('i', 26, struct ifaliasreq) [SIOCAIFADDR] */
/* #define DIOCCACHESYNC _IOW('d', 118, int) [DIOCCACHESYNC] */
/* #define TIOCSDTR _IO('t', 121) [TIOCSDTR] */
/* #define SIOCDLIFADDR _IOW('i', 30, struct if_laddrreq) [SIOCDLIFADDR] */
/* #define DIOCGWEDGEINFO _IOR('d', 122, struct dkwedge_info) [DIOCGWEDGEINFO] */
/* #define TIOCXMTFRAME _IOW('t', 68, struct mbuf *) [TIOCXMTFRAME] */
/* #define TIOCSFLAGS _IOW('t', 92, int) [TIOCSFLAGS] */
/* #define DIOCWLABEL _IOW('d', 109, int) [DIOCWLABEL] */
/* #define TIOCSTI _IOW('t', 114, char) [TIOCSTI] */
/* #define SIOCSIFBRDADDR _IOW('i', 19, struct ifreq) [SIOCSIFBRDADDR] */
/* #define SIOCGLIFPHYADDR _IOWR('i', 75, struct if_laddrreq) [SIOCGLIFPHYADDR] */
/* #define DIOCWFORMAT _IOWR('d', 106, struct format_op) [DIOCWFORMAT] */
/* #define SIOCSPGRP _IOW('s', 8, int) [SIOCSPGRP] */
/* #define TIOCPTSNAME _IOR('t', 72, struct ptmget) [TIOCPTSNAME] */
/* #define PTIOCSUNOS _IOW('Z', 1, struct ioctl_pt) [PTIOCSUNOS] */
/* #define SIOCADDMULTI _IOW('i', 49, struct ifreq) [SIOCADDMULTI] */
/* #define SIOCZIFDATA _IOWR('i', 129, struct ifdatareq) [SIOCZIFDATA] */
/* #define SIOCGETSGCNT _IOWR('u', 52, struct sioc_sg_req) [SIOCGETSGCNT] */
/* #define SIOCALIFADDR _IOW('i', 28, struct if_laddrreq) [SIOCALIFADDR] */
/* #define TIOCSLINED _IOW('t', 67, linedn_t) [TIOCSLINED] */
/* #define SIOCGETVIFCNT _IOWR('u', 51, struct sioc_vif_req) [SIOCGETVIFCNT] */
/* #define TIOCSTART _IO('t', 110) [TIOCSTART] */
/* #define DIOCAWEDGE _IOWR('d', 121, struct dkwedge_info) [DIOCAWEDGE] */
/* #define SIOCIFGCLONERS _IOWR('i', 120, struct if_clonereq) [SIOCIFGCLONERS] */
/* #define SIOCDELRT _IOW('r', 11, struct ortentry) [SIOCDELRT] */
/* #define FIOSETOWN _IOW('f', 124, int) [FIOSETOWN] */
/* #define DIOCSCACHE _IOW('d', 117, int) [DIOCSCACHE] */
/* #define DIOCRFORMAT _IOWR('d', 105, struct format_op) [DIOCRFORMAT] */
/* #define SIOCGIFDSTADDR _IOWR('i', 34, struct ifreq) [SIOCGIFDSTADDR] */
/* #define FIONREAD _IOR('f', 127, int) [FIONREAD] */
/* #define TIOCSTAT _IOW('t', 101, int) [TIOCSTAT] */
/* #define SIOCGHIWAT _IOR('s', 1, int) [SIOCGHIWAT] */
/* #define DIOCGPART _IOW('d', 104, struct partinfo) [DIOCGPART] */
/* #define TIOCMGET _IOR('t', 106, int) [TIOCMGET] */
/* #define TIOCNXCL _IO('t', 14) [TIOCNXCL] */
/* #define SIOCSIFFLAGS _IOW('i', 16, struct ifreq) [SIOCSIFFLAGS] */
/* #define SIOCGLOWAT _IOR('s', 3, int) [SIOCGLOWAT] */
/* #define SIOCSIFMTU _IOW('i', 127, struct ifreq) [SIOCSIFMTU] */
/* #define SIOCGIFMEDIA _IOWR('i', 54, struct ifmediareq) [SIOCGIFMEDIA] */
/* #define TIOCFLUSH _IOW('t', 16, int) [TIOCFLUSH] */
/* #define DIOCEJECT _IOW('d', 112, int) [DIOCEJECT] */
/* #define TIOCGWINSZ _IOR('t', 104, struct winsize) [TIOCGWINSZ] */
/* #define SIOCGIFCAP _IOWR('i', 118, struct ifcapreq) [SIOCGIFCAP] */
/* #define TIOCGPGRP _IOR('t', 119, int) [TIOCGPGRP] */
/* #define SIOCGIFNETMASK _IOWR('i', 37, struct ifreq) [SIOCGIFNETMASK] */
/* #define TIOCPKT _IOW('t', 112, int) [TIOCPKT] */
/* #define OSIOCGIFDSTADDR _IOWR('i', 15, struct ifreq) [OSIOCGIFDSTADDR] */
/* #define DIOCBSFLUSH _IO('d', 120) [DIOCBSFLUSH] */
/* #define FIONCLEX _IO('f', 2) [FIONCLEX] */
/* #define SIOCSLIFPHYADDR _IOW('i', 74, struct if_laddrreq) [SIOCSLIFPHYADDR] */
/* #define FIOCLEX _IO('f', 1) [FIOCLEX] */
/* #define SIOCGIFBRDADDR _IOWR('i', 35, struct ifreq) [SIOCGIFBRDADDR] */
/* #define OSIOCGIFCONF _IOWR('i', 20, struct ifconf) [OSIOCGIFCONF] */
/* #define TIOCPTMGET _IOR('t', 70, struct ptmget) [TIOCPTMGET] */
/* #define TIOCEXT _IOW('t', 96, int) [TIOCEXT] */
/* #define SIOCIFDESTROY _IOW('i', 121, struct ifreq) [SIOCIFDESTROY] */
/* #define DIOCGDINFO _IOR('d', 101, struct disklabel) [DIOCGDINFO] */
/* #define SIOCGIFCONF _IOWR('i', 36, struct ifconf) [SIOCGIFCONF] */
/* #define OFIOGETBMAP _IOWR('f', 122, uint32_t) [OFIOGETBMAP] */
/* #define TIOCSCTTY _IO('t', 97) [TIOCSCTTY] */
/* #define FIONSPACE _IOR('f', 120, int) [FIONSPACE] */
/* #define DIOCCLRLABEL _IO('d', 115) [DIOCCLRLABEL] */
/* #define FIOGETBMAP _IOWR('f', 122, daddr_t) [FIOGETBMAP] */
/* #define TIOCOUTQ _IOR('t', 115, int) [TIOCOUTQ] */
/* #define TIOCCBRK _IO('t', 122) [TIOCCBRK] */
/* #define OSIOCGIFNETMASK _IOWR('i', 21, struct ifreq) [OSIOCGIFNETMASK] */
/* #define TIOCREMOTE _IOW('t', 105, int) [TIOCREMOTE] */
/* #define SIOCADDRT _IOW('r', 10, struct ortentry) [SIOCADDRT] */
/* #define SIOCGIFDATA _IOWR('i', 128, struct ifdatareq) [SIOCGIFDATA] */
/* #define DIOCWDINFO _IOW('d', 103, struct disklabel) [DIOCWDINFO] */
/* #define SIOCDELMULTI _IOW('i', 50, struct ifreq) [SIOCDELMULTI] */
/* #define OSIOCGIFBRDADDR _IOWR('i', 18, struct ifreq) [OSIOCGIFBRDADDR] */
/* #define TIOCMBIC _IOW('t', 107, int) [TIOCMBIC] */
/* #define TIOCMBIS _IOW('t', 108, int) [TIOCMBIS] */
/* #define SIOCSIFGENERIC _IOW('i', 57, struct ifreq) [SIOCSIFGENERIC] */
/* #define SIOCSDRVSPEC _IOW('i', 123, struct ifdrv) [SIOCSDRVSPEC] */
/* #define TIOCSETA _IOW('t', 20, struct termios) [TIOCSETA] */
/* #define TIOCSETD _IOW('t', 27, int) [TIOCSETD] */
/* #define TIOCCONS _IOW('t', 98, int) [TIOCCONS] */
/* #define TIOCSPGRP _IOW('t', 118, int) [TIOCSPGRP] */
/* #define TIOCGSID _IOR('t', 99, int) [TIOCGSID] */
/* #define _IO(g,n) _IOC(IOC_VOID, (g), (n), 0) [_IO(g,n)] */
/* #define TIOCCDTR _IO('t', 120) [TIOCCDTR] */
/* #define SIOCATMARK _IOR('s', 7, int) [SIOCATMARK] */
/* #define SIOCSLOWAT _IOW('s', 2, int) [SIOCSLOWAT] */
/* #define PTIOCLINUX _IOW('Z', 3, struct ioctl_pt) [PTIOCLINUX] */
/* #define DIOCSBAD _IOW('d', 110, struct dkbad) [DIOCSBAD] */
/* #define SIOCSIFADDR _IOW('i', 12, struct ifreq) [SIOCSIFADDR] */
/* #define TIOCSWINSZ _IOW('t', 103, struct winsize) [TIOCSWINSZ] */
/* #define SIOCSIFCAP _IOW('i', 117, struct ifcapreq) [SIOCSIFCAP] */
/* #define SIOCGIFFLAGS _IOWR('i', 17, struct ifreq) [SIOCGIFFLAGS] */
/* #define TIOCSETAF _IOW('t', 22, struct termios) [TIOCSETAF] */
/* #define TIOCSETAW _IOW('t', 21, struct termios) [TIOCSETAW] */
/* #define _IOC(inout,group,num,len) (inout | ((len & IOCPARM_MASK) << 16) | ((group) << 8) | (num)) [_IOC(inout,group,num,len)] */
/* #define SIOCSIFMETRIC _IOW('i', 24, struct ifreq) [SIOCSIFMETRIC] */
/* #define _IOR(g,n,t) _IOC(IOC_OUT, (g), (n), sizeof(t)) [_IOR(g,n,t)] */
/* #define _IOW(g,n,t) _IOC(IOC_IN, (g), (n), sizeof(t)) [_IOW(g,n,t)] */
/* #define ODIOCEJECT _IO('d', 112) [ODIOCEJECT] */
/* #define SIOCGIFALIAS _IOWR('i', 27, struct ifaliasreq) [SIOCGIFALIAS] */
/* #define TIOCGFLAGS _IOR('t', 93, int) [TIOCGFLAGS] */
/* #define DIOCKLABEL _IOW('d', 119, int) [DIOCKLABEL] */
/* #define DIOCDWEDGE _IOW('d', 123, struct dkwedge_info) [DIOCDWEDGE] */
/* #define TIOCNOTTY _IO('t', 113) [TIOCNOTTY] */
/* #define TIOCSTOP _IO('t', 111) [TIOCSTOP] */
/* #define PTIOCFREEBSD _IOW('Z', 4, struct ioctl_pt) [PTIOCFREEBSD] */
/* #define DIOCSDINFO _IOW('d', 102, struct disklabel) [DIOCSDINFO] */
/* #define TIOCDCDTIMESTAMP _IOR('t', 88, struct timeval) [TIOCDCDTIMESTAMP] */
/* #define SIOCIFCREATE _IOW('i', 122, struct ifreq) [SIOCIFCREATE] */
/* #define DIOCLOCK _IOW('d', 113, int) [DIOCLOCK] */
/* #define SIOCSIFPHYADDR _IOW('i', 70, struct ifaliasreq) [SIOCSIFPHYADDR] */
/* #define DIOCBSLIST _IOWR('d', 119, struct disk_badsecinfo) [DIOCBSLIST] */
/* #define BIOCIMMEDIATE _IOW('B',112, u_int) [BIOCIMMEDIATE] */
	case BIOCIMMEDIATE:
		return "BIOCIMMEDIATE";
/* #define BIOCGDLT _IOR('B',106, u_int) [BIOCGDLT] */
	case BIOCGDLT:
		return "BIOCGDLT";
/* #define BIOCSETIF _IOW('B',108, struct ifreq) [BIOCSETIF] */
	case BIOCSETIF:
		return "BIOCSETIF";
/* #define BIOCPROMISC _IO('B',105) [BIOCPROMISC] */
	case BIOCPROMISC:
		return "BIOCPROMISC";
/* #define BIOCSRTIMEOUT _IOW('B',109, struct timeval) [BIOCSRTIMEOUT] */
	case BIOCSRTIMEOUT:
		return "BIOCSRTIMEOUT";
/* #define BIOCSETF _IOW('B',103, struct bpf_program) [BIOCSETF] */
	case BIOCSETF:
		return "BIOCSETF";
/* #define BIOCGBLEN _IOR('B',102, u_int) [BIOCGBLEN] */
	case BIOCGBLEN:
		return "BIOCGBLEN";
/* #define BIOCGSTATSOLD _IOR('B',111, struct bpf_stat_old) [BIOCGSTATSOLD] */
	case BIOCGSTATSOLD:
		return "BIOCGSTATSOLD";
/* #define BIOCGRTIMEOUT _IOR('B',110, struct timeval) [BIOCGRTIMEOUT] */
	case BIOCGRTIMEOUT:
		return "BIOCGRTIMEOUT";
/* #define BIOCGSTATS _IOR('B',111, struct bpf_stat) [BIOCGSTATS] */
	case BIOCGSTATS:
		return "BIOCGSTATS";
/* #define BIOCVERSION _IOR('B',113, struct bpf_version) [BIOCVERSION] */
	case BIOCVERSION:
		return "BIOCVERSION";
/* #define BIOCSSEESENT _IOW('B',121, u_int) [BIOCSSEESENT] */
	case BIOCSSEESENT:
		return "BIOCSSEESENT";
/* #define BIOCSUDPF _IOW('B',115, struct bpf_program) [BIOCSUDPF] */
	case BIOCSUDPF:
		return "BIOCSUDPF";
/* #define BIOCGDLTLIST _IOWR('B',119, struct bpf_dltlist) [BIOCGDLTLIST] */
	case BIOCGDLTLIST:
		return "BIOCGDLTLIST";
/* #define BIOCSBLEN _IOWR('B',102, u_int) [BIOCSBLEN] */
	case BIOCSBLEN:
		return "BIOCSBLEN";
/* #define BIOCSHDRCMPLT _IOW('B',117, u_int) [BIOCSHDRCMPLT] */
	case BIOCSHDRCMPLT:
		return "BIOCSHDRCMPLT";
/* #define BIOCFLUSH _IO('B',104) [BIOCFLUSH] */
	case BIOCFLUSH:
		return "BIOCFLUSH";
/* #define BIOCSDLT _IOW('B',118, u_int) [BIOCSDLT] */
	case BIOCSDLT:
		return "BIOCSDLT";
/* #define BIOCGHDRCMPLT _IOR('B',116, u_int) [BIOCGHDRCMPLT] */
	case BIOCGHDRCMPLT:
		return "BIOCGHDRCMPLT";
/* #define BIOCSTCPF _IOW('B',114, struct bpf_program) [BIOCSTCPF] */
	case BIOCSTCPF:
		return "BIOCSTCPF";
/* #define BIOCGSEESENT _IOR('B',120, u_int) [BIOCGSEESENT] */
	case BIOCGSEESENT:
		return "BIOCGSEESENT";
/* #define BIOCGETIF _IOR('B',107, struct ifreq) [BIOCGETIF] */
	case BIOCGETIF:
		return "BIOCGETIF";
/* #define GREGPROTO _IOWR('i', 106, struct ifreq) [GREGPROTO] */
	case GREGPROTO:
		return "GREGPROTO";
/* #define GREGADDRD _IOWR('i', 104, struct ifreq) [GREGADDRD] */
	case GREGADDRD:
		return "GREGADDRD";
/* #define GREGADDRS _IOWR('i', 103, struct ifreq) [GREGADDRS] */
	case GREGADDRS:
		return "GREGADDRS";
/* #define GRESPROTO _IOW('i' , 105, struct ifreq) [GRESPROTO] */
	case GRESPROTO:
		return "GRESPROTO";
/* #define GRESADDRD _IOW('i', 102, struct ifreq) [GRESADDRD] */
	case GRESADDRD:
		return "GRESADDRD";
/* #define GRESADDRS _IOW('i', 101, struct ifreq) [GRESADDRS] */
	case GRESADDRS:
		return "GRESADDRS";
/* #define PPPIOCGASYNCMAP _IOR('t', 88, int) [PPPIOCGASYNCMAP] */
	case PPPIOCGASYNCMAP:
		return "PPPIOCGASYNCMAP";
/* #define SIOCGPPPCSTATS _IOWR('i', 122, struct ifpppcstatsreq) [SIOCGPPPCSTATS] */
	case SIOCGPPPCSTATS:
		return "SIOCGPPPCSTATS";
/* #define PPPIOCGMRU _IOR('t', 83, int) [PPPIOCGMRU] */
	case PPPIOCGMRU:
		return "PPPIOCGMRU";
/* #define PPPIOCGFLAGS _IOR('t', 90, int) [PPPIOCGFLAGS] */
	case PPPIOCGFLAGS:
		return "PPPIOCGFLAGS";
/* #define PPPIOCSCOMPRESS _IOW('t', 77, struct ppp_option_data) [PPPIOCSCOMPRESS] */
	case PPPIOCSCOMPRESS:
		return "PPPIOCSCOMPRESS";
/* #define PPPIOCGMTU _IOR('t', 73, int) [PPPIOCGMTU] */
	case PPPIOCGMTU:
		return "PPPIOCGMTU";
/* #define PPPIOCGNPMODE _IOWR('t', 76, struct npioctl) [PPPIOCGNPMODE] */
	case PPPIOCGNPMODE:
		return "PPPIOCGNPMODE";
/* #define PPPIOCSMAXCID _IOW('t', 81, int) [PPPIOCSMAXCID] */
	case PPPIOCSMAXCID:
		return "PPPIOCSMAXCID";
/* #define PPPIOCSRASYNCMAP _IOW('t', 84, int) [PPPIOCSRASYNCMAP] */
	case PPPIOCSRASYNCMAP:
		return "PPPIOCSRASYNCMAP";
/* #define PPPIOCSFLAGS _IOW('t', 89, int) [PPPIOCSFLAGS] */
	case PPPIOCSFLAGS:
		return "PPPIOCSFLAGS";
/* #define PPPIOCGIDLE _IOR('t', 74, struct ppp_idle) [PPPIOCGIDLE] */
	case PPPIOCGIDLE:
		return "PPPIOCGIDLE";
/* #define PPPIOCGUNIT _IOR('t', 86, int) [PPPIOCGUNIT] */
	case PPPIOCGUNIT:
		return "PPPIOCGUNIT";
/* #define PPPIOCGXASYNCMAP _IOR('t', 80, ext_accm) [PPPIOCGXASYNCMAP] */
	case PPPIOCGXASYNCMAP:
		return "PPPIOCGXASYNCMAP";
/* #define PPPIOCGRAWIN _IOR('t', 91, struct ppp_rawin) [PPPIOCGRAWIN] */
	case PPPIOCGRAWIN:
		return "PPPIOCGRAWIN";
/* #define PPPIOCGRASYNCMAP _IOR('t', 85, int) [PPPIOCGRASYNCMAP] */
	case PPPIOCGRASYNCMAP:
		return "PPPIOCGRASYNCMAP";
/* #define SIOCGPPPSTATS _IOWR('i', 123, struct ifpppstatsreq) [SIOCGPPPSTATS] */
	case SIOCGPPPSTATS:
		return "SIOCGPPPSTATS";
/* #define PPPIOCSMRU _IOW('t', 82, int) [PPPIOCSMRU] */
	case PPPIOCSMRU:
		return "PPPIOCSMRU";
/* #define PPPIOCSMTU _IOW('t', 72, int) [PPPIOCSMTU] */
	case PPPIOCSMTU:
		return "PPPIOCSMTU";
/* #define PPPIOCSXASYNCMAP _IOW('t', 79, ext_accm) [PPPIOCSXASYNCMAP] */
	case PPPIOCSXASYNCMAP:
		return "PPPIOCSXASYNCMAP";
/* #define PPPIOCSASYNCMAP _IOW('t', 87, int) [PPPIOCSASYNCMAP] */
	case PPPIOCSASYNCMAP:
		return "PPPIOCSASYNCMAP";
/* #define PPPIOCXFERUNIT _IO('t', 78) [PPPIOCXFERUNIT] */
	case PPPIOCXFERUNIT:
		return "PPPIOCXFERUNIT";
/* #define PPPIOCSNPMODE _IOW('t', 75, struct npioctl) [PPPIOCSNPMODE] */
	case PPPIOCSNPMODE:
		return "PPPIOCSNPMODE";
/* #define PPPOESETPARMS _IOW('i', 110, struct pppoediscparms) [PPPOESETPARMS] */
	case PPPOESETPARMS:
		return "PPPOESETPARMS";
/* #define PPPOEGETSESSION _IOWR('i', 112, struct pppoeconnectionstate) [PPPOEGETSESSION] */
	case PPPOEGETSESSION:
		return "PPPOEGETSESSION";
/* #define PPPOEGETPARMS _IOWR('i', 111, struct pppoediscparms) [PPPOEGETPARMS] */
	case PPPOEGETPARMS:
		return "PPPOEGETPARMS";
/* #define PPPOEGETIDLETIMEOUT _IOWR('i', 113, struct pppoeidletimeoutcfg) [PPPOEGETIDLETIMEOUT] */
	case PPPOEGETIDLETIMEOUT:
		return "PPPOEGETIDLETIMEOUT";
/* #define PPPOESETIDLETIMEOUT _IOW('i', 114, struct pppoeidletimeoutcfg) [PPPOESETIDLETIMEOUT] */
	case PPPOESETIDLETIMEOUT:
		return "PPPOESETIDLETIMEOUT";
/* #define SPPPGETDNSADDRS _IOWR('i', 131, struct spppdnsaddrs) [SPPPGETDNSADDRS] */
	case SPPPGETDNSADDRS:
		return "SPPPGETDNSADDRS";
/* #define SPPPSETKEEPALIVE _IOW('i', 132, struct spppkeepalivesettings) [SPPPSETKEEPALIVE] */
	case SPPPSETKEEPALIVE:
		return "SPPPSETKEEPALIVE";
/* #define SPPPSETIDLETO _IOW('i', 126, struct spppidletimeout) [SPPPSETIDLETO] */
	case SPPPSETIDLETO:
		return "SPPPSETIDLETO";
/* #define SPPPSETDNSOPTS _IOW('i', 129, struct spppdnssettings) [SPPPSETDNSOPTS] */
	case SPPPSETDNSOPTS:
		return "SPPPSETDNSOPTS";
/* #define SPPPSETAUTHCFG _IOW('i', 121, struct spppauthcfg) [SPPPSETAUTHCFG] */
	case SPPPSETAUTHCFG:
		return "SPPPSETAUTHCFG";
/* #define SPPPGETSTATUSNCP _IOWR('i', 134, struct spppstatusncp) [SPPPGETSTATUSNCP] */
	case SPPPGETSTATUSNCP:
		return "SPPPGETSTATUSNCP";
/* #define SPPPGETSTATUS _IOWR('i', 124, struct spppstatus) [SPPPGETSTATUS] */
	case SPPPGETSTATUS:
		return "SPPPGETSTATUS";
/* #define SPPPGETIDLETO _IOWR('i', 125, struct spppidletimeout) [SPPPGETIDLETO] */
	case SPPPGETIDLETO:
		return "SPPPGETIDLETO";
/* #define SPPPGETKEEPALIVE _IOWR('i', 133, struct spppkeepalivesettings) [SPPPGETKEEPALIVE] */
	case SPPPGETKEEPALIVE:
		return "SPPPGETKEEPALIVE";
/* #define SPPPGETDNSOPTS _IOWR('i', 130, struct spppdnssettings) [SPPPGETDNSOPTS] */
	case SPPPGETDNSOPTS:
		return "SPPPGETDNSOPTS";
/* #define SPPPSETLCPCFG _IOW('i', 123, struct sppplcpcfg) [SPPPSETLCPCFG] */
	case SPPPSETLCPCFG:
		return "SPPPSETLCPCFG";
/* #define SPPPGETAUTHCFG _IOWR('i', 120, struct spppauthcfg) [SPPPGETAUTHCFG] */
	case SPPPGETAUTHCFG:
		return "SPPPGETAUTHCFG";
/* #define SPPPGETLCPCFG _IOWR('i', 122, struct sppplcpcfg) [SPPPGETLCPCFG] */
	case SPPPGETLCPCFG:
		return "SPPPGETLCPCFG";
/* #define SPPPSETAUTHFAILURE _IOW('i', 128, struct spppauthfailuresettings) [SPPPSETAUTHFAILURE] */
	case SPPPSETAUTHFAILURE:
		return "SPPPSETAUTHFAILURE";
/* #define SPPPGETAUTHFAILURES _IOWR('i', 127, struct spppauthfailurestats) [SPPPGETAUTHFAILURES] */
	case SPPPGETAUTHFAILURES:
		return "SPPPGETAUTHFAILURES";
/* #define TAPGIFNAME _IOR('e', 0, struct ifreq) [TAPGIFNAME] */
	case TAPGIFNAME:
		return "TAPGIFNAME";
/* #define TUNSDEBUG _IOW('t', 90, int) [TUNSDEBUG] */
	case TUNSDEBUG:
		return "TUNSDEBUG";
/* #define TUNSIFMODE _IOW('t', 88, int) [TUNSIFMODE] */
	case TUNSIFMODE:
		return "TUNSIFMODE";
/* #define TUNSLMODE _IOW('t', 87, int) [TUNSLMODE] */
/* #define TUNGDEBUG _IOR('t', 89, int) [TUNGDEBUG] */
	case TUNGDEBUG:
		return "TUNGDEBUG";
/* #define DIOCRTSTADDRS _IOWR('D', 73, struct pfioc_table) [DIOCRTSTADDRS] */
	case DIOCRTSTADDRS:
		return "DIOCRTSTADDRS";
/* #define DIOCBEGINADDRS _IOWR('D', 51, struct pfioc_pooladdr) [DIOCBEGINADDRS] */
	case DIOCBEGINADDRS:
		return "DIOCBEGINADDRS";
/* #define DIOCSTART _IO ('D', 1) [DIOCSTART] */
	case DIOCSTART:
		return "DIOCSTART";
/* #define DIOCRGETADDRS _IOWR('D', 70, struct pfioc_table) [DIOCRGETADDRS] */
	case DIOCRGETADDRS:
		return "DIOCRGETADDRS";
/* #define DIOCRDELTABLES _IOWR('D', 62, struct pfioc_table) [DIOCRDELTABLES] */
	case DIOCRDELTABLES:
		return "DIOCRDELTABLES";
/* #define DIOCIGETIFACES _IOWR('D', 87, struct pfioc_iface) [DIOCIGETIFACES] */
	case DIOCIGETIFACES:
		return "DIOCIGETIFACES";
/* #define DIOCRGETTABLES _IOWR('D', 63, struct pfioc_table) [DIOCRGETTABLES] */
	case DIOCRGETTABLES:
		return "DIOCRGETTABLES";
/* #define DIOCSTOPALTQ _IO ('D', 43) [DIOCSTOPALTQ] */
	case DIOCSTOPALTQ:
		return "DIOCSTOPALTQ";
/* #define BLKDEV_IOSIZE 2048 [BLKDEV_IOSIZE] */
/* #define DIOCXCOMMIT _IOWR('D', 82, struct pfioc_trans) [DIOCXCOMMIT] */
	case DIOCXCOMMIT:
		return "DIOCXCOMMIT";
/* #define DIOCOSFPGET _IOWR('D', 80, struct pf_osfp_ioctl) [DIOCOSFPGET] */
	case DIOCOSFPGET:
		return "DIOCOSFPGET";
/* #define DIOCGETSTATUS _IOWR('D', 21, struct pf_status) [DIOCGETSTATUS] */
	case DIOCGETSTATUS:
		return "DIOCGETSTATUS";
/* #define DIOCXROLLBACK _IOWR('D', 83, struct pfioc_trans) [DIOCXROLLBACK] */
	case DIOCXROLLBACK:
		return "DIOCXROLLBACK";
/* #define DIOCGETALTQ _IOWR('D', 48, struct pfioc_altq) [DIOCGETALTQ] */
	case DIOCGETALTQ:
		return "DIOCGETALTQ";
/* #define DIOCRADDADDRS _IOWR('D', 67, struct pfioc_table) [DIOCRADDADDRS] */
	case DIOCRADDADDRS:
		return "DIOCRADDADDRS";
/* #define DIOCGETRULE _IOWR('D', 7, struct pfioc_rule) [DIOCGETRULE] */
	case DIOCGETRULE:
		return "DIOCGETRULE";
/* #define DIOCRADDTABLES _IOWR('D', 61, struct pfioc_table) [DIOCRADDTABLES] */
	case DIOCRADDTABLES:
		return "DIOCRADDTABLES";
/* #define DIOCGETALTQS _IOWR('D', 47, struct pfioc_altq) [DIOCGETALTQS] */
	case DIOCGETALTQS:
		return "DIOCGETALTQS";
/* #define DIOCCLRSRCNODES _IO('D', 85) [DIOCCLRSRCNODES] */
	case DIOCCLRSRCNODES:
		return "DIOCCLRSRCNODES";
/* #define DIOCSETHOSTID _IOWR('D', 86, u_int32_t) [DIOCSETHOSTID] */
	case DIOCSETHOSTID:
		return "DIOCSETHOSTID";
/* #define DIOCSETLIMIT _IOWR('D', 40, struct pfioc_limit) [DIOCSETLIMIT] */
	case DIOCSETLIMIT:
		return "DIOCSETLIMIT";
/* #define DIOCGETRULESET _IOWR('D', 59, struct pfioc_ruleset) [DIOCGETRULESET] */
	case DIOCGETRULESET:
		return "DIOCGETRULESET";
/* #define DIOCCLRSTATES _IOWR('D', 18, struct pfioc_state_kill) [DIOCCLRSTATES] */
	case DIOCCLRSTATES:
		return "DIOCCLRSTATES";
/* #define DIOCICLRISTATS _IOWR('D', 88, struct pfioc_iface) [DIOCICLRISTATS] */
	case DIOCICLRISTATS:
		return "DIOCICLRISTATS";
/* #define DIOCKILLSTATES _IOWR('D', 41, struct pfioc_state_kill) [DIOCKILLSTATES] */
	case DIOCKILLSTATES:
		return "DIOCKILLSTATES";
/* #define DIOCOSFPADD _IOWR('D', 79, struct pf_osfp_ioctl) [DIOCOSFPADD] */
	case DIOCOSFPADD:
		return "DIOCOSFPADD";
/* #define DIOCRSETTFLAGS _IOWR('D', 74, struct pfioc_table) [DIOCRSETTFLAGS] */
	case DIOCRSETTFLAGS:
		return "DIOCRSETTFLAGS";
/* #define DIOCGETQSTATS _IOWR('D', 50, struct pfioc_qstats) [DIOCGETQSTATS] */
	case DIOCGETQSTATS:
		return "DIOCGETQSTATS";
/* #define DIOCRCLRASTATS _IOWR('D', 72, struct pfioc_table) [DIOCRCLRASTATS] */
	case DIOCRCLRASTATS:
		return "DIOCRCLRASTATS";
/* #define DIOCSETSTATUSIF _IOWR('D', 20, struct pfioc_if) [DIOCSETSTATUSIF] */
	case DIOCSETSTATUSIF:
		return "DIOCSETSTATUSIF";
/* #define DIOCRGETTSTATS _IOWR('D', 64, struct pfioc_table) [DIOCRGETTSTATS] */
	case DIOCRGETTSTATS:
		return "DIOCRGETTSTATS";
/* #define DIOCADDALTQ _IOWR('D', 45, struct pfioc_altq) [DIOCADDALTQ] */
	case DIOCADDALTQ:
		return "DIOCADDALTQ";
/* #define DIOCADDRULE _IOWR('D', 4, struct pfioc_rule) [DIOCADDRULE] */
	case DIOCADDRULE:
		return "DIOCADDRULE";
/* #define DIOCSETDEBUG _IOWR('D', 24, u_int32_t) [DIOCSETDEBUG] */
	case DIOCSETDEBUG:
		return "DIOCSETDEBUG";
/* #define DIOCGETLIMIT _IOWR('D', 39, struct pfioc_limit) [DIOCGETLIMIT] */
	case DIOCGETLIMIT:
		return "DIOCGETLIMIT";
/* #define DIOCXBEGIN _IOWR('D', 81, struct pfioc_trans) [DIOCXBEGIN] */
	case DIOCXBEGIN:
		return "DIOCXBEGIN";
/* #define DIOCRDELADDRS _IOWR('D', 68, struct pfioc_table) [DIOCRDELADDRS] */
	case DIOCRDELADDRS:
		return "DIOCRDELADDRS";
/* #define DIOCRCLRTABLES _IOWR('D', 60, struct pfioc_table) [DIOCRCLRTABLES] */
	case DIOCRCLRTABLES:
		return "DIOCRCLRTABLES";
/* #define DIOCCHANGEALTQ _IOWR('D', 49, struct pfioc_altq) [DIOCCHANGEALTQ] */
	case DIOCCHANGEALTQ:
		return "DIOCCHANGEALTQ";
/* #define DIOCCHANGERULE _IOWR('D', 26, struct pfioc_rule) [DIOCCHANGERULE] */
	case DIOCCHANGERULE:
		return "DIOCCHANGERULE";
/* #define DIOCCLRSTATUS _IO ('D', 22) [DIOCCLRSTATUS] */
	case DIOCCLRSTATUS:
		return "DIOCCLRSTATUS";
/* #define DIOCGETRULES _IOWR('D', 6, struct pfioc_rule) [DIOCGETRULES] */
	case DIOCGETRULES:
		return "DIOCGETRULES";
/* #define DIOCRSETADDRS _IOWR('D', 69, struct pfioc_table) [DIOCRSETADDRS] */
	case DIOCRSETADDRS:
		return "DIOCRSETADDRS";
/* #define DIOCSETTIMEOUT _IOWR('D', 29, struct pfioc_tm) [DIOCSETTIMEOUT] */
	case DIOCSETTIMEOUT:
		return "DIOCSETTIMEOUT";
/* #define DIOCGETTIMEOUT _IOWR('D', 30, struct pfioc_tm) [DIOCGETTIMEOUT] */
	case DIOCGETTIMEOUT:
		return "DIOCGETTIMEOUT";
/* #define DIOCNATLOOK _IOWR('D', 23, struct pfioc_natlook) [DIOCNATLOOK] */
	case DIOCNATLOOK:
		return "DIOCNATLOOK";
/* #define DIOCOSFPFLUSH _IO('D', 78) [DIOCOSFPFLUSH] */
	case DIOCOSFPFLUSH:
		return "DIOCOSFPFLUSH";
/* #define DIOCRGETASTATS _IOWR('D', 71, struct pfioc_table) [DIOCRGETASTATS] */
	case DIOCRGETASTATS:
		return "DIOCRGETASTATS";
/* #define DIOCRINADEFINE _IOWR('D', 77, struct pfioc_table) [DIOCRINADEFINE] */
	case DIOCRINADEFINE:
		return "DIOCRINADEFINE";
/* #define DIOCGETADDR _IOWR('D', 54, struct pfioc_pooladdr) [DIOCGETADDR] */
	case DIOCGETADDR:
		return "DIOCGETADDR";
/* #define DIOCGETRULESETS _IOWR('D', 58, struct pfioc_ruleset) [DIOCGETRULESETS] */
	case DIOCGETRULESETS:
		return "DIOCGETRULESETS";
/* #define DIOCCHANGEADDR _IOWR('D', 55, struct pfioc_pooladdr) [DIOCCHANGEADDR] */
	case DIOCCHANGEADDR:
		return "DIOCCHANGEADDR";
/* #define DIOCGETADDRS _IOWR('D', 53, struct pfioc_pooladdr) [DIOCGETADDRS] */
	case DIOCGETADDRS:
		return "DIOCGETADDRS";
/* #define DIOCSTOP _IO ('D', 2) [DIOCSTOP] */
	case DIOCSTOP:
		return "DIOCSTOP";
/* #define DIOCADDSTATE _IOWR('D', 37, struct pfioc_state) [DIOCADDSTATE] */
	case DIOCADDSTATE:
		return "DIOCADDSTATE";
/* #define DIOCGETSTATE _IOWR('D', 19, struct pfioc_state) [DIOCGETSTATE] */
	case DIOCGETSTATE:
		return "DIOCGETSTATE";
/* #define DIOCCLRRULECTRS _IO ('D', 38) [DIOCCLRRULECTRS] */
	case DIOCCLRRULECTRS:
		return "DIOCCLRRULECTRS";
/* #define DIOCGETSRCNODES _IOWR('D', 84, struct pfioc_src_nodes) [DIOCGETSRCNODES] */
	case DIOCGETSRCNODES:
		return "DIOCGETSRCNODES";
/* #define DIOCADDADDR _IOWR('D', 52, struct pfioc_pooladdr) [DIOCADDADDR] */
	case DIOCADDADDR:
		return "DIOCADDADDR";
/* #define DIOCRCLRTSTATS _IOWR('D', 65, struct pfioc_table) [DIOCRCLRTSTATS] */
	case DIOCRCLRTSTATS:
		return "DIOCRCLRTSTATS";
/* #define DIOCGETSTATES _IOWR('D', 25, struct pfioc_states) [DIOCGETSTATES] */
	case DIOCGETSTATES:
		return "DIOCGETSTATES";
/* #define DIOCSTARTALTQ _IO ('D', 42) [DIOCSTARTALTQ] */
	case DIOCSTARTALTQ:
		return "DIOCSTARTALTQ";
/* #define DIOCRCLRADDRS _IOWR('D', 66, struct pfioc_table) [DIOCRCLRADDRS] */
	case DIOCRCLRADDRS:
		return "DIOCRCLRADDRS";
/* #define SLIOCGUNIT _IOR('t', 88, int) [SLIOCGUNIT] */
/* #define SIOCSWAPA _IOR('r', 69, u_int) [SIOCSWAPA] */
	case SIOCSWAPA:
		return "SIOCSWAPA";
/* #define SIOCSTGET _IOWR('r', 81, struct ipfobj) [SIOCSTGET] */
	case SIOCSTGET:
		return "SIOCSTGET";
/* #define SIOCFRZST _IOWR('r', 74, struct ipfobj) [SIOCFRZST] */
	case SIOCFRZST:
		return "SIOCFRZST";
/* #define SIOCSTPUT _IOWR('r', 80, struct ipfobj) [SIOCSTPUT] */
	case SIOCSTPUT:
		return "SIOCSTPUT";
/* #define SIOCIPFFB _IOR('r', 66, int) [SIOCIPFFB] */
	case SIOCIPFFB:
		return "SIOCIPFFB";
/* #define SIOCIPFFL _IOWR('r', 65, int) [SIOCIPFFL] */
	case SIOCIPFFL:
		return "SIOCIPFFL";
/* #define SIOCIPFGET _IOWR('r', 88, struct ipfobj) [SIOCIPFGET] */
	case SIOCIPFGET:
		return "SIOCIPFGET";
/* #define SIOCSTGSZ _IOWR('r', 82, struct ipfobj) [SIOCSTGSZ] */
	case SIOCSTGSZ:
		return "SIOCSTGSZ";
/* #define SIOCGETFF _IOR('r', 63, u_int) [SIOCGETFF] */
	case SIOCGETFF:
		return "SIOCGETFF";
/* #define SIOCGETFS _IOWR('r', 64, struct ipfobj) [SIOCGETFS] */
	case SIOCGETFS:
		return "SIOCGETFS";
/* #define SIOCFRSYN _IOW('r', 73, u_int) [SIOCFRSYN] */
	case SIOCFRSYN:
		return "SIOCFRSYN";
/* #define SIOCGETLG _IOWR('r', 85, int) [SIOCGETLG] */
	case SIOCGETLG:
		return "SIOCGETLG";
/* #define SIOCRMIFR _IOW('r', 68, struct ipfobj) [SIOCRMIFR] */
	case SIOCRMIFR:
		return "SIOCRMIFR";
/* #define SIOCINIFR _IOW('r', 71, struct ipfobj) [SIOCINIFR] */
	case SIOCINIFR:
		return "SIOCINIFR";
/* #define SIOCATHST _IOWR('r', 78, struct ipfobj) [SIOCATHST] */
	case SIOCATHST:
		return "SIOCATHST";
/* #define SIOCADIFR _IOW('r', 67, struct ipfobj) [SIOCADIFR] */
	case SIOCADIFR:
		return "SIOCADIFR";
/* #define SIOCZRLST _IOWR('r', 75, struct ipfobj) [SIOCZRLST] */
	case SIOCZRLST:
		return "SIOCZRLST";
/* #define SIOCSETFF _IOW('r', 62, u_int) [SIOCSETFF] */
	case SIOCSETFF:
		return "SIOCSETFF";
/* #define SIOCFUNCL _IOWR('r', 86, struct ipfunc_resolve) [SIOCFUNCL] */
	case SIOCFUNCL:
		return "SIOCFUNCL";
/* #define SIOCFRENB _IOW('r', 72, u_int) [SIOCFRENB] */
	case SIOCFRENB:
		return "SIOCFRENB";
/* #define SIOCIPFGETNEXT _IOWR('r', 87, struct ipfobj) [SIOCIPFGETNEXT] */
	case SIOCIPFGETNEXT:
		return "SIOCIPFGETNEXT";
/* #define SIOCSETLG _IOWR('r', 84, int) [SIOCSETLG] */
	case SIOCSETLG:
		return "SIOCSETLG";
/* #define SIOCAUTHR _IOWR('r', 77, struct ipfobj) [SIOCAUTHR] */
	case SIOCAUTHR:
		return "SIOCAUTHR";
/* #define SIOCAUTHW _IOWR('r', 76, struct ipfobj) [SIOCAUTHW] */
	case SIOCAUTHW:
		return "SIOCAUTHW";
/* #define SIOCIPFSET _IOWR('r', 89, struct ipfobj) [SIOCIPFSET] */
	case SIOCIPFSET:
		return "SIOCIPFSET";
/* #define SIOCSTLCK _IOWR('r', 79, u_int) [SIOCSTLCK] */
	case SIOCSTLCK:
		return "SIOCSTLCK";
/* #define SIOCRMAFR _IOW('r', 61, struct ipfobj) [SIOCRMAFR] */
	case SIOCRMAFR:
		return "SIOCRMAFR";
/* #define SIOCINAFR _IOW('r', 70, struct ipfobj) [SIOCINAFR] */
	case SIOCINAFR:
		return "SIOCINAFR";
/* #define SIOCGFRST _IOWR('r', 83, struct ipfobj) [SIOCGFRST] */
	case SIOCGFRST:
		return "SIOCGFRST";
/* #define SIOCADAFR _IOW('r', 60, struct ipfobj) [SIOCADAFR] */
	case SIOCADAFR:
		return "SIOCADAFR";
/* #define SIOCLOOKUPSTAT _IOWR('r', 64, struct iplookupop) [SIOCLOOKUPSTAT] */
	case SIOCLOOKUPSTAT:
		return "SIOCLOOKUPSTAT";
/* #define SIOCLOOKUPSTATW _IOW('r', 64, struct iplookupop) [SIOCLOOKUPSTATW] */
	case SIOCLOOKUPSTATW:
		return "SIOCLOOKUPSTATW";
/* #define SIOCLOOKUPDELNODE _IOWR('r', 68, struct iplookupop) [SIOCLOOKUPDELNODE] */
	case SIOCLOOKUPDELNODE:
		return "SIOCLOOKUPDELNODE";
/* #define SIOCLOOKUPADDNODE _IOWR('r', 67, struct iplookupop) [SIOCLOOKUPADDNODE] */
	case SIOCLOOKUPADDNODE:
		return "SIOCLOOKUPADDNODE";
/* #define SIOCLOOKUPDELNODEW _IOW('r', 68, struct iplookupop) [SIOCLOOKUPDELNODEW] */
	case SIOCLOOKUPDELNODEW:
		return "SIOCLOOKUPDELNODEW";
/* #define SIOCLOOKUPFLUSH _IOWR('r', 65, struct iplookupflush) [SIOCLOOKUPFLUSH] */
	case SIOCLOOKUPFLUSH:
		return "SIOCLOOKUPFLUSH";
/* #define SIOCLOOKUPDELTABLE _IOWR('r', 61, struct iplookupop) [SIOCLOOKUPDELTABLE] */
	case SIOCLOOKUPDELTABLE:
		return "SIOCLOOKUPDELTABLE";
/* #define SIOCLOOKUPADDNODEW _IOW('r', 67, struct iplookupop) [SIOCLOOKUPADDNODEW] */
	case SIOCLOOKUPADDNODEW:
		return "SIOCLOOKUPADDNODEW";
/* #define SIOCLOOKUPADDTABLE _IOWR('r', 60, struct iplookupop) [SIOCLOOKUPADDTABLE] */
	case SIOCLOOKUPADDTABLE:
		return "SIOCLOOKUPADDTABLE";
/* #define SIOCRMNAT _IOW('r', 61, struct ipfobj) [SIOCRMNAT] */
/* #define SIOCADNAT _IOW('r', 60, struct ipfobj) [SIOCADNAT] */
/* #define SIOCGNATL _IOWR('r', 63, struct ipfobj) [SIOCGNATL] */
	case SIOCGNATL:
		return "SIOCGNATL";
/* #define SIOCGNATS _IOWR('r', 62, struct ipfobj) [SIOCGNATS] */
	case SIOCGNATS:
		return "SIOCGNATS";
/* #define SIOCPROXY _IOWR('r', 64, struct ap_control) [SIOCPROXY] */
	case SIOCPROXY:
		return "SIOCPROXY";
/* #define SIOCDELST _IOW('r', 61, struct ipfobj) [SIOCDELST] */
/* #define SIOCGSTYPE _IOR('a', 40, struct systype_req) [SIOCGSTYPE] */
	case SIOCGSTYPE:
		return "SIOCGSTYPE";
/* #define SIOCSSTYPE _IOW('a', 39, struct systype_req) [SIOCSSTYPE] */
	case SIOCSSTYPE:
		return "SIOCSSTYPE";
/* #define TIOCREMOTE _IOW('t', 105, int) [TIOCREMOTE] */
/* #define DIOCSRETRIES _IOW('d', 108, int) [DIOCSRETRIES] */
/* #define FIONBIO _IOW('f', 126, int) [FIONBIO] */
/* #define DIOCLWEDGES _IOWR('d', 124, struct dkwedge_list) [DIOCLWEDGES] */
/* #define TIOCGLINED _IOR('t', 66, linedn_t) [TIOCGLINED] */
/* #define FIONWRITE _IOR('f', 121, int) [FIONWRITE] */
/* #define TIOCMSET _IOW('t', 109, int) [TIOCMSET] */
/* #define TIOCRCVFRAME _IOW('t', 69, struct mbuf *) [TIOCRCVFRAME] */
/* #define TIOCDRAIN _IO('t', 94) [TIOCDRAIN] */
/* #define TIOCEXCL _IO('t', 13) [TIOCEXCL] */
/* #define TIOCPKT_IOCTL 0x40 [TIOCPKT_IOCTL] */
/* #define FIOGETOWN _IOR('f', 123, int) [FIOGETOWN] */
/* #define SIOCGPGRP _IOR('s', 9, int) [SIOCGPGRP] */
/* #define DIOCGCACHE _IOR('d', 116, int) [DIOCGCACHE] */
/* #define TIOCUCNTL _IOW('t', 102, int) [TIOCUCNTL] */
/* #define SIOCGIFADDR _IOWR('i', 33, struct ifreq) [SIOCGIFADDR] */
/* #define SIOCGIFMETRIC _IOWR('i', 23, struct ifreq) [SIOCGIFMETRIC] */
/* #define OSIOCGIFADDR _IOWR('i', 13, struct ifreq) [OSIOCGIFADDR] */
/* #define SIOCGIFGENERIC _IOWR('i', 58, struct ifreq) [SIOCGIFGENERIC] */
/* #define SIOCGIFPSRCADDR _IOWR('i', 71, struct ifreq) [SIOCGIFPSRCADDR] */
/* #define SIOCSIFDSTADDR _IOW('i', 14, struct ifreq) [SIOCSIFDSTADDR] */
/* #define SIOCGIFPDSTADDR _IOWR('i', 72, struct ifreq) [SIOCGIFPDSTADDR] */
/* #define DIOCGDEFLABEL _IOR('d', 114, struct disklabel) [DIOCGDEFLABEL] */
/* #define FIOASYNC _IOW('f', 125, int) [FIOASYNC] */
/* #define SIOCSHIWAT _IOW('s', 0, int) [SIOCSHIWAT] */
/* #define SIOCSIFMEDIA _IOWR('i', 53, struct ifreq) [SIOCSIFMEDIA] */
/* #define TIOCSBRK _IO('t', 123) [TIOCSBRK] */
/* #define DIOCSSTEP _IOW('d', 107, int) [DIOCSSTEP] */
/* #define SIOCDIFPHYADDR _IOW('i', 73, struct ifreq) [SIOCDIFPHYADDR] */
/* #define SIOCGLIFADDR _IOWR('i', 29, struct if_laddrreq) [SIOCGLIFADDR] */
/* #define SIOCGDRVSPEC _IOWR('i', 123, struct ifdrv) [SIOCGDRVSPEC] */
/* #define SIOCDIFADDR _IOW('i', 25, struct ifreq) [SIOCDIFADDR] */
/* #define ATAIOCCOMMAND _IOWR('Q', 8, atareq_t) [ATAIOCCOMMAND] */
	case ATAIOCCOMMAND:
		return "ATAIOCCOMMAND";
/* #define TIOCGRANTPT _IO('t', 71) [TIOCGRANTPT] */
/* #define SIOCGIFMTU _IOWR('i', 126, struct ifreq) [SIOCGIFMTU] */
/* #define SIOCGIFDLT _IOWR('i', 119, struct ifreq) [SIOCGIFDLT] */
/* #define TIOCSIG _IO('t', 95) [TIOCSIG] */
/* #define PTIOCNETBSD _IOW('Z', 0, struct ioctl_pt) [PTIOCNETBSD] */
/* #define _IOWR(g,n,t) _IOC(IOC_INOUT, (g), (n), sizeof(t)) [_IOWR(g,n,t)] */
/* #define TIOCGETA _IOR('t', 19, struct termios) [TIOCGETA] */
/* #define TIOCGETD _IOR('t', 26, int) [TIOCGETD] */
/* #define PTIOCULTRIX _IOW('Z', 6, struct ioctl_pt) [PTIOCULTRIX] */
/* #define SIOCSIFNETMASK _IOW('i', 22, struct ifreq) [SIOCSIFNETMASK] */
/* #define SIOCAIFADDR _IOW('i', 26, struct ifaliasreq) [SIOCAIFADDR] */
/* #define DIOCCACHESYNC _IOW('d', 118, int) [DIOCCACHESYNC] */
/* #define TIOCSETD _IOW('t', 27, int) [TIOCSETD] */
/* #define TIOCSDTR _IO('t', 121) [TIOCSDTR] */
/* #define SIOCDLIFADDR _IOW('i', 30, struct if_laddrreq) [SIOCDLIFADDR] */
/* #define OSIOCGIFCONF _IOWR('i', 20, struct ifconf) [OSIOCGIFCONF] */
/* #define DIOCGWEDGEINFO _IOR('d', 122, struct dkwedge_info) [DIOCGWEDGEINFO] */
/* #define TIOCXMTFRAME _IOW('t', 68, struct mbuf *) [TIOCXMTFRAME] */
/* #define TIOCSFLAGS _IOW('t', 92, int) [TIOCSFLAGS] */
/* #define DIOCWLABEL _IOW('d', 109, int) [DIOCWLABEL] */
/* #define TIOCSTI _IOW('t', 114, char) [TIOCSTI] */
/* #define SIOCSIFBRDADDR _IOW('i', 19, struct ifreq) [SIOCSIFBRDADDR] */
/* #define SIOCGLIFPHYADDR _IOWR('i', 75, struct if_laddrreq) [SIOCGLIFPHYADDR] */
/* #define DIOCWFORMAT _IOWR('d', 106, struct format_op) [DIOCWFORMAT] */
/* #define SIOCSPGRP _IOW('s', 8, int) [SIOCSPGRP] */
/* #define TIOCPTSNAME _IOR('t', 72, struct ptmget) [TIOCPTSNAME] */
/* #define SIOCADDMULTI _IOW('i', 49, struct ifreq) [SIOCADDMULTI] */
/* #define SIOCZIFDATA _IOWR('i', 129, struct ifdatareq) [SIOCZIFDATA] */
/* #define SIOCGETSGCNT _IOWR('u', 52, struct sioc_sg_req) [SIOCGETSGCNT] */
/* #define SIOCALIFADDR _IOW('i', 28, struct if_laddrreq) [SIOCALIFADDR] */
/* #define TIOCSLINED _IOW('t', 67, linedn_t) [TIOCSLINED] */
/* #define SIOCGETVIFCNT _IOWR('u', 51, struct sioc_vif_req) [SIOCGETVIFCNT] */
/* #define PTIOCSUNOS _IOW('Z', 1, struct ioctl_pt) [PTIOCSUNOS] */
/* #define TIOCSTART _IO('t', 110) [TIOCSTART] */
/* #define DIOCAWEDGE _IOWR('d', 121, struct dkwedge_info) [DIOCAWEDGE] */
/* #define SIOCIFGCLONERS _IOWR('i', 120, struct if_clonereq) [SIOCIFGCLONERS] */
/* #define SIOCDELRT _IOW('r', 11, struct ortentry) [SIOCDELRT] */
/* #define FIOSETOWN _IOW('f', 124, int) [FIOSETOWN] */
/* #define DIOCSCACHE _IOW('d', 117, int) [DIOCSCACHE] */
/* #define DIOCRFORMAT _IOWR('d', 105, struct format_op) [DIOCRFORMAT] */
/* #define ATABUSIORESET _IO('A', 51) [ATABUSIORESET] */
	case ATABUSIORESET:
		return "ATABUSIORESET";
/* #define SIOCGIFDSTADDR _IOWR('i', 34, struct ifreq) [SIOCGIFDSTADDR] */
/* #define TIOCSTAT _IOW('t', 101, int) [TIOCSTAT] */
/* #define SIOCGHIWAT _IOR('s', 1, int) [SIOCGHIWAT] */
/* #define DIOCGPART _IOW('d', 104, struct partinfo) [DIOCGPART] */
/* #define ATABUSIOSCAN _IOW('A', 50, struct atabusioscan_args) [ATABUSIOSCAN] */
	case ATABUSIOSCAN:
		return "ATABUSIOSCAN";
/* #define TIOCMGET _IOR('t', 106, int) [TIOCMGET] */
/* #define TIOCNXCL _IO('t', 14) [TIOCNXCL] */
/* #define SIOCSIFFLAGS _IOW('i', 16, struct ifreq) [SIOCSIFFLAGS] */
/* #define SIOCGLOWAT _IOR('s', 3, int) [SIOCGLOWAT] */
/* #define SIOCSIFMTU _IOW('i', 127, struct ifreq) [SIOCSIFMTU] */
/* #define SIOCGIFMEDIA _IOWR('i', 54, struct ifmediareq) [SIOCGIFMEDIA] */
/* #define ATABUSIODETACH _IOW('A', 52, struct atabusiodetach_args) [ATABUSIODETACH] */
	case ATABUSIODETACH:
		return "ATABUSIODETACH";
/* #define TIOCFLUSH _IOW('t', 16, int) [TIOCFLUSH] */
/* #define DIOCEJECT _IOW('d', 112, int) [DIOCEJECT] */
/* #define TIOCGWINSZ _IOR('t', 104, struct winsize) [TIOCGWINSZ] */
/* #define SIOCGIFCAP _IOWR('i', 118, struct ifcapreq) [SIOCGIFCAP] */
/* #define TIOCGPGRP _IOR('t', 119, int) [TIOCGPGRP] */
/* #define SIOCGIFNETMASK _IOWR('i', 37, struct ifreq) [SIOCGIFNETMASK] */
/* #define TIOCPKT _IOW('t', 112, int) [TIOCPKT] */
/* #define OSIOCGIFDSTADDR _IOWR('i', 15, struct ifreq) [OSIOCGIFDSTADDR] */
/* #define DIOCBSFLUSH _IO('d', 120) [DIOCBSFLUSH] */
/* #define FIONCLEX _IO('f', 2) [FIONCLEX] */
/* #define SIOCSLIFPHYADDR _IOW('i', 74, struct if_laddrreq) [SIOCSLIFPHYADDR] */
/* #define FIOCLEX _IO('f', 1) [FIOCLEX] */
/* #define SIOCGIFBRDADDR _IOWR('i', 35, struct ifreq) [SIOCGIFBRDADDR] */
/* #define TIOCPTMGET _IOR('t', 70, struct ptmget) [TIOCPTMGET] */
/* #define TIOCEXT _IOW('t', 96, int) [TIOCEXT] */
/* #define SIOCIFDESTROY _IOW('i', 121, struct ifreq) [SIOCIFDESTROY] */
/* #define DIOCGDINFO _IOR('d', 101, struct disklabel) [DIOCGDINFO] */
/* #define SIOCGIFCONF _IOWR('i', 36, struct ifconf) [SIOCGIFCONF] */
/* #define OFIOGETBMAP _IOWR('f', 122, uint32_t) [OFIOGETBMAP] */
/* #define TIOCSCTTY _IO('t', 97) [TIOCSCTTY] */
/* #define FIONSPACE _IOR('f', 120, int) [FIONSPACE] */
/* #define DIOCCLRLABEL _IO('d', 115) [DIOCCLRLABEL] */
/* #define FIOGETBMAP _IOWR('f', 122, daddr_t) [FIOGETBMAP] */
/* #define TIOCOUTQ _IOR('t', 115, int) [TIOCOUTQ] */
/* #define TIOCCBRK _IO('t', 122) [TIOCCBRK] */
/* #define OSIOCGIFNETMASK _IOWR('i', 21, struct ifreq) [OSIOCGIFNETMASK] */
/* #define SIOCADDRT _IOW('r', 10, struct ortentry) [SIOCADDRT] */
/* #define SIOCGIFDATA _IOWR('i', 128, struct ifdatareq) [SIOCGIFDATA] */
/* #define DIOCWDINFO _IOW('d', 103, struct disklabel) [DIOCWDINFO] */
/* #define SIOCDELMULTI _IOW('i', 50, struct ifreq) [SIOCDELMULTI] */
/* #define OSIOCGIFBRDADDR _IOWR('i', 18, struct ifreq) [OSIOCGIFBRDADDR] */
/* #define TIOCMBIC _IOW('t', 107, int) [TIOCMBIC] */
/* #define TIOCMBIS _IOW('t', 108, int) [TIOCMBIS] */
/* #define SIOCSIFGENERIC _IOW('i', 57, struct ifreq) [SIOCSIFGENERIC] */
/* #define SIOCSDRVSPEC _IOW('i', 123, struct ifdrv) [SIOCSDRVSPEC] */
/* #define TIOCSETA _IOW('t', 20, struct termios) [TIOCSETA] */
/* #define TIOCCONS _IOW('t', 98, int) [TIOCCONS] */
/* #define TIOCSPGRP _IOW('t', 118, int) [TIOCSPGRP] */
/* #define TIOCGSID _IOR('t', 99, int) [TIOCGSID] */
/* #define FIONREAD _IOR('f', 127, int) [FIONREAD] */
/* #define _IO(g,n) _IOC(IOC_VOID, (g), (n), 0) [_IO(g,n)] */
/* #define TIOCCDTR _IO('t', 120) [TIOCCDTR] */
/* #define SIOCATMARK _IOR('s', 7, int) [SIOCATMARK] */
/* #define SIOCSLOWAT _IOW('s', 2, int) [SIOCSLOWAT] */
/* #define PTIOCLINUX _IOW('Z', 3, struct ioctl_pt) [PTIOCLINUX] */
/* #define DIOCSBAD _IOW('d', 110, struct dkbad) [DIOCSBAD] */
/* #define SIOCSIFADDR _IOW('i', 12, struct ifreq) [SIOCSIFADDR] */
/* #define TIOCSWINSZ _IOW('t', 103, struct winsize) [TIOCSWINSZ] */
/* #define SIOCSIFCAP _IOW('i', 117, struct ifcapreq) [SIOCSIFCAP] */
/* #define SIOCGIFFLAGS _IOWR('i', 17, struct ifreq) [SIOCGIFFLAGS] */
/* #define TIOCSETAF _IOW('t', 22, struct termios) [TIOCSETAF] */
/* #define TIOCSETAW _IOW('t', 21, struct termios) [TIOCSETAW] */
/* #define _IOC(inout,group,num,len) (inout | ((len & IOCPARM_MASK) << 16) | ((group) << 8) | (num)) [_IOC(inout,group,num,len)] */
/* #define SIOCSIFMETRIC _IOW('i', 24, struct ifreq) [SIOCSIFMETRIC] */
/* #define _IOR(g,n,t) _IOC(IOC_OUT, (g), (n), sizeof(t)) [_IOR(g,n,t)] */
/* #define _IOW(g,n,t) _IOC(IOC_IN, (g), (n), sizeof(t)) [_IOW(g,n,t)] */
/* #define ODIOCEJECT _IO('d', 112) [ODIOCEJECT] */
/* #define SIOCGIFALIAS _IOWR('i', 27, struct ifaliasreq) [SIOCGIFALIAS] */
/* #define TIOCGFLAGS _IOR('t', 93, int) [TIOCGFLAGS] */
/* #define DIOCKLABEL _IOW('d', 119, int) [DIOCKLABEL] */
/* #define DIOCDWEDGE _IOW('d', 123, struct dkwedge_info) [DIOCDWEDGE] */
/* #define TIOCNOTTY _IO('t', 113) [TIOCNOTTY] */
/* #define TIOCSTOP _IO('t', 111) [TIOCSTOP] */
/* #define PTIOCFREEBSD _IOW('Z', 4, struct ioctl_pt) [PTIOCFREEBSD] */
/* #define DIOCSDINFO _IOW('d', 102, struct disklabel) [DIOCSDINFO] */
/* #define TIOCDCDTIMESTAMP _IOR('t', 88, struct timeval) [TIOCDCDTIMESTAMP] */
/* #define SIOCIFCREATE _IOW('i', 122, struct ifreq) [SIOCIFCREATE] */
/* #define DIOCLOCK _IOW('d', 113, int) [DIOCLOCK] */
/* #define SIOCSIFPHYADDR _IOW('i', 70, struct ifaliasreq) [SIOCSIFPHYADDR] */
/* #define DIOCBSLIST _IOWR('d', 119, struct disk_badsecinfo) [DIOCBSLIST] */
/* #define CDIOCPLAYMSF _IOW('c', 25, struct ioc_play_msf) [CDIOCPLAYMSF] */
	case CDIOCPLAYMSF:
		return "CDIOCPLAYMSF";
/* #define CDIOCPREVENT _IO('c', 26) [CDIOCPREVENT] */
	case CDIOCPREVENT:
		return "CDIOCPREVENT";
/* #define CDIOCSETPATCH _IOW('c', 9, struct ioc_patch) [CDIOCSETPATCH] */
	case CDIOCSETPATCH:
		return "CDIOCSETPATCH";
/* #define CDIOCSETLEFT _IO('c', 15) [CDIOCSETLEFT] */
	case CDIOCSETLEFT:
		return "CDIOCSETLEFT";
/* #define CDIOREADMSADDR _IOWR('c', 6, int) [CDIOREADMSADDR] */
	case CDIOREADMSADDR:
		return "CDIOREADMSADDR";
/* #define CDIOCPAUSE _IO('c', 19) [CDIOCPAUSE] */
	case CDIOCPAUSE:
		return "CDIOCPAUSE";
/* #define CDIOCALLOW _IO('c', 25) [CDIOCALLOW] */
	case CDIOCALLOW:
		return "CDIOCALLOW";
/* #define CDIOCCLRDEBUG _IO('c', 18) [CDIOCCLRDEBUG] */
	case CDIOCCLRDEBUG:
		return "CDIOCCLRDEBUG";
/* #define CDIOCPLAYTRACKS _IOW('c', 1, struct ioc_play_track) [CDIOCPLAYTRACKS] */
	case CDIOCPLAYTRACKS:
		return "CDIOCPLAYTRACKS";
/* #define CDIOCEJECT _IO('c', 24) [CDIOCEJECT] */
	case CDIOCEJECT:
		return "CDIOCEJECT";
/* #define CDIOCSETMONO _IO('c', 12) [CDIOCSETMONO] */
	case CDIOCSETMONO:
		return "CDIOCSETMONO";
/* #define CDIOCSTART _IO('c', 22) [CDIOCSTART] */
	case CDIOCSTART:
		return "CDIOCSTART";
/* #define CDIOCRESUME _IO('c', 20) [CDIOCRESUME] */
	case CDIOCRESUME:
		return "CDIOCRESUME";
/* #define CDIOCSTOP _IO('c', 23) [CDIOCSTOP] */
	case CDIOCSTOP:
		return "CDIOCSTOP";
/* #define CDIOCSETRIGHT _IO('c', 16) [CDIOCSETRIGHT] */
	case CDIOCSETRIGHT:
		return "CDIOCSETRIGHT";
/* #define CDIOREADTOCENTRIES _IOWR('c', 5, struct ioc_read_toc_entry) [CDIOREADTOCENTRIES] */
	case CDIOREADTOCENTRIES:
		return "CDIOREADTOCENTRIES";
/* #define CDIOCLOADUNLOAD _IOW('c', 26, struct ioc_load_unload) [CDIOCLOADUNLOAD] */
	case CDIOCLOADUNLOAD:
		return "CDIOCLOADUNLOAD";
/* #define CDIOCGETVOL _IOR('c', 10, struct ioc_vol) [CDIOCGETVOL] */
	case CDIOCGETVOL:
		return "CDIOCGETVOL";
/* #define CDIOCRESET _IO('c', 21) [CDIOCRESET] */
	case CDIOCRESET:
		return "CDIOCRESET";
/* #define CDIOCCLOSE _IO('c', 27) [CDIOCCLOSE] */
	case CDIOCCLOSE:
		return "CDIOCCLOSE";
/* #define CDIOCSETMUTE _IO('c', 14) [CDIOCSETMUTE] */
	case CDIOCSETMUTE:
		return "CDIOCSETMUTE";
/* #define CDIOCPLAYBLOCKS _IOW('c', 2, struct ioc_play_blocks) [CDIOCPLAYBLOCKS] */
	case CDIOCPLAYBLOCKS:
		return "CDIOCPLAYBLOCKS";
/* #define CDIOREADTOCHEADER _IOR('c', 4, struct ioc_toc_header) [CDIOREADTOCHEADER] */
	case CDIOREADTOCHEADER:
		return "CDIOREADTOCHEADER";
/* #define CDIOCSETVOL _IOW('c', 11, struct ioc_vol) [CDIOCSETVOL] */
	case CDIOCSETVOL:
		return "CDIOCSETVOL";
/* #define CDIOCSETSTEREO _IO('c', 13) [CDIOCSETSTEREO] */
	case CDIOCSETSTEREO:
		return "CDIOCSETSTEREO";
/* #define CDIOCREADSUBCHANNEL _IOWR('c', 3, struct ioc_read_subchannel ) [CDIOCREADSUBCHANNEL] */
	case CDIOCREADSUBCHANNEL:
		return "CDIOCREADSUBCHANNEL";
/* #define CDIOCSETDEBUG _IO('c', 17) [CDIOCSETDEBUG] */
	case CDIOCSETDEBUG:
		return "CDIOCSETDEBUG";
/* #define CHIOGPICKER _IOR('c', 0x04, int) [CHIOGPICKER] */
/* #define CHIOMOVE _IOW('c', 0x01, struct changer_move_request) [CHIOMOVE] */
	case CHIOMOVE:
		return "CHIOMOVE";
/* #define CHIOEXCHANGE _IOW('c', 0x02, struct changer_exchange_request) [CHIOEXCHANGE] */
	case CHIOEXCHANGE:
		return "CHIOEXCHANGE";
/* #define CHIOPOSITION _IOW('c', 0x03, struct changer_position_request) [CHIOPOSITION] */
	case CHIOPOSITION:
		return "CHIOPOSITION";
/* #define CHIOSVOLTAG _IOW('c', 0x0a, struct changer_set_voltag_request) [CHIOSVOLTAG] */
	case CHIOSVOLTAG:
		return "CHIOSVOLTAG";
/* #define CHIOIELEM _IO('c', 0x07) [CHIOIELEM] */
	case CHIOIELEM:
		return "CHIOIELEM";
/* #define CHIOGPARAMS _IOR('c', 0x06, struct changer_params) [CHIOGPARAMS] */
	case CHIOGPARAMS:
		return "CHIOGPARAMS";
/* #define OCHIOGSTATUS _IOW('c', 0x08, struct ochanger_element_status_request) [OCHIOGSTATUS] */
	case OCHIOGSTATUS:
		return "OCHIOGSTATUS";
/* #define CHIOGSTATUS _IOW('c', 0x09, struct changer_element_status_request) [CHIOGSTATUS] */
	case CHIOGSTATUS:
		return "CHIOGSTATUS";
/* #define CHIOSPICKER _IOW('c', 0x05, int) [CHIOSPICKER] */
	case CHIOSPICKER:
		return "CHIOSPICKER";
/* #define DIOCSRETRIES _IOW('d', 108, int) [DIOCSRETRIES] */
/* #define TIOCSTAT _IOW('t', 101, int) [TIOCSTAT] */
/* #define FIONBIO _IOW('f', 126, int) [FIONBIO] */
/* #define TIOCMSET _IOW('t', 109, int) [TIOCMSET] */
/* #define DIOCLWEDGES _IOWR('d', 124, struct dkwedge_list) [DIOCLWEDGES] */
/* #define TIOCGLINED _IOR('t', 66, linedn_t) [TIOCGLINED] */
/* #define SIOCSIFMETRIC _IOW('i', 24, struct ifreq) [SIOCSIFMETRIC] */
/* #define TIOCGPGRP _IOR('t', 119, int) [TIOCGPGRP] */
/* #define FIONWRITE _IOR('f', 121, int) [FIONWRITE] */
/* #define TIOCRCVFRAME _IOW('t', 69, struct mbuf *) [TIOCRCVFRAME] */
/* #define TIOCDRAIN _IO('t', 94) [TIOCDRAIN] */
/* #define TIOCEXCL _IO('t', 13) [TIOCEXCL] */
/* #define TIOCPKT_IOCTL 0x40 [TIOCPKT_IOCTL] */
/* #define FIOGETOWN _IOR('f', 123, int) [FIOGETOWN] */
/* #define SIOCGPGRP _IOR('s', 9, int) [SIOCGPGRP] */
/* #define DIOCGCACHE _IOR('d', 116, int) [DIOCGCACHE] */
/* #define SIOCGIFADDR _IOWR('i', 33, struct ifreq) [SIOCGIFADDR] */
/* #define SIOCGIFMETRIC _IOWR('i', 23, struct ifreq) [SIOCGIFMETRIC] */
/* #define OSIOCGIFADDR _IOWR('i', 13, struct ifreq) [OSIOCGIFADDR] */
/* #define FIOSETOWN _IOW('f', 124, int) [FIOSETOWN] */
/* #define SIOCGIFGENERIC _IOWR('i', 58, struct ifreq) [SIOCGIFGENERIC] */
/* #define SIOCGIFPSRCADDR _IOWR('i', 71, struct ifreq) [SIOCGIFPSRCADDR] */
/* #define SIOCSIFDSTADDR _IOW('i', 14, struct ifreq) [SIOCSIFDSTADDR] */
/* #define SIOCGIFPDSTADDR _IOWR('i', 72, struct ifreq) [SIOCGIFPDSTADDR] */
/* #define DIOCGDEFLABEL _IOR('d', 114, struct disklabel) [DIOCGDEFLABEL] */
/* #define BLKDEV_IOSIZE 2048 [BLKDEV_IOSIZE] */
/* #define FIOASYNC _IOW('f', 125, int) [FIOASYNC] */
/* #define PTIOCULTRIX _IOW('Z', 6, struct ioctl_pt) [PTIOCULTRIX] */
/* #define TIOCSETA _IOW('t', 20, struct termios) [TIOCSETA] */
/* #define DIOCSSTEP _IOW('d', 107, int) [DIOCSSTEP] */
/* #define SIOCDIFPHYADDR _IOW('i', 73, struct ifreq) [SIOCDIFPHYADDR] */
/* #define SIOCGLIFADDR _IOWR('i', 29, struct if_laddrreq) [SIOCGLIFADDR] */
/* #define SIOCGDRVSPEC _IOWR('i', 123, struct ifdrv) [SIOCGDRVSPEC] */
/* #define FIONREAD _IOR('f', 127, int) [FIONREAD] */
/* #define SIOCDIFADDR _IOW('i', 25, struct ifreq) [SIOCDIFADDR] */
/* #define SIOCGIFDLT _IOWR('i', 119, struct ifreq) [SIOCGIFDLT] */
/* #define TIOCSIG _IO('t', 95) [TIOCSIG] */
/* #define DIOCGPART _IOW('d', 104, struct partinfo) [DIOCGPART] */
/* #define _IOWR(g,n,t) _IOC(IOC_INOUT, (g), (n), sizeof(t)) [_IOWR(g,n,t)] */
/* #define TIOCGETA _IOR('t', 19, struct termios) [TIOCGETA] */
/* #define TIOCGETD _IOR('t', 26, int) [TIOCGETD] */
/* #define SIOCSIFNETMASK _IOW('i', 22, struct ifreq) [SIOCSIFNETMASK] */
/* #define SIOCAIFADDR _IOW('i', 26, struct ifaliasreq) [SIOCAIFADDR] */
/* #define DIOCCACHESYNC _IOW('d', 118, int) [DIOCCACHESYNC] */
/* #define TIOCSETD _IOW('t', 27, int) [TIOCSETD] */
/* #define TIOCSDTR _IO('t', 121) [TIOCSDTR] */
/* #define SIOCDLIFADDR _IOW('i', 30, struct if_laddrreq) [SIOCDLIFADDR] */
/* #define DIOCDWEDGE _IOW('d', 123, struct dkwedge_info) [DIOCDWEDGE] */
/* #define DIOCGWEDGEINFO _IOR('d', 122, struct dkwedge_info) [DIOCGWEDGEINFO] */
/* #define TIOCXMTFRAME _IOW('t', 68, struct mbuf *) [TIOCXMTFRAME] */
/* #define TIOCSFLAGS _IOW('t', 92, int) [TIOCSFLAGS] */
/* #define DIOCWLABEL _IOW('d', 109, int) [DIOCWLABEL] */
/* #define TIOCSTI _IOW('t', 114, char) [TIOCSTI] */
/* #define SIOCSHIWAT _IOW('s', 0, int) [SIOCSHIWAT] */
/* #define SIOCSIFBRDADDR _IOW('i', 19, struct ifreq) [SIOCSIFBRDADDR] */
/* #define SIOCGLIFPHYADDR _IOWR('i', 75, struct if_laddrreq) [SIOCGLIFPHYADDR] */
/* #define DIOCWFORMAT _IOWR('d', 106, struct format_op) [DIOCWFORMAT] */
/* #define SIOCSPGRP _IOW('s', 8, int) [SIOCSPGRP] */
/* #define TIOCGRANTPT _IO('t', 71) [TIOCGRANTPT] */
/* #define TIOCPTSNAME _IOR('t', 72, struct ptmget) [TIOCPTSNAME] */
/* #define SIOCSIFMEDIA _IOWR('i', 53, struct ifreq) [SIOCSIFMEDIA] */
/* #define TIOCCONS _IOW('t', 98, int) [TIOCCONS] */
/* #define SIOCADDMULTI _IOW('i', 49, struct ifreq) [SIOCADDMULTI] */
/* #define SIOCZIFDATA _IOWR('i', 129, struct ifdatareq) [SIOCZIFDATA] */
/* #define SIOCGETSGCNT _IOWR('u', 52, struct sioc_sg_req) [SIOCGETSGCNT] */
/* #define TIOCUCNTL _IOW('t', 102, int) [TIOCUCNTL] */
/* #define SIOCALIFADDR _IOW('i', 28, struct if_laddrreq) [SIOCALIFADDR] */
/* #define TIOCSLINED _IOW('t', 67, linedn_t) [TIOCSLINED] */
/* #define SIOCGETVIFCNT _IOWR('u', 51, struct sioc_vif_req) [SIOCGETVIFCNT] */
/* #define PTIOCSUNOS _IOW('Z', 1, struct ioctl_pt) [PTIOCSUNOS] */
/* #define DIOCAWEDGE _IOWR('d', 121, struct dkwedge_info) [DIOCAWEDGE] */
/* #define SIOCIFGCLONERS _IOWR('i', 120, struct if_clonereq) [SIOCIFGCLONERS] */
/* #define SIOCDELRT _IOW('r', 11, struct ortentry) [SIOCDELRT] */
/* #define DIOCSCACHE _IOW('d', 117, int) [DIOCSCACHE] */
/* #define DIOCRFORMAT _IOWR('d', 105, struct format_op) [DIOCRFORMAT] */
/* #define SIOCGIFMTU _IOWR('i', 126, struct ifreq) [SIOCGIFMTU] */
/* #define SIOCGIFDSTADDR _IOWR('i', 34, struct ifreq) [SIOCGIFDSTADDR] */
/* #define TIOCSPGRP _IOW('t', 118, int) [TIOCSPGRP] */
/* #define SIOCGHIWAT _IOR('s', 1, int) [SIOCGHIWAT] */
/* #define SIOCSIFADDR _IOW('i', 12, struct ifreq) [SIOCSIFADDR] */
/* #define TIOCNXCL _IO('t', 14) [TIOCNXCL] */
/* #define SIOCSIFFLAGS _IOW('i', 16, struct ifreq) [SIOCSIFFLAGS] */
/* #define TIOCSETAF _IOW('t', 22, struct termios) [TIOCSETAF] */
/* #define SIOCGLOWAT _IOR('s', 3, int) [SIOCGLOWAT] */
/* #define SIOCSIFMTU _IOW('i', 127, struct ifreq) [SIOCSIFMTU] */
/* #define SIOCGIFMEDIA _IOWR('i', 54, struct ifmediareq) [SIOCGIFMEDIA] */
/* #define TIOCFLUSH _IOW('t', 16, int) [TIOCFLUSH] */
/* #define DIOCEJECT _IOW('d', 112, int) [DIOCEJECT] */
/* #define DIOCLOCK _IOW('d', 113, int) [DIOCLOCK] */
/* #define TIOCGWINSZ _IOR('t', 104, struct winsize) [TIOCGWINSZ] */
/* #define SIOCGIFNETMASK _IOWR('i', 37, struct ifreq) [SIOCGIFNETMASK] */
/* #define TIOCREMOTE _IOW('t', 105, int) [TIOCREMOTE] */
/* #define FIOGETBMAP _IOWR('f', 122, daddr_t) [FIOGETBMAP] */
/* #define DIOCSDINFO _IOW('d', 102, struct disklabel) [DIOCSDINFO] */
/* #define TIOCPKT _IOW('t', 112, int) [TIOCPKT] */
/* #define OSIOCGIFDSTADDR _IOWR('i', 15, struct ifreq) [OSIOCGIFDSTADDR] */
/* #define DIOCBSFLUSH _IO('d', 120) [DIOCBSFLUSH] */
/* #define FIONCLEX _IO('f', 2) [FIONCLEX] */
/* #define SIOCSLIFPHYADDR _IOW('i', 74, struct if_laddrreq) [SIOCSLIFPHYADDR] */
/* #define TIOCCDTR _IO('t', 120) [TIOCCDTR] */
/* #define FIOCLEX _IO('f', 1) [FIOCLEX] */
/* #define SIOCGIFBRDADDR _IOWR('i', 35, struct ifreq) [SIOCGIFBRDADDR] */
/* #define TIOCSBRK _IO('t', 123) [TIOCSBRK] */
/* #define TIOCPTMGET _IOR('t', 70, struct ptmget) [TIOCPTMGET] */
/* #define TIOCEXT _IOW('t', 96, int) [TIOCEXT] */
/* #define SIOCIFDESTROY _IOW('i', 121, struct ifreq) [SIOCIFDESTROY] */
/* #define DIOCGDINFO _IOR('d', 101, struct disklabel) [DIOCGDINFO] */
/* #define TIOCSCTTY _IO('t', 97) [TIOCSCTTY] */
/* #define TIOCMGET _IOR('t', 106, int) [TIOCMGET] */
/* #define FIONSPACE _IOR('f', 120, int) [FIONSPACE] */
/* #define DIOCCLRLABEL _IO('d', 115) [DIOCCLRLABEL] */
/* #define TIOCOUTQ _IOR('t', 115, int) [TIOCOUTQ] */
/* #define TIOCCBRK _IO('t', 122) [TIOCCBRK] */
/* #define SIOCADDRT _IOW('r', 10, struct ortentry) [SIOCADDRT] */
/* #define SIOCGIFDATA _IOWR('i', 128, struct ifdatareq) [SIOCGIFDATA] */
/* #define TIOCSETAW _IOW('t', 21, struct termios) [TIOCSETAW] */
/* #define DIOCWDINFO _IOW('d', 103, struct disklabel) [DIOCWDINFO] */
/* #define OFIOGETBMAP _IOWR('f', 122, uint32_t) [OFIOGETBMAP] */
/* #define SIOCDELMULTI _IOW('i', 50, struct ifreq) [SIOCDELMULTI] */
/* #define TIOCMBIC _IOW('t', 107, int) [TIOCMBIC] */
/* #define TIOCMBIS _IOW('t', 108, int) [TIOCMBIS] */
/* #define SIOCSIFGENERIC _IOW('i', 57, struct ifreq) [SIOCSIFGENERIC] */
/* #define SIOCSDRVSPEC _IOW('i', 123, struct ifdrv) [SIOCSDRVSPEC] */
/* #define TIOCGSID _IOR('t', 99, int) [TIOCGSID] */
/* #define _IO(g,n) _IOC(IOC_VOID, (g), (n), 0) [_IO(g,n)] */
/* #define SIOCSIFCAP _IOW('i', 117, struct ifcapreq) [SIOCSIFCAP] */
/* #define SIOCGIFCONF _IOWR('i', 36, struct ifconf) [SIOCGIFCONF] */
/* #define SIOCSIFPHYADDR _IOW('i', 70, struct ifaliasreq) [SIOCSIFPHYADDR] */
/* #define SIOCATMARK _IOR('s', 7, int) [SIOCATMARK] */
/* #define PTIOCNETBSD _IOW('Z', 0, struct ioctl_pt) [PTIOCNETBSD] */
/* #define SIOCSLOWAT _IOW('s', 2, int) [SIOCSLOWAT] */
/* #define PTIOCLINUX _IOW('Z', 3, struct ioctl_pt) [PTIOCLINUX] */
/* #define OSIOCGIFNETMASK _IOWR('i', 21, struct ifreq) [OSIOCGIFNETMASK] */
/* #define OSIOCGIFBRDADDR _IOWR('i', 18, struct ifreq) [OSIOCGIFBRDADDR] */
/* #define DIOCSBAD _IOW('d', 110, struct dkbad) [DIOCSBAD] */
/* #define TIOCSWINSZ _IOW('t', 103, struct winsize) [TIOCSWINSZ] */
/* #define SIOCGIFFLAGS _IOWR('i', 17, struct ifreq) [SIOCGIFFLAGS] */
/* #define OSIOCGIFCONF _IOWR('i', 20, struct ifconf) [OSIOCGIFCONF] */
/* #define _IOC(inout,group,num,len) (inout | ((len & IOCPARM_MASK) << 16) | ((group) << 8) | (num)) [_IOC(inout,group,num,len)] */
/* #define _IOR(g,n,t) _IOC(IOC_OUT, (g), (n), sizeof(t)) [_IOR(g,n,t)] */
/* #define ODIOCEJECT _IO('d', 112) [ODIOCEJECT] */
/* #define SIOCGIFALIAS _IOWR('i', 27, struct ifaliasreq) [SIOCGIFALIAS] */
/* #define TIOCGFLAGS _IOR('t', 93, int) [TIOCGFLAGS] */
/* #define DIOCKLABEL _IOW('d', 119, int) [DIOCKLABEL] */
/* #define TIOCNOTTY _IO('t', 113) [TIOCNOTTY] */
/* #define SIOCGIFCAP _IOWR('i', 118, struct ifcapreq) [SIOCGIFCAP] */
/* #define TIOCSTOP _IO('t', 111) [TIOCSTOP] */
/* #define PTIOCFREEBSD _IOW('Z', 4, struct ioctl_pt) [PTIOCFREEBSD] */
/* #define TIOCDCDTIMESTAMP _IOR('t', 88, struct timeval) [TIOCDCDTIMESTAMP] */
/* #define SIOCIFCREATE _IOW('i', 122, struct ifreq) [SIOCIFCREATE] */
/* #define TIOCSTART _IO('t', 110) [TIOCSTART] */
/* #define _IOW(g,n,t) _IOC(IOC_IN, (g), (n), sizeof(t)) [_IOW(g,n,t)] */
/* #define DIOCBSLIST _IOWR('d', 119, struct disk_badsecinfo) [DIOCBSLIST] */
/* #define DIOCSRETRIES _IOW('d', 108, int) [DIOCSRETRIES] */
/* #define DIOCLWEDGES _IOWR('d', 124, struct dkwedge_list) [DIOCLWEDGES] */
/* #define DIOCGCACHE _IOR('d', 116, int) [DIOCGCACHE] */
/* #define DIOCGDEFLABEL _IOR('d', 114, struct disklabel) [DIOCGDEFLABEL] */
/* #define DIOCSSTEP _IOW('d', 107, int) [DIOCSSTEP] */
/* #define _IOWR(g,n,t) _IOC(IOC_INOUT, (g), (n), sizeof(t)) [_IOWR(g,n,t)] */
/* #define DIOCCACHESYNC _IOW('d', 118, int) [DIOCCACHESYNC] */
/* #define DIOCGWEDGEINFO _IOR('d', 122, struct dkwedge_info) [DIOCGWEDGEINFO] */
/* #define DIOCWLABEL _IOW('d', 109, int) [DIOCWLABEL] */
/* #define DIOCWFORMAT _IOWR('d', 106, struct format_op) [DIOCWFORMAT] */
/* #define DIOCAWEDGE _IOWR('d', 121, struct dkwedge_info) [DIOCAWEDGE] */
/* #define DIOCSCACHE _IOW('d', 117, int) [DIOCSCACHE] */
/* #define DIOCRFORMAT _IOWR('d', 105, struct format_op) [DIOCRFORMAT] */
/* #define DIOCGPART _IOW('d', 104, struct partinfo) [DIOCGPART] */
/* #define DIOCEJECT _IOW('d', 112, int) [DIOCEJECT] */
/* #define DIOCGDINFO _IOR('d', 101, struct disklabel) [DIOCGDINFO] */
/* #define DIOCCLRLABEL _IO('d', 115) [DIOCCLRLABEL] */
/* #define DIOCWDINFO _IOW('d', 103, struct disklabel) [DIOCWDINFO] */
/* #define _IO(g,n) _IOC(IOC_VOID, (g), (n), 0) [_IO(g,n)] */
/* #define DIOCBSFLUSH _IO('d', 120) [DIOCBSFLUSH] */
/* #define DIOCSBAD _IOW('d', 110, struct dkbad) [DIOCSBAD] */
/* #define _IOC(inout,group,num,len) (inout | ((len & IOCPARM_MASK) << 16) | ((group) << 8) | (num)) [_IOC(inout,group,num,len)] */
/* #define _IOR(g,n,t) _IOC(IOC_OUT, (g), (n), sizeof(t)) [_IOR(g,n,t)] */
/* #define _IOW(g,n,t) _IOC(IOC_IN, (g), (n), sizeof(t)) [_IOW(g,n,t)] */
/* #define ODIOCEJECT _IO('d', 112) [ODIOCEJECT] */
/* #define DIOCKLABEL _IOW('d', 119, int) [DIOCKLABEL] */
/* #define DIOCDWEDGE _IOW('d', 123, struct dkwedge_info) [DIOCDWEDGE] */
/* #define DIOCSDINFO _IOW('d', 102, struct disklabel) [DIOCSDINFO] */
/* #define DIOCLOCK _IOW('d', 113, int) [DIOCLOCK] */
/* #define DIOCBSLIST _IOWR('d', 119, struct disk_badsecinfo) [DIOCBSLIST] */
/* #define DRVRESCANBUS _IOW('D', 124, struct devrescanargs) [DRVRESCANBUS] */
	case DRVRESCANBUS:
		return "DRVRESCANBUS";
/* #define DRVDETACHDEV _IOW('D', 123, struct devdetachargs) [DRVDETACHDEV] */
	case DRVDETACHDEV:
		return "DRVDETACHDEV";
/* #define _IOWR(g,n,t) _IOC(IOC_INOUT, (g), (n), sizeof(t)) [_IOWR(g,n,t)] */
/* #define _IO(g,n) _IOC(IOC_VOID, (g), (n), 0) [_IO(g,n)] */
/* #define _IOC(inout,group,num,len) (inout | ((len & IOCPARM_MASK) << 16) | ((group) << 8) | (num)) [_IOC(inout,group,num,len)] */
/* #define _IOR(g,n,t) _IOC(IOC_OUT, (g), (n), sizeof(t)) [_IOR(g,n,t)] */
/* #define _IOW(g,n,t) _IOC(IOC_IN, (g), (n), sizeof(t)) [_IOW(g,n,t)] */
/* #define FDIOCGETFORMAT _IOR('d', 117, struct fdformat_parms) [FDIOCGETFORMAT] */
	case FDIOCGETFORMAT:
		return "FDIOCGETFORMAT";
/* #define _IOWR(g,n,t) _IOC(IOC_INOUT, (g), (n), sizeof(t)) [_IOWR(g,n,t)] */
/* #define FDIOCSETOPTS _IOW('d', 115, int) [FDIOCSETOPTS] */
	case FDIOCSETOPTS:
		return "FDIOCSETOPTS";
/* #define FDIOCGETOPTS _IOR('d', 114, int) [FDIOCGETOPTS] */
	case FDIOCGETOPTS:
		return "FDIOCGETOPTS";
/* #define _IO(g,n) _IOC(IOC_VOID, (g), (n), 0) [_IO(g,n)] */
/* #define FDIOCSETFORMAT _IOW('d', 116, struct fdformat_parms) [FDIOCSETFORMAT] */
	case FDIOCSETFORMAT:
		return "FDIOCSETFORMAT";
/* #define _IOC(inout,group,num,len) (inout | ((len & IOCPARM_MASK) << 16) | ((group) << 8) | (num)) [_IOC(inout,group,num,len)] */
/* #define _IOR(g,n,t) _IOC(IOC_OUT, (g), (n), sizeof(t)) [_IOR(g,n,t)] */
/* #define _IOW(g,n,t) _IOC(IOC_IN, (g), (n), sizeof(t)) [_IOW(g,n,t)] */
/* #define FIONBIO _IOW('f', 126, int) [FIONBIO] */
/* #define FIONWRITE _IOR('f', 121, int) [FIONWRITE] */
/* #define FIOGETOWN _IOR('f', 123, int) [FIOGETOWN] */
/* #define FIOASYNC _IOW('f', 125, int) [FIOASYNC] */
/* #define _IOWR(g,n,t) _IOC(IOC_INOUT, (g), (n), sizeof(t)) [_IOWR(g,n,t)] */
/* #define FIOSETOWN _IOW('f', 124, int) [FIOSETOWN] */
/* #define FIONREAD _IOR('f', 127, int) [FIONREAD] */
/* #define FIONCLEX _IO('f', 2) [FIONCLEX] */
/* #define FIOCLEX _IO('f', 1) [FIOCLEX] */
/* #define OFIOGETBMAP _IOWR('f', 122, uint32_t) [OFIOGETBMAP] */
/* #define FIONSPACE _IOR('f', 120, int) [FIONSPACE] */
/* #define FIOGETBMAP _IOWR('f', 122, daddr_t) [FIOGETBMAP] */
/* #define _IO(g,n) _IOC(IOC_VOID, (g), (n), 0) [_IO(g,n)] */
/* #define _IOC(inout,group,num,len) (inout | ((len & IOCPARM_MASK) << 16) | ((group) << 8) | (num)) [_IOC(inout,group,num,len)] */
/* #define _IOR(g,n,t) _IOC(IOC_OUT, (g), (n), sizeof(t)) [_IOR(g,n,t)] */
/* #define _IOW(g,n,t) _IOC(IOC_IN, (g), (n), sizeof(t)) [_IOW(g,n,t)] */
/* #define _IOWR(g,n,t) _IOC(IOC_INOUT, (g), (n), sizeof(t)) [_IOWR(g,n,t)] */
/* #define _IO(g,n) _IOC(IOC_VOID, (g), (n), 0) [_IO(g,n)] */
/* #define _IOC(inout,group,num,len) (inout | ((len & IOCPARM_MASK) << 16) | ((group) << 8) | (num)) [_IOC(inout,group,num,len)] */
/* #define _IOR(g,n,t) _IOC(IOC_OUT, (g), (n), sizeof(t)) [_IOR(g,n,t)] */
/* #define _IOW(g,n,t) _IOC(IOC_IN, (g), (n), sizeof(t)) [_IOW(g,n,t)] */
/* #define DIOCSRETRIES _IOW('d', 108, int) [DIOCSRETRIES] */
/* #define FIONBIO _IOW('f', 126, int) [FIONBIO] */
/* #define DIOCLWEDGES _IOWR('d', 124, struct dkwedge_list) [DIOCLWEDGES] */
/* #define TIOCGLINED _IOR('t', 66, linedn_t) [TIOCGLINED] */
/* #define FIONWRITE _IOR('f', 121, int) [FIONWRITE] */
/* #define TIOCMSET _IOW('t', 109, int) [TIOCMSET] */
/* #define TIOCRCVFRAME _IOW('t', 69, struct mbuf *) [TIOCRCVFRAME] */
/* #define TIOCDRAIN _IO('t', 94) [TIOCDRAIN] */
/* #define TIOCEXCL _IO('t', 13) [TIOCEXCL] */
/* #define TIOCPKT_IOCTL 0x40 [TIOCPKT_IOCTL] */
/* #define FIOGETOWN _IOR('f', 123, int) [FIOGETOWN] */
/* #define SIOCGPGRP _IOR('s', 9, int) [SIOCGPGRP] */
/* #define DIOCGCACHE _IOR('d', 116, int) [DIOCGCACHE] */
/* #define TIOCUCNTL _IOW('t', 102, int) [TIOCUCNTL] */
/* #define SIOCGIFADDR _IOWR('i', 33, struct ifreq) [SIOCGIFADDR] */
/* #define SIOCGIFMETRIC _IOWR('i', 23, struct ifreq) [SIOCGIFMETRIC] */
/* #define OSIOCGIFADDR _IOWR('i', 13, struct ifreq) [OSIOCGIFADDR] */
/* #define SIOCGIFGENERIC _IOWR('i', 58, struct ifreq) [SIOCGIFGENERIC] */
/* #define SIOCGIFPSRCADDR _IOWR('i', 71, struct ifreq) [SIOCGIFPSRCADDR] */
/* #define SIOCSIFDSTADDR _IOW('i', 14, struct ifreq) [SIOCSIFDSTADDR] */
/* #define SIOCGIFPDSTADDR _IOWR('i', 72, struct ifreq) [SIOCGIFPDSTADDR] */
/* #define DIOCGDEFLABEL _IOR('d', 114, struct disklabel) [DIOCGDEFLABEL] */
/* #define FIOASYNC _IOW('f', 125, int) [FIOASYNC] */
/* #define SIOCSHIWAT _IOW('s', 0, int) [SIOCSHIWAT] */
/* #define SIOCSIFMEDIA _IOWR('i', 53, struct ifreq) [SIOCSIFMEDIA] */
/* #define TIOCSBRK _IO('t', 123) [TIOCSBRK] */
/* #define DIOCSSTEP _IOW('d', 107, int) [DIOCSSTEP] */
/* #define SIOCDIFPHYADDR _IOW('i', 73, struct ifreq) [SIOCDIFPHYADDR] */
/* #define SIOCGLIFADDR _IOWR('i', 29, struct if_laddrreq) [SIOCGLIFADDR] */
/* #define SIOCGDRVSPEC _IOWR('i', 123, struct ifdrv) [SIOCGDRVSPEC] */
/* #define SIOCDIFADDR _IOW('i', 25, struct ifreq) [SIOCDIFADDR] */
/* #define TIOCGRANTPT _IO('t', 71) [TIOCGRANTPT] */
/* #define SIOCGIFMTU _IOWR('i', 126, struct ifreq) [SIOCGIFMTU] */
/* #define SIOCGIFDLT _IOWR('i', 119, struct ifreq) [SIOCGIFDLT] */
/* #define TIOCSIG _IO('t', 95) [TIOCSIG] */
/* #define PTIOCNETBSD _IOW('Z', 0, struct ioctl_pt) [PTIOCNETBSD] */
/* #define _IOWR(g,n,t) _IOC(IOC_INOUT, (g), (n), sizeof(t)) [_IOWR(g,n,t)] */
/* #define TIOCGETA _IOR('t', 19, struct termios) [TIOCGETA] */
/* #define TIOCGETD _IOR('t', 26, int) [TIOCGETD] */
/* #define PTIOCULTRIX _IOW('Z', 6, struct ioctl_pt) [PTIOCULTRIX] */
/* #define SIOCSIFNETMASK _IOW('i', 22, struct ifreq) [SIOCSIFNETMASK] */
/* #define SIOCAIFADDR _IOW('i', 26, struct ifaliasreq) [SIOCAIFADDR] */
/* #define DIOCCACHESYNC _IOW('d', 118, int) [DIOCCACHESYNC] */
/* #define TIOCSDTR _IO('t', 121) [TIOCSDTR] */
/* #define SIOCDLIFADDR _IOW('i', 30, struct if_laddrreq) [SIOCDLIFADDR] */
/* #define DIOCGWEDGEINFO _IOR('d', 122, struct dkwedge_info) [DIOCGWEDGEINFO] */
/* #define TIOCXMTFRAME _IOW('t', 68, struct mbuf *) [TIOCXMTFRAME] */
/* #define TIOCSFLAGS _IOW('t', 92, int) [TIOCSFLAGS] */
/* #define DIOCWLABEL _IOW('d', 109, int) [DIOCWLABEL] */
/* #define TIOCSTI _IOW('t', 114, char) [TIOCSTI] */
/* #define SIOCSIFBRDADDR _IOW('i', 19, struct ifreq) [SIOCSIFBRDADDR] */
/* #define SIOCGLIFPHYADDR _IOWR('i', 75, struct if_laddrreq) [SIOCGLIFPHYADDR] */
/* #define DIOCWFORMAT _IOWR('d', 106, struct format_op) [DIOCWFORMAT] */
/* #define SIOCSPGRP _IOW('s', 8, int) [SIOCSPGRP] */
/* #define TIOCPTSNAME _IOR('t', 72, struct ptmget) [TIOCPTSNAME] */
/* #define PTIOCSUNOS _IOW('Z', 1, struct ioctl_pt) [PTIOCSUNOS] */
/* #define SIOCADDMULTI _IOW('i', 49, struct ifreq) [SIOCADDMULTI] */
/* #define SIOCZIFDATA _IOWR('i', 129, struct ifdatareq) [SIOCZIFDATA] */
/* #define SIOCGETSGCNT _IOWR('u', 52, struct sioc_sg_req) [SIOCGETSGCNT] */
/* #define SIOCALIFADDR _IOW('i', 28, struct if_laddrreq) [SIOCALIFADDR] */
/* #define TIOCSLINED _IOW('t', 67, linedn_t) [TIOCSLINED] */
/* #define SIOCGETVIFCNT _IOWR('u', 51, struct sioc_vif_req) [SIOCGETVIFCNT] */
/* #define TIOCSTART _IO('t', 110) [TIOCSTART] */
/* #define DIOCAWEDGE _IOWR('d', 121, struct dkwedge_info) [DIOCAWEDGE] */
/* #define SIOCIFGCLONERS _IOWR('i', 120, struct if_clonereq) [SIOCIFGCLONERS] */
/* #define SIOCDELRT _IOW('r', 11, struct ortentry) [SIOCDELRT] */
/* #define FIOSETOWN _IOW('f', 124, int) [FIOSETOWN] */
/* #define DIOCSCACHE _IOW('d', 117, int) [DIOCSCACHE] */
/* #define DIOCRFORMAT _IOWR('d', 105, struct format_op) [DIOCRFORMAT] */
/* #define SIOCGIFDSTADDR _IOWR('i', 34, struct ifreq) [SIOCGIFDSTADDR] */
/* #define FIONREAD _IOR('f', 127, int) [FIONREAD] */
/* #define TIOCSTAT _IOW('t', 101, int) [TIOCSTAT] */
/* #define SIOCGHIWAT _IOR('s', 1, int) [SIOCGHIWAT] */
/* #define DIOCGPART _IOW('d', 104, struct partinfo) [DIOCGPART] */
/* #define TIOCMGET _IOR('t', 106, int) [TIOCMGET] */
/* #define TIOCNXCL _IO('t', 14) [TIOCNXCL] */
/* #define SIOCSIFFLAGS _IOW('i', 16, struct ifreq) [SIOCSIFFLAGS] */
/* #define SIOCGLOWAT _IOR('s', 3, int) [SIOCGLOWAT] */
/* #define SIOCSIFMTU _IOW('i', 127, struct ifreq) [SIOCSIFMTU] */
/* #define SIOCGIFMEDIA _IOWR('i', 54, struct ifmediareq) [SIOCGIFMEDIA] */
/* #define TIOCFLUSH _IOW('t', 16, int) [TIOCFLUSH] */
/* #define DIOCEJECT _IOW('d', 112, int) [DIOCEJECT] */
/* #define TIOCGWINSZ _IOR('t', 104, struct winsize) [TIOCGWINSZ] */
/* #define SIOCGIFCAP _IOWR('i', 118, struct ifcapreq) [SIOCGIFCAP] */
/* #define TIOCGPGRP _IOR('t', 119, int) [TIOCGPGRP] */
/* #define SIOCGIFNETMASK _IOWR('i', 37, struct ifreq) [SIOCGIFNETMASK] */
/* #define TIOCPKT _IOW('t', 112, int) [TIOCPKT] */
/* #define OSIOCGIFDSTADDR _IOWR('i', 15, struct ifreq) [OSIOCGIFDSTADDR] */
/* #define DIOCBSFLUSH _IO('d', 120) [DIOCBSFLUSH] */
/* #define FIONCLEX _IO('f', 2) [FIONCLEX] */
/* #define SIOCSLIFPHYADDR _IOW('i', 74, struct if_laddrreq) [SIOCSLIFPHYADDR] */
/* #define FIOCLEX _IO('f', 1) [FIOCLEX] */
/* #define SIOCGIFBRDADDR _IOWR('i', 35, struct ifreq) [SIOCGIFBRDADDR] */
/* #define OSIOCGIFCONF _IOWR('i', 20, struct ifconf) [OSIOCGIFCONF] */
/* #define TIOCPTMGET _IOR('t', 70, struct ptmget) [TIOCPTMGET] */
/* #define TIOCEXT _IOW('t', 96, int) [TIOCEXT] */
/* #define SIOCIFDESTROY _IOW('i', 121, struct ifreq) [SIOCIFDESTROY] */
/* #define DIOCGDINFO _IOR('d', 101, struct disklabel) [DIOCGDINFO] */
/* #define SIOCGIFCONF _IOWR('i', 36, struct ifconf) [SIOCGIFCONF] */
/* #define OFIOGETBMAP _IOWR('f', 122, uint32_t) [OFIOGETBMAP] */
/* #define TIOCSCTTY _IO('t', 97) [TIOCSCTTY] */
/* #define FIONSPACE _IOR('f', 120, int) [FIONSPACE] */
/* #define DIOCCLRLABEL _IO('d', 115) [DIOCCLRLABEL] */
/* #define FIOGETBMAP _IOWR('f', 122, daddr_t) [FIOGETBMAP] */
/* #define TIOCOUTQ _IOR('t', 115, int) [TIOCOUTQ] */
/* #define TIOCCBRK _IO('t', 122) [TIOCCBRK] */
/* #define OSIOCGIFNETMASK _IOWR('i', 21, struct ifreq) [OSIOCGIFNETMASK] */
/* #define TIOCREMOTE _IOW('t', 105, int) [TIOCREMOTE] */
/* #define SIOCADDRT _IOW('r', 10, struct ortentry) [SIOCADDRT] */
/* #define SIOCGIFDATA _IOWR('i', 128, struct ifdatareq) [SIOCGIFDATA] */
/* #define DIOCWDINFO _IOW('d', 103, struct disklabel) [DIOCWDINFO] */
/* #define SIOCDELMULTI _IOW('i', 50, struct ifreq) [SIOCDELMULTI] */
/* #define OSIOCGIFBRDADDR _IOWR('i', 18, struct ifreq) [OSIOCGIFBRDADDR] */
/* #define TIOCMBIC _IOW('t', 107, int) [TIOCMBIC] */
/* #define TIOCMBIS _IOW('t', 108, int) [TIOCMBIS] */
/* #define SIOCSIFGENERIC _IOW('i', 57, struct ifreq) [SIOCSIFGENERIC] */
/* #define SIOCSDRVSPEC _IOW('i', 123, struct ifdrv) [SIOCSDRVSPEC] */
/* #define TIOCSETA _IOW('t', 20, struct termios) [TIOCSETA] */
/* #define TIOCSETD _IOW('t', 27, int) [TIOCSETD] */
/* #define TIOCCONS _IOW('t', 98, int) [TIOCCONS] */
/* #define TIOCSPGRP _IOW('t', 118, int) [TIOCSPGRP] */
/* #define TIOCGSID _IOR('t', 99, int) [TIOCGSID] */
/* #define _IO(g,n) _IOC(IOC_VOID, (g), (n), 0) [_IO(g,n)] */
/* #define TIOCCDTR _IO('t', 120) [TIOCCDTR] */
/* #define SIOCATMARK _IOR('s', 7, int) [SIOCATMARK] */
/* #define SIOCSLOWAT _IOW('s', 2, int) [SIOCSLOWAT] */
/* #define PTIOCLINUX _IOW('Z', 3, struct ioctl_pt) [PTIOCLINUX] */
/* #define DIOCSBAD _IOW('d', 110, struct dkbad) [DIOCSBAD] */
/* #define SIOCSIFADDR _IOW('i', 12, struct ifreq) [SIOCSIFADDR] */
/* #define TIOCSWINSZ _IOW('t', 103, struct winsize) [TIOCSWINSZ] */
/* #define SIOCSIFCAP _IOW('i', 117, struct ifcapreq) [SIOCSIFCAP] */
/* #define SIOCGIFFLAGS _IOWR('i', 17, struct ifreq) [SIOCGIFFLAGS] */
/* #define TIOCSETAF _IOW('t', 22, struct termios) [TIOCSETAF] */
/* #define TIOCSETAW _IOW('t', 21, struct termios) [TIOCSETAW] */
/* #define _IOC(inout,group,num,len) (inout | ((len & IOCPARM_MASK) << 16) | ((group) << 8) | (num)) [_IOC(inout,group,num,len)] */
/* #define SIOCSIFMETRIC _IOW('i', 24, struct ifreq) [SIOCSIFMETRIC] */
/* #define _IOR(g,n,t) _IOC(IOC_OUT, (g), (n), sizeof(t)) [_IOR(g,n,t)] */
/* #define _IOW(g,n,t) _IOC(IOC_IN, (g), (n), sizeof(t)) [_IOW(g,n,t)] */
/* #define ODIOCEJECT _IO('d', 112) [ODIOCEJECT] */
/* #define SIOCGIFALIAS _IOWR('i', 27, struct ifaliasreq) [SIOCGIFALIAS] */
/* #define TIOCGFLAGS _IOR('t', 93, int) [TIOCGFLAGS] */
/* #define DIOCKLABEL _IOW('d', 119, int) [DIOCKLABEL] */
/* #define DIOCDWEDGE _IOW('d', 123, struct dkwedge_info) [DIOCDWEDGE] */
/* #define TIOCNOTTY _IO('t', 113) [TIOCNOTTY] */
/* #define TIOCSTOP _IO('t', 111) [TIOCSTOP] */
/* #define PTIOCFREEBSD _IOW('Z', 4, struct ioctl_pt) [PTIOCFREEBSD] */
/* #define DIOCSDINFO _IOW('d', 102, struct disklabel) [DIOCSDINFO] */
/* #define TIOCDCDTIMESTAMP _IOR('t', 88, struct timeval) [TIOCDCDTIMESTAMP] */
/* #define SIOCIFCREATE _IOW('i', 122, struct ifreq) [SIOCIFCREATE] */
/* #define DIOCLOCK _IOW('d', 113, int) [DIOCLOCK] */
/* #define SIOCSIFPHYADDR _IOW('i', 70, struct ifaliasreq) [SIOCSIFPHYADDR] */
/* #define DIOCBSLIST _IOWR('d', 119, struct disk_badsecinfo) [DIOCBSLIST] */
/* #define TIOCGLTC _IOR('t',116,struct ltchars) [TIOCGLTC] */
	case TIOCGLTC:
		return "TIOCGLTC";
/* #define TIOCLBIC _IOW('t', 126, int) [TIOCLBIC] */
	case TIOCLBIC:
		return "TIOCLBIC";
/* #define TIOCLBIS _IOW('t', 127, int) [TIOCLBIS] */
	case TIOCLBIS:
		return "TIOCLBIS";
/* #define OTIOCSETD _IOW('t', 1, int) [OTIOCSETD] */
	case OTIOCSETD:
		return "OTIOCSETD";
/* #define OTIOCCONS _IO('t', 98) [OTIOCCONS] */
	case OTIOCCONS:
		return "OTIOCCONS";
/* #define TIOCGETC _IOR('t',18,struct tchars) [TIOCGETC] */
	case TIOCGETC:
		return "TIOCGETC";
/* #define TIOCGETP _IOR('t', 8,struct sgttyb) [TIOCGETP] */
	case TIOCGETP:
		return "TIOCGETP";
/* #define TIOCLSET _IOW('t', 125, int) [TIOCLSET] */
	case TIOCLSET:
		return "TIOCLSET";
/* #define OTIOCGETD _IOR('t', 0, int) [OTIOCGETD] */
	case OTIOCGETD:
		return "OTIOCGETD";
/* #define TIOCSLTC _IOW('t',117,struct ltchars) [TIOCSLTC] */
	case TIOCSLTC:
		return "TIOCSLTC";
/* #define TIOCLGET _IOR('t', 124, int) [TIOCLGET] */
	case TIOCLGET:
		return "TIOCLGET";
/* #define TIOCSETC _IOW('t',17,struct tchars) [TIOCSETC] */
	case TIOCSETC:
		return "TIOCSETC";
/* #define TIOCSETN _IOW('t',10,struct sgttyb) [TIOCSETN] */
	case TIOCSETN:
		return "TIOCSETN";
/* #define TIOCSETP _IOW('t', 9,struct sgttyb) [TIOCSETP] */
	case TIOCSETP:
		return "TIOCSETP";
/* #define TIOCHPCL _IO('t', 2) [TIOCHPCL] */
	case TIOCHPCL:
		return "TIOCHPCL";
/* #define TIOCREMOTE _IOW('t', 105, int) [TIOCREMOTE] */
/* #define DIOCSRETRIES _IOW('d', 108, int) [DIOCSRETRIES] */
/* #define FIONBIO _IOW('f', 126, int) [FIONBIO] */
/* #define DIOCLWEDGES _IOWR('d', 124, struct dkwedge_list) [DIOCLWEDGES] */
/* #define TIOCGLINED _IOR('t', 66, linedn_t) [TIOCGLINED] */
/* #define FIONWRITE _IOR('f', 121, int) [FIONWRITE] */
/* #define TIOCMSET _IOW('t', 109, int) [TIOCMSET] */
/* #define TIOCRCVFRAME _IOW('t', 69, struct mbuf *) [TIOCRCVFRAME] */
/* #define TIOCDRAIN _IO('t', 94) [TIOCDRAIN] */
/* #define TIOCEXCL _IO('t', 13) [TIOCEXCL] */
/* #define TIOCPKT_IOCTL 0x40 [TIOCPKT_IOCTL] */
/* #define FIOGETOWN _IOR('f', 123, int) [FIOGETOWN] */
/* #define SIOCGPGRP _IOR('s', 9, int) [SIOCGPGRP] */
/* #define DIOCGCACHE _IOR('d', 116, int) [DIOCGCACHE] */
/* #define TIOCUCNTL _IOW('t', 102, int) [TIOCUCNTL] */
/* #define SIOCGIFADDR _IOWR('i', 33, struct ifreq) [SIOCGIFADDR] */
/* #define SIOCGIFMETRIC _IOWR('i', 23, struct ifreq) [SIOCGIFMETRIC] */
/* #define OSIOCGIFADDR _IOWR('i', 13, struct ifreq) [OSIOCGIFADDR] */
/* #define SIOCGIFGENERIC _IOWR('i', 58, struct ifreq) [SIOCGIFGENERIC] */
/* #define SIOCGIFPSRCADDR _IOWR('i', 71, struct ifreq) [SIOCGIFPSRCADDR] */
/* #define SIOCSIFDSTADDR _IOW('i', 14, struct ifreq) [SIOCSIFDSTADDR] */
/* #define SIOCGIFPDSTADDR _IOWR('i', 72, struct ifreq) [SIOCGIFPDSTADDR] */
/* #define DIOCGDEFLABEL _IOR('d', 114, struct disklabel) [DIOCGDEFLABEL] */
/* #define FIOASYNC _IOW('f', 125, int) [FIOASYNC] */
/* #define SIOCSHIWAT _IOW('s', 0, int) [SIOCSHIWAT] */
/* #define SIOCSIFMEDIA _IOWR('i', 53, struct ifreq) [SIOCSIFMEDIA] */
/* #define TIOCSBRK _IO('t', 123) [TIOCSBRK] */
/* #define DIOCSSTEP _IOW('d', 107, int) [DIOCSSTEP] */
/* #define SIOCDIFPHYADDR _IOW('i', 73, struct ifreq) [SIOCDIFPHYADDR] */
/* #define SIOCGLIFADDR _IOWR('i', 29, struct if_laddrreq) [SIOCGLIFADDR] */
/* #define SIOCGDRVSPEC _IOWR('i', 123, struct ifdrv) [SIOCGDRVSPEC] */
/* #define SIOCDIFADDR _IOW('i', 25, struct ifreq) [SIOCDIFADDR] */
/* #define TIOCGRANTPT _IO('t', 71) [TIOCGRANTPT] */
/* #define SIOCGIFMTU _IOWR('i', 126, struct ifreq) [SIOCGIFMTU] */
/* #define SIOCGIFDLT _IOWR('i', 119, struct ifreq) [SIOCGIFDLT] */
/* #define TIOCSIG _IO('t', 95) [TIOCSIG] */
/* #define PTIOCNETBSD _IOW('Z', 0, struct ioctl_pt) [PTIOCNETBSD] */
/* #define _IOWR(g,n,t) _IOC(IOC_INOUT, (g), (n), sizeof(t)) [_IOWR(g,n,t)] */
/* #define TIOCGETA _IOR('t', 19, struct termios) [TIOCGETA] */
/* #define TIOCGETD _IOR('t', 26, int) [TIOCGETD] */
/* #define PTIOCULTRIX _IOW('Z', 6, struct ioctl_pt) [PTIOCULTRIX] */
/* #define SIOCSIFNETMASK _IOW('i', 22, struct ifreq) [SIOCSIFNETMASK] */
/* #define SIOCAIFADDR _IOW('i', 26, struct ifaliasreq) [SIOCAIFADDR] */
/* #define DIOCCACHESYNC _IOW('d', 118, int) [DIOCCACHESYNC] */
/* #define TIOCSETD _IOW('t', 27, int) [TIOCSETD] */
/* #define TIOCSDTR _IO('t', 121) [TIOCSDTR] */
/* #define SIOCDLIFADDR _IOW('i', 30, struct if_laddrreq) [SIOCDLIFADDR] */
/* #define OSIOCGIFCONF _IOWR('i', 20, struct ifconf) [OSIOCGIFCONF] */
/* #define DIOCGWEDGEINFO _IOR('d', 122, struct dkwedge_info) [DIOCGWEDGEINFO] */
/* #define TIOCXMTFRAME _IOW('t', 68, struct mbuf *) [TIOCXMTFRAME] */
/* #define TIOCSFLAGS _IOW('t', 92, int) [TIOCSFLAGS] */
/* #define DIOCWLABEL _IOW('d', 109, int) [DIOCWLABEL] */
/* #define TIOCSTI _IOW('t', 114, char) [TIOCSTI] */
/* #define SIOCSIFBRDADDR _IOW('i', 19, struct ifreq) [SIOCSIFBRDADDR] */
/* #define SIOCGLIFPHYADDR _IOWR('i', 75, struct if_laddrreq) [SIOCGLIFPHYADDR] */
/* #define DIOCWFORMAT _IOWR('d', 106, struct format_op) [DIOCWFORMAT] */
/* #define SIOCSPGRP _IOW('s', 8, int) [SIOCSPGRP] */
/* #define TIOCPTSNAME _IOR('t', 72, struct ptmget) [TIOCPTSNAME] */
/* #define SIOCADDMULTI _IOW('i', 49, struct ifreq) [SIOCADDMULTI] */
/* #define SIOCZIFDATA _IOWR('i', 129, struct ifdatareq) [SIOCZIFDATA] */
/* #define SIOCGETSGCNT _IOWR('u', 52, struct sioc_sg_req) [SIOCGETSGCNT] */
/* #define SIOCALIFADDR _IOW('i', 28, struct if_laddrreq) [SIOCALIFADDR] */
/* #define TIOCSLINED _IOW('t', 67, linedn_t) [TIOCSLINED] */
/* #define SIOCGETVIFCNT _IOWR('u', 51, struct sioc_vif_req) [SIOCGETVIFCNT] */
/* #define PTIOCSUNOS _IOW('Z', 1, struct ioctl_pt) [PTIOCSUNOS] */
/* #define TIOCSTART _IO('t', 110) [TIOCSTART] */
/* #define DIOCAWEDGE _IOWR('d', 121, struct dkwedge_info) [DIOCAWEDGE] */
/* #define SIOCIFGCLONERS _IOWR('i', 120, struct if_clonereq) [SIOCIFGCLONERS] */
/* #define SIOCDELRT _IOW('r', 11, struct ortentry) [SIOCDELRT] */
/* #define FIOSETOWN _IOW('f', 124, int) [FIOSETOWN] */
/* #define DIOCSCACHE _IOW('d', 117, int) [DIOCSCACHE] */
/* #define DIOCRFORMAT _IOWR('d', 105, struct format_op) [DIOCRFORMAT] */
/* #define SIOCGIFDSTADDR _IOWR('i', 34, struct ifreq) [SIOCGIFDSTADDR] */
/* #define TIOCSTAT _IOW('t', 101, int) [TIOCSTAT] */
/* #define SIOCGHIWAT _IOR('s', 1, int) [SIOCGHIWAT] */
/* #define DIOCGPART _IOW('d', 104, struct partinfo) [DIOCGPART] */
/* #define TIOCMGET _IOR('t', 106, int) [TIOCMGET] */
/* #define TIOCNXCL _IO('t', 14) [TIOCNXCL] */
/* #define SIOCSIFFLAGS _IOW('i', 16, struct ifreq) [SIOCSIFFLAGS] */
/* #define SIOCGLOWAT _IOR('s', 3, int) [SIOCGLOWAT] */
/* #define SIOCSIFMTU _IOW('i', 127, struct ifreq) [SIOCSIFMTU] */
/* #define SIOCGIFMEDIA _IOWR('i', 54, struct ifmediareq) [SIOCGIFMEDIA] */
/* #define TIOCFLUSH _IOW('t', 16, int) [TIOCFLUSH] */
/* #define DIOCEJECT _IOW('d', 112, int) [DIOCEJECT] */
/* #define TIOCGWINSZ _IOR('t', 104, struct winsize) [TIOCGWINSZ] */
/* #define SIOCGIFCAP _IOWR('i', 118, struct ifcapreq) [SIOCGIFCAP] */
/* #define TIOCGPGRP _IOR('t', 119, int) [TIOCGPGRP] */
/* #define SIOCGIFNETMASK _IOWR('i', 37, struct ifreq) [SIOCGIFNETMASK] */
/* #define TIOCPKT _IOW('t', 112, int) [TIOCPKT] */
/* #define OSIOCGIFDSTADDR _IOWR('i', 15, struct ifreq) [OSIOCGIFDSTADDR] */
/* #define DIOCBSFLUSH _IO('d', 120) [DIOCBSFLUSH] */
/* #define FIONCLEX _IO('f', 2) [FIONCLEX] */
/* #define SIOCSLIFPHYADDR _IOW('i', 74, struct if_laddrreq) [SIOCSLIFPHYADDR] */
/* #define FIOCLEX _IO('f', 1) [FIOCLEX] */
/* #define SIOCGIFBRDADDR _IOWR('i', 35, struct ifreq) [SIOCGIFBRDADDR] */
/* #define TIOCPTMGET _IOR('t', 70, struct ptmget) [TIOCPTMGET] */
/* #define TIOCEXT _IOW('t', 96, int) [TIOCEXT] */
/* #define SIOCIFDESTROY _IOW('i', 121, struct ifreq) [SIOCIFDESTROY] */
/* #define DIOCGDINFO _IOR('d', 101, struct disklabel) [DIOCGDINFO] */
/* #define SIOCGIFCONF _IOWR('i', 36, struct ifconf) [SIOCGIFCONF] */
/* #define OFIOGETBMAP _IOWR('f', 122, uint32_t) [OFIOGETBMAP] */
/* #define TIOCSCTTY _IO('t', 97) [TIOCSCTTY] */
/* #define FIONSPACE _IOR('f', 120, int) [FIONSPACE] */
/* #define DIOCCLRLABEL _IO('d', 115) [DIOCCLRLABEL] */
/* #define FIOGETBMAP _IOWR('f', 122, daddr_t) [FIOGETBMAP] */
/* #define TIOCOUTQ _IOR('t', 115, int) [TIOCOUTQ] */
/* #define TIOCCBRK _IO('t', 122) [TIOCCBRK] */
/* #define OSIOCGIFNETMASK _IOWR('i', 21, struct ifreq) [OSIOCGIFNETMASK] */
/* #define SIOCADDRT _IOW('r', 10, struct ortentry) [SIOCADDRT] */
/* #define SIOCGIFDATA _IOWR('i', 128, struct ifdatareq) [SIOCGIFDATA] */
/* #define DIOCWDINFO _IOW('d', 103, struct disklabel) [DIOCWDINFO] */
/* #define SIOCDELMULTI _IOW('i', 50, struct ifreq) [SIOCDELMULTI] */
/* #define OSIOCGIFBRDADDR _IOWR('i', 18, struct ifreq) [OSIOCGIFBRDADDR] */
/* #define TIOCMBIC _IOW('t', 107, int) [TIOCMBIC] */
/* #define TIOCMBIS _IOW('t', 108, int) [TIOCMBIS] */
/* #define SIOCSIFGENERIC _IOW('i', 57, struct ifreq) [SIOCSIFGENERIC] */
/* #define SIOCSDRVSPEC _IOW('i', 123, struct ifdrv) [SIOCSDRVSPEC] */
/* #define TIOCSETA _IOW('t', 20, struct termios) [TIOCSETA] */
/* #define TIOCCONS _IOW('t', 98, int) [TIOCCONS] */
/* #define TIOCSPGRP _IOW('t', 118, int) [TIOCSPGRP] */
/* #define TIOCGSID _IOR('t', 99, int) [TIOCGSID] */
/* #define FIONREAD _IOR('f', 127, int) [FIONREAD] */
/* #define _IO(g,n) _IOC(IOC_VOID, (g), (n), 0) [_IO(g,n)] */
/* #define TIOCCDTR _IO('t', 120) [TIOCCDTR] */
/* #define SIOCATMARK _IOR('s', 7, int) [SIOCATMARK] */
/* #define SIOCSLOWAT _IOW('s', 2, int) [SIOCSLOWAT] */
/* #define PTIOCLINUX _IOW('Z', 3, struct ioctl_pt) [PTIOCLINUX] */
/* #define DIOCSBAD _IOW('d', 110, struct dkbad) [DIOCSBAD] */
/* #define SIOCSIFADDR _IOW('i', 12, struct ifreq) [SIOCSIFADDR] */
/* #define TIOCSWINSZ _IOW('t', 103, struct winsize) [TIOCSWINSZ] */
/* #define SIOCSIFCAP _IOW('i', 117, struct ifcapreq) [SIOCSIFCAP] */
/* #define SIOCGIFFLAGS _IOWR('i', 17, struct ifreq) [SIOCGIFFLAGS] */
/* #define TIOCSETAF _IOW('t', 22, struct termios) [TIOCSETAF] */
/* #define TIOCSETAW _IOW('t', 21, struct termios) [TIOCSETAW] */
/* #define _IOC(inout,group,num,len) (inout | ((len & IOCPARM_MASK) << 16) | ((group) << 8) | (num)) [_IOC(inout,group,num,len)] */
/* #define SIOCSIFMETRIC _IOW('i', 24, struct ifreq) [SIOCSIFMETRIC] */
/* #define _IOR(g,n,t) _IOC(IOC_OUT, (g), (n), sizeof(t)) [_IOR(g,n,t)] */
/* #define _IOW(g,n,t) _IOC(IOC_IN, (g), (n), sizeof(t)) [_IOW(g,n,t)] */
/* #define ODIOCEJECT _IO('d', 112) [ODIOCEJECT] */
/* #define SIOCGIFALIAS _IOWR('i', 27, struct ifaliasreq) [SIOCGIFALIAS] */
/* #define TIOCGFLAGS _IOR('t', 93, int) [TIOCGFLAGS] */
/* #define DIOCKLABEL _IOW('d', 119, int) [DIOCKLABEL] */
/* #define DIOCDWEDGE _IOW('d', 123, struct dkwedge_info) [DIOCDWEDGE] */
/* #define TIOCNOTTY _IO('t', 113) [TIOCNOTTY] */
/* #define TIOCSTOP _IO('t', 111) [TIOCSTOP] */
/* #define PTIOCFREEBSD _IOW('Z', 4, struct ioctl_pt) [PTIOCFREEBSD] */
/* #define DIOCSDINFO _IOW('d', 102, struct disklabel) [DIOCSDINFO] */
/* #define TIOCDCDTIMESTAMP _IOR('t', 88, struct timeval) [TIOCDCDTIMESTAMP] */
/* #define SIOCIFCREATE _IOW('i', 122, struct ifreq) [SIOCIFCREATE] */
/* #define DIOCLOCK _IOW('d', 113, int) [DIOCLOCK] */
/* #define SIOCSIFPHYADDR _IOW('i', 70, struct ifaliasreq) [SIOCSIFPHYADDR] */
/* #define DIOCBSLIST _IOWR('d', 119, struct disk_badsecinfo) [DIOCBSLIST] */
/* #define KIOCGSYMBOL _IOW('l', 1, struct ksyms_gsymbol) [KIOCGSYMBOL] */
	case KIOCGSYMBOL:
		return "KIOCGSYMBOL";
/* #define KIOCGSIZE _IOR('l', 3, int) [KIOCGSIZE] */
	case KIOCGSIZE:
		return "KIOCGSIZE";
/* #define KIOCGVALUE _IOW('l', 2, struct ksyms_gsymbol) [KIOCGVALUE] */
	case KIOCGVALUE:
		return "KIOCGVALUE";
/* #define LMLOADBUF _IOW('K', 1, struct lmc_loadbuf) [LMLOADBUF] */
	case LMLOADBUF:
		return "LMLOADBUF";
/* #define LMREADY _IOW('K', 3, u_long) [LMREADY] */
	case LMREADY:
		return "LMREADY";
/* #define LMUNRESRV _IO('K', 2) [LMUNRESRV] */
	case LMUNRESRV:
		return "LMUNRESRV";
/* #define LMLOADSYMS _IOW('K', 12, struct lmc_loadbuf) [LMLOADSYMS] */
	case LMLOADSYMS:
		return "LMLOADSYMS";
/* #define LMSTAT _IOWR('K', 11, struct lmc_stat) [LMSTAT] */
	case LMSTAT:
		return "LMSTAT";
/* #define LMFORCE _IOW('K', 13, u_long) [LMFORCE] */
	case LMFORCE:
		return "LMFORCE";
/* #define LMUNLOAD _IOWR('K', 10, struct lmc_unload) [LMUNLOAD] */
	case LMUNLOAD:
		return "LMUNLOAD";
/* #define LMLOAD _IOW('K', 9, struct lmc_load) [LMLOAD] */
	case LMLOAD:
		return "LMLOAD";
/* #define LMRESERV _IOWR('K', 4, struct lmc_resrv) [LMRESERV] */
	case LMRESERV:
		return "LMRESERV";
/* #define MTIOCIEOT _IO('m', 3) [MTIOCIEOT] */
	case MTIOCIEOT:
		return "MTIOCIEOT";
/* #define MTIOCSLOCATE _IOW('m', 5, u_int32_t) [MTIOCSLOCATE] */
	case MTIOCSLOCATE:
		return "MTIOCSLOCATE";
/* #define MTIOCEEOT _IO('m', 4) [MTIOCEEOT] */
	case MTIOCEEOT:
		return "MTIOCEEOT";
/* #define MTIOCHLOCATE _IOW('m', 6, u_int32_t) [MTIOCHLOCATE] */
	case MTIOCHLOCATE:
		return "MTIOCHLOCATE";
/* #define MTIOCRDSPOS _IOR('m', 5, u_int32_t) [MTIOCRDSPOS] */
	case MTIOCRDSPOS:
		return "MTIOCRDSPOS";
/* #define MTIOCGET _IOR('m', 2, struct mtget) [MTIOCGET] */
	case MTIOCGET:
		return "MTIOCGET";
/* #define MTIOCRDHPOS _IOR('m', 6, u_int32_t) [MTIOCRDHPOS] */
	case MTIOCRDHPOS:
		return "MTIOCRDHPOS";
/* #define MTIOCTOP _IOW('m', 1, struct mtop) [MTIOCTOP] */
	case MTIOCTOP:
		return "MTIOCTOP";
/* #define _IOWR(g,n,t) _IOC(IOC_INOUT, (g), (n), sizeof(t)) [_IOWR(g,n,t)] */
/* #define POWER_IOC_GET_TYPE _IOR('P', 0, sizeof(struct power_type)) [POWER_IOC_GET_TYPE] */
	case POWER_IOC_GET_TYPE:
		return "POWER_IOC_GET_TYPE";
/* #define _IO(g,n) _IOC(IOC_VOID, (g), (n), 0) [_IO(g,n)] */
/* #define _IOC(inout,group,num,len) (inout | ((len & IOCPARM_MASK) << 16) | ((group) << 8) | (num)) [_IOC(inout,group,num,len)] */
/* #define _IOR(g,n,t) _IOC(IOC_OUT, (g), (n), sizeof(t)) [_IOR(g,n,t)] */
/* #define _IOW(g,n,t) _IOC(IOC_IN, (g), (n), sizeof(t)) [_IOW(g,n,t)] */
/* #define BLKDEV_IOSIZE 2048 [BLKDEV_IOSIZE] */
/* #define RIOCGINFO _IOR('R', 21, struct radio_info) [RIOCGINFO] */
	case RIOCGINFO:
		return "RIOCGINFO";
/* #define RIOCSINFO _IOWR('R', 22, struct radio_info) [RIOCSINFO] */
	case RIOCSINFO:
		return "RIOCSINFO";
/* #define RIOCSSRCH _IOW('R', 23, int) [RIOCSSRCH] */
	case RIOCSSRCH:
		return "RIOCSSRCH";
/* #define RNDGETPOOLSTAT _IOR('R', 106, rndpoolstat_t) [RNDGETPOOLSTAT] */
	case RNDGETPOOLSTAT:
		return "RNDGETPOOLSTAT";
/* #define RNDADDDATA _IOW('R', 105, rnddata_t) [RNDADDDATA] */
	case RNDADDDATA:
		return "RNDADDDATA";
/* #define RNDGETSRCNAME _IOWR('R', 103, rndstat_name_t) [RNDGETSRCNAME] */
	case RNDGETSRCNAME:
		return "RNDGETSRCNAME";
/* #define RNDGETENTCNT _IOR('R', 101, u_int32_t) [RNDGETENTCNT] */
	case RNDGETENTCNT:
		return "RNDGETENTCNT";
/* #define RNDGETSRCNUM _IOWR('R', 102, rndstat_t) [RNDGETSRCNUM] */
	case RNDGETSRCNUM:
		return "RNDGETSRCNUM";
/* #define RNDCTL _IOW('R', 104, rndctl_t) [RNDCTL] */
	case RNDCTL:
		return "RNDCTL";
/* #define SCIOCSET _IOW('S', 2, struct scan_io) [SCIOCSET] */
	case SCIOCSET:
		return "SCIOCSET";
/* #define SCIOCRESTART _IO('S', 3) [SCIOCRESTART] */
	case SCIOCRESTART:
		return "SCIOCRESTART";
/* #define SCIOCGET _IOR('S', 1, struct scan_io) [SCIOCGET] */
	case SCIOCGET:
		return "SCIOCGET";
/* #define TIOCREMOTE _IOW('t', 105, int) [TIOCREMOTE] */
/* #define DIOCSRETRIES _IOW('d', 108, int) [DIOCSRETRIES] */
/* #define FIONBIO _IOW('f', 126, int) [FIONBIO] */
/* #define SCBUSIODETACH _IOW('U', 2, struct scbusiodetach_args) [SCBUSIODETACH] */
	case SCBUSIODETACH:
		return "SCBUSIODETACH";
/* #define DIOCLWEDGES _IOWR('d', 124, struct dkwedge_list) [DIOCLWEDGES] */
/* #define TIOCGLINED _IOR('t', 66, linedn_t) [TIOCGLINED] */
/* #define FIONWRITE _IOR('f', 121, int) [FIONWRITE] */
/* #define TIOCMSET _IOW('t', 109, int) [TIOCMSET] */
/* #define TIOCRCVFRAME _IOW('t', 69, struct mbuf *) [TIOCRCVFRAME] */
/* #define TIOCDRAIN _IO('t', 94) [TIOCDRAIN] */
/* #define TIOCEXCL _IO('t', 13) [TIOCEXCL] */
/* #define SCBUSIORESET _IO('U', 1) [SCBUSIORESET] */
	case SCBUSIORESET:
		return "SCBUSIORESET";
/* #define TIOCPKT_IOCTL 0x40 [TIOCPKT_IOCTL] */
/* #define FIOGETOWN _IOR('f', 123, int) [FIOGETOWN] */
/* #define SIOCGPGRP _IOR('s', 9, int) [SIOCGPGRP] */
/* #define DIOCGCACHE _IOR('d', 116, int) [DIOCGCACHE] */
/* #define TIOCUCNTL _IOW('t', 102, int) [TIOCUCNTL] */
/* #define SIOCGIFADDR _IOWR('i', 33, struct ifreq) [SIOCGIFADDR] */
/* #define SIOCGIFMETRIC _IOWR('i', 23, struct ifreq) [SIOCGIFMETRIC] */
/* #define OSIOCGIFADDR _IOWR('i', 13, struct ifreq) [OSIOCGIFADDR] */
/* #define SIOCGIFGENERIC _IOWR('i', 58, struct ifreq) [SIOCGIFGENERIC] */
/* #define SIOCGIFPSRCADDR _IOWR('i', 71, struct ifreq) [SIOCGIFPSRCADDR] */
/* #define SIOCSIFDSTADDR _IOW('i', 14, struct ifreq) [SIOCSIFDSTADDR] */
/* #define SIOCGIFPDSTADDR _IOWR('i', 72, struct ifreq) [SIOCGIFPDSTADDR] */
/* #define DIOCGDEFLABEL _IOR('d', 114, struct disklabel) [DIOCGDEFLABEL] */
/* #define FIOASYNC _IOW('f', 125, int) [FIOASYNC] */
/* #define SIOCSHIWAT _IOW('s', 0, int) [SIOCSHIWAT] */
/* #define SIOCSIFMEDIA _IOWR('i', 53, struct ifreq) [SIOCSIFMEDIA] */
/* #define TIOCSBRK _IO('t', 123) [TIOCSBRK] */
/* #define DIOCSSTEP _IOW('d', 107, int) [DIOCSSTEP] */
/* #define SIOCDIFPHYADDR _IOW('i', 73, struct ifreq) [SIOCDIFPHYADDR] */
/* #define SIOCGLIFADDR _IOWR('i', 29, struct if_laddrreq) [SIOCGLIFADDR] */
/* #define SIOCGDRVSPEC _IOWR('i', 123, struct ifdrv) [SIOCGDRVSPEC] */
/* #define SIOCDIFADDR _IOW('i', 25, struct ifreq) [SIOCDIFADDR] */
/* #define TIOCGRANTPT _IO('t', 71) [TIOCGRANTPT] */
/* #define SIOCGIFMTU _IOWR('i', 126, struct ifreq) [SIOCGIFMTU] */
/* #define SIOCGIFDLT _IOWR('i', 119, struct ifreq) [SIOCGIFDLT] */
/* #define TIOCSIG _IO('t', 95) [TIOCSIG] */
/* #define PTIOCNETBSD _IOW('Z', 0, struct ioctl_pt) [PTIOCNETBSD] */
/* #define OSCIOCIDENTIFY _IOR('Q', 4, struct oscsi_addr) [OSCIOCIDENTIFY] */
	case OSCIOCIDENTIFY:
		return "OSCIOCIDENTIFY";
/* #define _IOWR(g,n,t) _IOC(IOC_INOUT, (g), (n), sizeof(t)) [_IOWR(g,n,t)] */
/* #define TIOCGETA _IOR('t', 19, struct termios) [TIOCGETA] */
/* #define TIOCGETD _IOR('t', 26, int) [TIOCGETD] */
/* #define PTIOCULTRIX _IOW('Z', 6, struct ioctl_pt) [PTIOCULTRIX] */
/* #define SIOCSIFNETMASK _IOW('i', 22, struct ifreq) [SIOCSIFNETMASK] */
/* #define SIOCAIFADDR _IOW('i', 26, struct ifaliasreq) [SIOCAIFADDR] */
/* #define DIOCCACHESYNC _IOW('d', 118, int) [DIOCCACHESYNC] */
/* #define TIOCSETD _IOW('t', 27, int) [TIOCSETD] */
/* #define TIOCSDTR _IO('t', 121) [TIOCSDTR] */
/* #define SIOCDLIFADDR _IOW('i', 30, struct if_laddrreq) [SIOCDLIFADDR] */
/* #define OSIOCGIFCONF _IOWR('i', 20, struct ifconf) [OSIOCGIFCONF] */
/* #define DIOCGWEDGEINFO _IOR('d', 122, struct dkwedge_info) [DIOCGWEDGEINFO] */
/* #define TIOCXMTFRAME _IOW('t', 68, struct mbuf *) [TIOCXMTFRAME] */
/* #define TIOCSFLAGS _IOW('t', 92, int) [TIOCSFLAGS] */
/* #define DIOCWLABEL _IOW('d', 109, int) [DIOCWLABEL] */
/* #define TIOCSTI _IOW('t', 114, char) [TIOCSTI] */
/* #define SIOCSIFBRDADDR _IOW('i', 19, struct ifreq) [SIOCSIFBRDADDR] */
/* #define SIOCGLIFPHYADDR _IOWR('i', 75, struct if_laddrreq) [SIOCGLIFPHYADDR] */
/* #define DIOCWFORMAT _IOWR('d', 106, struct format_op) [DIOCWFORMAT] */
/* #define SIOCSPGRP _IOW('s', 8, int) [SIOCSPGRP] */
/* #define TIOCPTSNAME _IOR('t', 72, struct ptmget) [TIOCPTSNAME] */
/* #define SIOCADDMULTI _IOW('i', 49, struct ifreq) [SIOCADDMULTI] */
/* #define SIOCZIFDATA _IOWR('i', 129, struct ifdatareq) [SIOCZIFDATA] */
/* #define SIOCGETSGCNT _IOWR('u', 52, struct sioc_sg_req) [SIOCGETSGCNT] */
/* #define SIOCALIFADDR _IOW('i', 28, struct if_laddrreq) [SIOCALIFADDR] */
/* #define TIOCSLINED _IOW('t', 67, linedn_t) [TIOCSLINED] */
/* #define SIOCGETVIFCNT _IOWR('u', 51, struct sioc_vif_req) [SIOCGETVIFCNT] */
/* #define PTIOCSUNOS _IOW('Z', 1, struct ioctl_pt) [PTIOCSUNOS] */
/* #define TIOCSTART _IO('t', 110) [TIOCSTART] */
/* #define DIOCAWEDGE _IOWR('d', 121, struct dkwedge_info) [DIOCAWEDGE] */
/* #define SIOCIFGCLONERS _IOWR('i', 120, struct if_clonereq) [SIOCIFGCLONERS] */
/* #define SIOCDELRT _IOW('r', 11, struct ortentry) [SIOCDELRT] */
/* #define FIOSETOWN _IOW('f', 124, int) [FIOSETOWN] */
/* #define DIOCSCACHE _IOW('d', 117, int) [DIOCSCACHE] */
/* #define SCBUSACCEL _IOW('U', 2, struct scbusaccel_args) [SCBUSACCEL] */
	case SCBUSACCEL:
		return "SCBUSACCEL";
/* #define DIOCRFORMAT _IOWR('d', 105, struct format_op) [DIOCRFORMAT] */
/* #define SIOCGIFDSTADDR _IOWR('i', 34, struct ifreq) [SIOCGIFDSTADDR] */
/* #define TIOCSTAT _IOW('t', 101, int) [TIOCSTAT] */
/* #define SIOCGHIWAT _IOR('s', 1, int) [SIOCGHIWAT] */
/* #define DIOCGPART _IOW('d', 104, struct partinfo) [DIOCGPART] */
/* #define TIOCMGET _IOR('t', 106, int) [TIOCMGET] */
/* #define SCIOCIDENTIFY _IOR('Q', 4, struct scsi_addr) [SCIOCIDENTIFY] */
	case SCIOCIDENTIFY:
		return "SCIOCIDENTIFY";
/* #define TIOCNXCL _IO('t', 14) [TIOCNXCL] */
/* #define SIOCSIFFLAGS _IOW('i', 16, struct ifreq) [SIOCSIFFLAGS] */
/* #define SIOCGLOWAT _IOR('s', 3, int) [SIOCGLOWAT] */
/* #define SIOCSIFMTU _IOW('i', 127, struct ifreq) [SIOCSIFMTU] */
/* #define SIOCGIFMEDIA _IOWR('i', 54, struct ifmediareq) [SIOCGIFMEDIA] */
/* #define TIOCFLUSH _IOW('t', 16, int) [TIOCFLUSH] */
/* #define DIOCEJECT _IOW('d', 112, int) [DIOCEJECT] */
/* #define TIOCGWINSZ _IOR('t', 104, struct winsize) [TIOCGWINSZ] */
/* #define SIOCGIFCAP _IOWR('i', 118, struct ifcapreq) [SIOCGIFCAP] */
/* #define TIOCGPGRP _IOR('t', 119, int) [TIOCGPGRP] */
/* #define SIOCGIFNETMASK _IOWR('i', 37, struct ifreq) [SIOCGIFNETMASK] */
/* #define SCBUSIOSCAN _IOW('U', 0, struct scbusioscan_args) [SCBUSIOSCAN] */
	case SCBUSIOSCAN:
		return "SCBUSIOSCAN";
/* #define TIOCPKT _IOW('t', 112, int) [TIOCPKT] */
/* #define SCIOCRESET _IO('Q', 7) [SCIOCRESET] */
	case SCIOCRESET:
		return "SCIOCRESET";
/* #define OSIOCGIFDSTADDR _IOWR('i', 15, struct ifreq) [OSIOCGIFDSTADDR] */
/* #define DIOCBSFLUSH _IO('d', 120) [DIOCBSFLUSH] */
/* #define FIONCLEX _IO('f', 2) [FIONCLEX] */
/* #define SIOCSLIFPHYADDR _IOW('i', 74, struct if_laddrreq) [SIOCSLIFPHYADDR] */
/* #define FIOCLEX _IO('f', 1) [FIOCLEX] */
/* #define SIOCGIFBRDADDR _IOWR('i', 35, struct ifreq) [SIOCGIFBRDADDR] */
/* #define TIOCPTMGET _IOR('t', 70, struct ptmget) [TIOCPTMGET] */
/* #define TIOCEXT _IOW('t', 96, int) [TIOCEXT] */
/* #define SIOCIFDESTROY _IOW('i', 121, struct ifreq) [SIOCIFDESTROY] */
/* #define DIOCGDINFO _IOR('d', 101, struct disklabel) [DIOCGDINFO] */
/* #define SIOCGIFCONF _IOWR('i', 36, struct ifconf) [SIOCGIFCONF] */
/* #define OFIOGETBMAP _IOWR('f', 122, uint32_t) [OFIOGETBMAP] */
/* #define TIOCSCTTY _IO('t', 97) [TIOCSCTTY] */
/* #define SCCMD_IOV 0x00000004 [SCCMD_IOV] */
	case SCCMD_IOV:
		return "SCCMD_IOV";
/* #define FIONSPACE _IOR('f', 120, int) [FIONSPACE] */
/* #define DIOCCLRLABEL _IO('d', 115) [DIOCCLRLABEL] */
/* #define FIOGETBMAP _IOWR('f', 122, daddr_t) [FIOGETBMAP] */
/* #define TIOCOUTQ _IOR('t', 115, int) [TIOCOUTQ] */
/* #define TIOCCBRK _IO('t', 122) [TIOCCBRK] */
/* #define OSIOCGIFNETMASK _IOWR('i', 21, struct ifreq) [OSIOCGIFNETMASK] */
/* #define SCIOCRECONFIG _IO('Q', 6) [SCIOCRECONFIG] */
	case SCIOCRECONFIG:
		return "SCIOCRECONFIG";
/* #define SIOCADDRT _IOW('r', 10, struct ortentry) [SIOCADDRT] */
/* #define SIOCGIFDATA _IOWR('i', 128, struct ifdatareq) [SIOCGIFDATA] */
/* #define DIOCWDINFO _IOW('d', 103, struct disklabel) [DIOCWDINFO] */
/* #define SCIOCDEBUG _IOW('Q', 2, int) [SCIOCDEBUG] */
	case SCIOCDEBUG:
		return "SCIOCDEBUG";
/* #define SIOCDELMULTI _IOW('i', 50, struct ifreq) [SIOCDELMULTI] */
/* #define OSIOCGIFBRDADDR _IOWR('i', 18, struct ifreq) [OSIOCGIFBRDADDR] */
/* #define TIOCMBIC _IOW('t', 107, int) [TIOCMBIC] */
/* #define TIOCMBIS _IOW('t', 108, int) [TIOCMBIS] */
/* #define SIOCSIFGENERIC _IOW('i', 57, struct ifreq) [SIOCSIFGENERIC] */
/* #define SIOCSDRVSPEC _IOW('i', 123, struct ifdrv) [SIOCSDRVSPEC] */
/* #define TIOCSETA _IOW('t', 20, struct termios) [TIOCSETA] */
/* #define TIOCCONS _IOW('t', 98, int) [TIOCCONS] */
/* #define TIOCSPGRP _IOW('t', 118, int) [TIOCSPGRP] */
/* #define TIOCGSID _IOR('t', 99, int) [TIOCGSID] */
/* #define FIONREAD _IOR('f', 127, int) [FIONREAD] */
/* #define _IO(g,n) _IOC(IOC_VOID, (g), (n), 0) [_IO(g,n)] */
/* #define TIOCCDTR _IO('t', 120) [TIOCCDTR] */
/* #define SIOCATMARK _IOR('s', 7, int) [SIOCATMARK] */
/* #define SCIOCCOMMAND _IOWR('Q', 1, scsireq_t) [SCIOCCOMMAND] */
	case SCIOCCOMMAND:
		return "SCIOCCOMMAND";
/* #define SIOCSLOWAT _IOW('s', 2, int) [SIOCSLOWAT] */
/* #define PTIOCLINUX _IOW('Z', 3, struct ioctl_pt) [PTIOCLINUX] */
/* #define DIOCSBAD _IOW('d', 110, struct dkbad) [DIOCSBAD] */
/* #define SCIOCDECONFIG _IO('Q', 5) [SCIOCDECONFIG] */
	case SCIOCDECONFIG:
		return "SCIOCDECONFIG";
/* #define SIOCSIFADDR _IOW('i', 12, struct ifreq) [SIOCSIFADDR] */
/* #define TIOCSWINSZ _IOW('t', 103, struct winsize) [TIOCSWINSZ] */
/* #define SIOCSIFCAP _IOW('i', 117, struct ifcapreq) [SIOCSIFCAP] */
/* #define SIOCGIFFLAGS _IOWR('i', 17, struct ifreq) [SIOCGIFFLAGS] */
/* #define SCBUSIOLLSCAN _IO('U', 3) [SCBUSIOLLSCAN] */
	case SCBUSIOLLSCAN:
		return "SCBUSIOLLSCAN";
/* #define TIOCSETAF _IOW('t', 22, struct termios) [TIOCSETAF] */
/* #define TIOCSETAW _IOW('t', 21, struct termios) [TIOCSETAW] */
/* #define _IOC(inout,group,num,len) (inout | ((len & IOCPARM_MASK) << 16) | ((group) << 8) | (num)) [_IOC(inout,group,num,len)] */
/* #define SIOCSIFMETRIC _IOW('i', 24, struct ifreq) [SIOCSIFMETRIC] */
/* #define _IOR(g,n,t) _IOC(IOC_OUT, (g), (n), sizeof(t)) [_IOR(g,n,t)] */
/* #define _IOW(g,n,t) _IOC(IOC_IN, (g), (n), sizeof(t)) [_IOW(g,n,t)] */
/* #define ODIOCEJECT _IO('d', 112) [ODIOCEJECT] */
/* #define SIOCGIFALIAS _IOWR('i', 27, struct ifaliasreq) [SIOCGIFALIAS] */
/* #define TIOCGFLAGS _IOR('t', 93, int) [TIOCGFLAGS] */
/* #define DIOCKLABEL _IOW('d', 119, int) [DIOCKLABEL] */
/* #define DIOCDWEDGE _IOW('d', 123, struct dkwedge_info) [DIOCDWEDGE] */
/* #define TIOCNOTTY _IO('t', 113) [TIOCNOTTY] */
/* #define TIOCSTOP _IO('t', 111) [TIOCSTOP] */
/* #define PTIOCFREEBSD _IOW('Z', 4, struct ioctl_pt) [PTIOCFREEBSD] */
/* #define DIOCSDINFO _IOW('d', 102, struct disklabel) [DIOCSDINFO] */
/* #define TIOCDCDTIMESTAMP _IOR('t', 88, struct timeval) [TIOCDCDTIMESTAMP] */
/* #define SIOCIFCREATE _IOW('i', 122, struct ifreq) [SIOCIFCREATE] */
/* #define DIOCLOCK _IOW('d', 113, int) [DIOCLOCK] */
/* #define SIOCSIFPHYADDR _IOW('i', 70, struct ifaliasreq) [SIOCSIFPHYADDR] */
/* #define DIOCBSLIST _IOWR('d', 119, struct disk_badsecinfo) [DIOCBSLIST] */
/* #define SIOCGPGRP _IOR('s', 9, int) [SIOCGPGRP] */
/* #define SIOCGIFMETRIC _IOWR('i', 23, struct ifreq) [SIOCGIFMETRIC] */
/* #define OSIOCGIFADDR _IOWR('i', 13, struct ifreq) [OSIOCGIFADDR] */
/* #define SIOCGIFGENERIC _IOWR('i', 58, struct ifreq) [SIOCGIFGENERIC] */
/* #define SIOCGIFPSRCADDR _IOWR('i', 71, struct ifreq) [SIOCGIFPSRCADDR] */
/* #define SIOCSIFDSTADDR _IOW('i', 14, struct ifreq) [SIOCSIFDSTADDR] */
/* #define SIOCGIFPDSTADDR _IOWR('i', 72, struct ifreq) [SIOCGIFPDSTADDR] */
/* #define SIOCGIFADDR _IOWR('i', 33, struct ifreq) [SIOCGIFADDR] */
/* #define SIOCSHIWAT _IOW('s', 0, int) [SIOCSHIWAT] */
/* #define SIOCSIFMEDIA _IOWR('i', 53, struct ifreq) [SIOCSIFMEDIA] */
/* #define SIOCDIFPHYADDR _IOW('i', 73, struct ifreq) [SIOCDIFPHYADDR] */
/* #define SIOCGLIFADDR _IOWR('i', 29, struct if_laddrreq) [SIOCGLIFADDR] */
/* #define SIOCGDRVSPEC _IOWR('i', 123, struct ifdrv) [SIOCGDRVSPEC] */
/* #define SIOCDIFADDR _IOW('i', 25, struct ifreq) [SIOCDIFADDR] */
/* #define SIOCGIFMTU _IOWR('i', 126, struct ifreq) [SIOCGIFMTU] */
/* #define SIOCGIFDLT _IOWR('i', 119, struct ifreq) [SIOCGIFDLT] */
/* #define _IOWR(g,n,t) _IOC(IOC_INOUT, (g), (n), sizeof(t)) [_IOWR(g,n,t)] */
/* #define SIOCSIFNETMASK _IOW('i', 22, struct ifreq) [SIOCSIFNETMASK] */
/* #define SIOCAIFADDR _IOW('i', 26, struct ifaliasreq) [SIOCAIFADDR] */
/* #define SIOCDLIFADDR _IOW('i', 30, struct if_laddrreq) [SIOCDLIFADDR] */
/* #define SIOCSIFBRDADDR _IOW('i', 19, struct ifreq) [SIOCSIFBRDADDR] */
/* #define SIOCGLIFPHYADDR _IOWR('i', 75, struct if_laddrreq) [SIOCGLIFPHYADDR] */
/* #define SIOCSPGRP _IOW('s', 8, int) [SIOCSPGRP] */
/* #define SIOCADDMULTI _IOW('i', 49, struct ifreq) [SIOCADDMULTI] */
/* #define SIOCZIFDATA _IOWR('i', 129, struct ifdatareq) [SIOCZIFDATA] */
/* #define SIOCGETSGCNT _IOWR('u', 52, struct sioc_sg_req) [SIOCGETSGCNT] */
/* #define SIOCALIFADDR _IOW('i', 28, struct if_laddrreq) [SIOCALIFADDR] */
/* #define SIOCGETVIFCNT _IOWR('u', 51, struct sioc_vif_req) [SIOCGETVIFCNT] */
/* #define SIOCIFGCLONERS _IOWR('i', 120, struct if_clonereq) [SIOCIFGCLONERS] */
/* #define SIOCDELRT _IOW('r', 11, struct ortentry) [SIOCDELRT] */
/* #define SIOCGIFDSTADDR _IOWR('i', 34, struct ifreq) [SIOCGIFDSTADDR] */
/* #define SIOCSIFFLAGS _IOW('i', 16, struct ifreq) [SIOCSIFFLAGS] */
/* #define SIOCGLOWAT _IOR('s', 3, int) [SIOCGLOWAT] */
/* #define SIOCSIFMTU _IOW('i', 127, struct ifreq) [SIOCSIFMTU] */
/* #define SIOCGIFMEDIA _IOWR('i', 54, struct ifmediareq) [SIOCGIFMEDIA] */
/* #define SIOCGIFCAP _IOWR('i', 118, struct ifcapreq) [SIOCGIFCAP] */
/* #define SIOCGIFNETMASK _IOWR('i', 37, struct ifreq) [SIOCGIFNETMASK] */
/* #define OSIOCGIFDSTADDR _IOWR('i', 15, struct ifreq) [OSIOCGIFDSTADDR] */
/* #define SIOCSLIFPHYADDR _IOW('i', 74, struct if_laddrreq) [SIOCSLIFPHYADDR] */
/* #define SIOCGIFBRDADDR _IOWR('i', 35, struct ifreq) [SIOCGIFBRDADDR] */
/* #define OSIOCGIFCONF _IOWR('i', 20, struct ifconf) [OSIOCGIFCONF] */
/* #define SIOCIFDESTROY _IOW('i', 121, struct ifreq) [SIOCIFDESTROY] */
/* #define SIOCGIFCONF _IOWR('i', 36, struct ifconf) [SIOCGIFCONF] */
/* #define OSIOCGIFNETMASK _IOWR('i', 21, struct ifreq) [OSIOCGIFNETMASK] */
/* #define SIOCADDRT _IOW('r', 10, struct ortentry) [SIOCADDRT] */
/* #define SIOCGIFDATA _IOWR('i', 128, struct ifdatareq) [SIOCGIFDATA] */
/* #define SIOCDELMULTI _IOW('i', 50, struct ifreq) [SIOCDELMULTI] */
/* #define OSIOCGIFBRDADDR _IOWR('i', 18, struct ifreq) [OSIOCGIFBRDADDR] */
/* #define SIOCSIFGENERIC _IOW('i', 57, struct ifreq) [SIOCSIFGENERIC] */
/* #define SIOCSDRVSPEC _IOW('i', 123, struct ifdrv) [SIOCSDRVSPEC] */
/* #define SIOCGHIWAT _IOR('s', 1, int) [SIOCGHIWAT] */
/* #define _IO(g,n) _IOC(IOC_VOID, (g), (n), 0) [_IO(g,n)] */
/* #define SIOCATMARK _IOR('s', 7, int) [SIOCATMARK] */
/* #define SIOCSLOWAT _IOW('s', 2, int) [SIOCSLOWAT] */
/* #define SIOCSIFADDR _IOW('i', 12, struct ifreq) [SIOCSIFADDR] */
/* #define SIOCSIFCAP _IOW('i', 117, struct ifcapreq) [SIOCSIFCAP] */
/* #define SIOCGIFFLAGS _IOWR('i', 17, struct ifreq) [SIOCGIFFLAGS] */
/* #define _IOC(inout,group,num,len) (inout | ((len & IOCPARM_MASK) << 16) | ((group) << 8) | (num)) [_IOC(inout,group,num,len)] */
/* #define SIOCSIFMETRIC _IOW('i', 24, struct ifreq) [SIOCSIFMETRIC] */
/* #define _IOR(g,n,t) _IOC(IOC_OUT, (g), (n), sizeof(t)) [_IOR(g,n,t)] */
/* #define _IOW(g,n,t) _IOC(IOC_IN, (g), (n), sizeof(t)) [_IOW(g,n,t)] */
/* #define SIOCGIFALIAS _IOWR('i', 27, struct ifaliasreq) [SIOCGIFALIAS] */
/* #define SIOCIFCREATE _IOW('i', 122, struct ifreq) [SIOCIFCREATE] */
/* #define SIOCSIFPHYADDR _IOW('i', 70, struct ifaliasreq) [SIOCSIFPHYADDR] */
/* #define TIOCREMOTE _IOW('t', 105, int) [TIOCREMOTE] */
/* #define DIOCSRETRIES _IOW('d', 108, int) [DIOCSRETRIES] */
/* #define FIONBIO _IOW('f', 126, int) [FIONBIO] */
/* #define DIOCLWEDGES _IOWR('d', 124, struct dkwedge_list) [DIOCLWEDGES] */
/* #define FIONWRITE _IOR('f', 121, int) [FIONWRITE] */
/* #define TIOCMSET _IOW('t', 109, int) [TIOCMSET] */
/* #define TIOCRCVFRAME _IOW('t', 69, struct mbuf *) [TIOCRCVFRAME] */
/* #define TIOCDRAIN _IO('t', 94) [TIOCDRAIN] */
/* #define TIOCEXCL _IO('t', 13) [TIOCEXCL] */
/* #define TIOCPKT_IOCTL 0x40 [TIOCPKT_IOCTL] */
/* #define SIOCGIFBRDADDR _IOWR('i', 35, struct ifreq) [SIOCGIFBRDADDR] */
/* #define FIOGETOWN _IOR('f', 123, int) [FIOGETOWN] */
/* #define DIOCGCACHE _IOR('d', 116, int) [DIOCGCACHE] */
/* #define TIOCUCNTL _IOW('t', 102, int) [TIOCUCNTL] */
/* #define SIOCGIFADDR _IOWR('i', 33, struct ifreq) [SIOCGIFADDR] */
/* #define SIOCGIFMETRIC _IOWR('i', 23, struct ifreq) [SIOCGIFMETRIC] */
/* #define OSIOCGIFADDR _IOWR('i', 13, struct ifreq) [OSIOCGIFADDR] */
/* #define SIOCGIFGENERIC _IOWR('i', 58, struct ifreq) [SIOCGIFGENERIC] */
/* #define SIOCGIFPSRCADDR _IOWR('i', 71, struct ifreq) [SIOCGIFPSRCADDR] */
/* #define SIOCSIFDSTADDR _IOW('i', 14, struct ifreq) [SIOCSIFDSTADDR] */
/* #define SIOCGIFPDSTADDR _IOWR('i', 72, struct ifreq) [SIOCGIFPDSTADDR] */
/* #define DIOCGDEFLABEL _IOR('d', 114, struct disklabel) [DIOCGDEFLABEL] */
/* #define FIOASYNC _IOW('f', 125, int) [FIOASYNC] */
/* #define SIOCSHIWAT _IOW('s', 0, int) [SIOCSHIWAT] */
/* #define SIOCSIFMEDIA _IOWR('i', 53, struct ifreq) [SIOCSIFMEDIA] */
/* #define TIOCSBRK _IO('t', 123) [TIOCSBRK] */
/* #define DIOCSSTEP _IOW('d', 107, int) [DIOCSSTEP] */
/* #define SIOCDIFPHYADDR _IOW('i', 73, struct ifreq) [SIOCDIFPHYADDR] */
/* #define SIOCGLIFADDR _IOWR('i', 29, struct if_laddrreq) [SIOCGLIFADDR] */
/* #define SIOCGDRVSPEC _IOWR('i', 123, struct ifdrv) [SIOCGDRVSPEC] */
/* #define SIOCDIFADDR _IOW('i', 25, struct ifreq) [SIOCDIFADDR] */
/* #define TIOCGRANTPT _IO('t', 71) [TIOCGRANTPT] */
/* #define SIOCGIFDLT _IOWR('i', 119, struct ifreq) [SIOCGIFDLT] */
/* #define TIOCSIG _IO('t', 95) [TIOCSIG] */
/* #define PTIOCNETBSD _IOW('Z', 0, struct ioctl_pt) [PTIOCNETBSD] */
/* #define _IOWR(g,n,t) _IOC(IOC_INOUT, (g), (n), sizeof(t)) [_IOWR(g,n,t)] */
/* #define TIOCGETA _IOR('t', 19, struct termios) [TIOCGETA] */
/* #define TIOCGETD _IOR('t', 26, int) [TIOCGETD] */
/* #define PTIOCULTRIX _IOW('Z', 6, struct ioctl_pt) [PTIOCULTRIX] */
/* #define SIOCSIFNETMASK _IOW('i', 22, struct ifreq) [SIOCSIFNETMASK] */
/* #define SIOCAIFADDR _IOW('i', 26, struct ifaliasreq) [SIOCAIFADDR] */
/* #define DIOCCACHESYNC _IOW('d', 118, int) [DIOCCACHESYNC] */
/* #define TIOCSETD _IOW('t', 27, int) [TIOCSETD] */
/* #define TIOCSDTR _IO('t', 121) [TIOCSDTR] */
/* #define SIOCGPGRP _IOR('s', 9, int) [SIOCGPGRP] */
/* #define SIOCDLIFADDR _IOW('i', 30, struct if_laddrreq) [SIOCDLIFADDR] */
/* #define OSIOCGIFCONF _IOWR('i', 20, struct ifconf) [OSIOCGIFCONF] */
/* #define DIOCGWEDGEINFO _IOR('d', 122, struct dkwedge_info) [DIOCGWEDGEINFO] */
/* #define TIOCXMTFRAME _IOW('t', 68, struct mbuf *) [TIOCXMTFRAME] */
/* #define TIOCSFLAGS _IOW('t', 92, int) [TIOCSFLAGS] */
/* #define DIOCWLABEL _IOW('d', 109, int) [DIOCWLABEL] */
/* #define TIOCSTI _IOW('t', 114, char) [TIOCSTI] */
/* #define SIOCSIFBRDADDR _IOW('i', 19, struct ifreq) [SIOCSIFBRDADDR] */
/* #define SIOCGLIFPHYADDR _IOWR('i', 75, struct if_laddrreq) [SIOCGLIFPHYADDR] */
/* #define DIOCWFORMAT _IOWR('d', 106, struct format_op) [DIOCWFORMAT] */
/* #define SIOCSPGRP _IOW('s', 8, int) [SIOCSPGRP] */
/* #define TIOCPTSNAME _IOR('t', 72, struct ptmget) [TIOCPTSNAME] */
/* #define SIOCADDMULTI _IOW('i', 49, struct ifreq) [SIOCADDMULTI] */
/* #define SIOCZIFDATA _IOWR('i', 129, struct ifdatareq) [SIOCZIFDATA] */
/* #define SIOCGETSGCNT _IOWR('u', 52, struct sioc_sg_req) [SIOCGETSGCNT] */
/* #define SIOCALIFADDR _IOW('i', 28, struct if_laddrreq) [SIOCALIFADDR] */
/* #define TIOCSLINED _IOW('t', 67, linedn_t) [TIOCSLINED] */
/* #define SIOCGETVIFCNT _IOWR('u', 51, struct sioc_vif_req) [SIOCGETVIFCNT] */
/* #define TIOCGLINED _IOR('t', 66, linedn_t) [TIOCGLINED] */
/* #define PTIOCSUNOS _IOW('Z', 1, struct ioctl_pt) [PTIOCSUNOS] */
/* #define TIOCSTART _IO('t', 110) [TIOCSTART] */
/* #define DIOCAWEDGE _IOWR('d', 121, struct dkwedge_info) [DIOCAWEDGE] */
/* #define SIOCIFGCLONERS _IOWR('i', 120, struct if_clonereq) [SIOCIFGCLONERS] */
/* #define SIOCDELRT _IOW('r', 11, struct ortentry) [SIOCDELRT] */
/* #define FIOSETOWN _IOW('f', 124, int) [FIOSETOWN] */
/* #define DIOCSCACHE _IOW('d', 117, int) [DIOCSCACHE] */
/* #define DIOCRFORMAT _IOWR('d', 105, struct format_op) [DIOCRFORMAT] */
/* #define SIOCGIFMTU _IOWR('i', 126, struct ifreq) [SIOCGIFMTU] */
/* #define SIOCGIFDSTADDR _IOWR('i', 34, struct ifreq) [SIOCGIFDSTADDR] */
/* #define TIOCSTAT _IOW('t', 101, int) [TIOCSTAT] */
/* #define TIOCGSID _IOR('t', 99, int) [TIOCGSID] */
/* #define SIOCGHIWAT _IOR('s', 1, int) [SIOCGHIWAT] */
/* #define DIOCGPART _IOW('d', 104, struct partinfo) [DIOCGPART] */
/* #define TIOCMGET _IOR('t', 106, int) [TIOCMGET] */
/* #define TIOCNXCL _IO('t', 14) [TIOCNXCL] */
/* #define SIOCSIFFLAGS _IOW('i', 16, struct ifreq) [SIOCSIFFLAGS] */
/* #define SIOCGLOWAT _IOR('s', 3, int) [SIOCGLOWAT] */
/* #define SIOCSIFMTU _IOW('i', 127, struct ifreq) [SIOCSIFMTU] */
/* #define SIOCGIFMEDIA _IOWR('i', 54, struct ifmediareq) [SIOCGIFMEDIA] */
/* #define TIOCFLUSH _IOW('t', 16, int) [TIOCFLUSH] */
/* #define DIOCEJECT _IOW('d', 112, int) [DIOCEJECT] */
/* #define TIOCGWINSZ _IOR('t', 104, struct winsize) [TIOCGWINSZ] */
/* #define SIOCGIFCAP _IOWR('i', 118, struct ifcapreq) [SIOCGIFCAP] */
/* #define TIOCGPGRP _IOR('t', 119, int) [TIOCGPGRP] */
/* #define SIOCGIFNETMASK _IOWR('i', 37, struct ifreq) [SIOCGIFNETMASK] */
/* #define TIOCPKT _IOW('t', 112, int) [TIOCPKT] */
/* #define OSIOCGIFDSTADDR _IOWR('i', 15, struct ifreq) [OSIOCGIFDSTADDR] */
/* #define DIOCBSFLUSH _IO('d', 120) [DIOCBSFLUSH] */
/* #define FIONCLEX _IO('f', 2) [FIONCLEX] */
/* #define SIOCSLIFPHYADDR _IOW('i', 74, struct if_laddrreq) [SIOCSLIFPHYADDR] */
/* #define TIOCCDTR _IO('t', 120) [TIOCCDTR] */
/* #define FIOCLEX _IO('f', 1) [FIOCLEX] */
/* #define TIOCPTMGET _IOR('t', 70, struct ptmget) [TIOCPTMGET] */
/* #define TIOCEXT _IOW('t', 96, int) [TIOCEXT] */
/* #define SIOCIFDESTROY _IOW('i', 121, struct ifreq) [SIOCIFDESTROY] */
/* #define DIOCGDINFO _IOR('d', 101, struct disklabel) [DIOCGDINFO] */
/* #define SIOCGIFCONF _IOWR('i', 36, struct ifconf) [SIOCGIFCONF] */
/* #define OFIOGETBMAP _IOWR('f', 122, uint32_t) [OFIOGETBMAP] */
/* #define TIOCSCTTY _IO('t', 97) [TIOCSCTTY] */
/* #define FIONSPACE _IOR('f', 120, int) [FIONSPACE] */
/* #define DIOCCLRLABEL _IO('d', 115) [DIOCCLRLABEL] */
/* #define FIOGETBMAP _IOWR('f', 122, daddr_t) [FIOGETBMAP] */
/* #define TIOCOUTQ _IOR('t', 115, int) [TIOCOUTQ] */
/* #define TIOCCBRK _IO('t', 122) [TIOCCBRK] */
/* #define OSIOCGIFNETMASK _IOWR('i', 21, struct ifreq) [OSIOCGIFNETMASK] */
/* #define SIOCADDRT _IOW('r', 10, struct ortentry) [SIOCADDRT] */
/* #define SIOCGIFDATA _IOWR('i', 128, struct ifdatareq) [SIOCGIFDATA] */
/* #define DIOCWDINFO _IOW('d', 103, struct disklabel) [DIOCWDINFO] */
/* #define SIOCDELMULTI _IOW('i', 50, struct ifreq) [SIOCDELMULTI] */
/* #define OSIOCGIFBRDADDR _IOWR('i', 18, struct ifreq) [OSIOCGIFBRDADDR] */
/* #define TIOCMBIC _IOW('t', 107, int) [TIOCMBIC] */
/* #define TIOCMBIS _IOW('t', 108, int) [TIOCMBIS] */
/* #define SIOCSIFGENERIC _IOW('i', 57, struct ifreq) [SIOCSIFGENERIC] */
/* #define SIOCSDRVSPEC _IOW('i', 123, struct ifdrv) [SIOCSDRVSPEC] */
/* #define TIOCSETA _IOW('t', 20, struct termios) [TIOCSETA] */
/* #define TIOCCONS _IOW('t', 98, int) [TIOCCONS] */
/* #define TIOCSPGRP _IOW('t', 118, int) [TIOCSPGRP] */
/* #define FIONREAD _IOR('f', 127, int) [FIONREAD] */
/* #define _IO(g,n) _IOC(IOC_VOID, (g), (n), 0) [_IO(g,n)] */
/* #define SIOCATMARK _IOR('s', 7, int) [SIOCATMARK] */
/* #define SIOCSLOWAT _IOW('s', 2, int) [SIOCSLOWAT] */
/* #define PTIOCLINUX _IOW('Z', 3, struct ioctl_pt) [PTIOCLINUX] */
/* #define DIOCSBAD _IOW('d', 110, struct dkbad) [DIOCSBAD] */
/* #define SIOCSIFADDR _IOW('i', 12, struct ifreq) [SIOCSIFADDR] */
/* #define TIOCSWINSZ _IOW('t', 103, struct winsize) [TIOCSWINSZ] */
/* #define SIOCSIFCAP _IOW('i', 117, struct ifcapreq) [SIOCSIFCAP] */
/* #define SIOCGIFFLAGS _IOWR('i', 17, struct ifreq) [SIOCGIFFLAGS] */
/* #define TIOCSETAF _IOW('t', 22, struct termios) [TIOCSETAF] */
/* #define TIOCSETAW _IOW('t', 21, struct termios) [TIOCSETAW] */
/* #define _IOC(inout,group,num,len) (inout | ((len & IOCPARM_MASK) << 16) | ((group) << 8) | (num)) [_IOC(inout,group,num,len)] */
/* #define SIOCSIFMETRIC _IOW('i', 24, struct ifreq) [SIOCSIFMETRIC] */
/* #define _IOR(g,n,t) _IOC(IOC_OUT, (g), (n), sizeof(t)) [_IOR(g,n,t)] */
/* #define _IOW(g,n,t) _IOC(IOC_IN, (g), (n), sizeof(t)) [_IOW(g,n,t)] */
/* #define ODIOCEJECT _IO('d', 112) [ODIOCEJECT] */
/* #define SIOCGIFALIAS _IOWR('i', 27, struct ifaliasreq) [SIOCGIFALIAS] */
/* #define TIOCGFLAGS _IOR('t', 93, int) [TIOCGFLAGS] */
/* #define DIOCKLABEL _IOW('d', 119, int) [DIOCKLABEL] */
/* #define DIOCDWEDGE _IOW('d', 123, struct dkwedge_info) [DIOCDWEDGE] */
/* #define TIOCNOTTY _IO('t', 113) [TIOCNOTTY] */
/* #define TIOCSTOP _IO('t', 111) [TIOCSTOP] */
/* #define PTIOCFREEBSD _IOW('Z', 4, struct ioctl_pt) [PTIOCFREEBSD] */
/* #define DIOCSDINFO _IOW('d', 102, struct disklabel) [DIOCSDINFO] */
/* #define TIOCDCDTIMESTAMP _IOR('t', 88, struct timeval) [TIOCDCDTIMESTAMP] */
/* #define SIOCIFCREATE _IOW('i', 122, struct ifreq) [SIOCIFCREATE] */
/* #define DIOCLOCK _IOW('d', 113, int) [DIOCLOCK] */
/* #define SIOCSIFPHYADDR _IOW('i', 70, struct ifaliasreq) [SIOCSIFPHYADDR] */
/* #define DIOCBSLIST _IOWR('d', 119, struct disk_badsecinfo) [DIOCBSLIST] */
/* #define DIOCSRETRIES _IOW('d', 108, int) [DIOCSRETRIES] */
/* #define FIONBIO _IOW('f', 126, int) [FIONBIO] */
/* #define DIOCLWEDGES _IOWR('d', 124, struct dkwedge_list) [DIOCLWEDGES] */
/* #define TIOCGLINED _IOR('t', 66, linedn_t) [TIOCGLINED] */
/* #define FIONWRITE _IOR('f', 121, int) [FIONWRITE] */
/* #define TIOCMSET _IOW('t', 109, int) [TIOCMSET] */
/* #define TIOCRCVFRAME _IOW('t', 69, struct mbuf *) [TIOCRCVFRAME] */
/* #define TIOCDRAIN _IO('t', 94) [TIOCDRAIN] */
/* #define TIOCEXCL _IO('t', 13) [TIOCEXCL] */
/* #define TIOCPKT_IOCTL 0x40 [TIOCPKT_IOCTL] */
/* #define FIOGETOWN _IOR('f', 123, int) [FIOGETOWN] */
/* #define SIOCGPGRP _IOR('s', 9, int) [SIOCGPGRP] */
/* #define DIOCGCACHE _IOR('d', 116, int) [DIOCGCACHE] */
/* #define TIOCUCNTL _IOW('t', 102, int) [TIOCUCNTL] */
/* #define SIOCGIFADDR _IOWR('i', 33, struct ifreq) [SIOCGIFADDR] */
/* #define SIOCGIFMETRIC _IOWR('i', 23, struct ifreq) [SIOCGIFMETRIC] */
/* #define OSIOCGIFADDR _IOWR('i', 13, struct ifreq) [OSIOCGIFADDR] */
/* #define BIOSMODE _IOW('b', 1, int) [BIOSMODE] */
	case BIOSMODE:
		return "BIOSMODE";
/* #define SIOCGIFGENERIC _IOWR('i', 58, struct ifreq) [SIOCGIFGENERIC] */
/* #define SIOCGIFPSRCADDR _IOWR('i', 71, struct ifreq) [SIOCGIFPSRCADDR] */
/* #define SIOCSIFDSTADDR _IOW('i', 14, struct ifreq) [SIOCSIFDSTADDR] */
/* #define SIOCGIFPDSTADDR _IOWR('i', 72, struct ifreq) [SIOCGIFPDSTADDR] */
/* #define DIOCGDEFLABEL _IOR('d', 114, struct disklabel) [DIOCGDEFLABEL] */
/* #define FIOASYNC _IOW('f', 125, int) [FIOASYNC] */
/* #define SIOCSHIWAT _IOW('s', 0, int) [SIOCSHIWAT] */
/* #define SIOCSIFMEDIA _IOWR('i', 53, struct ifreq) [SIOCSIFMEDIA] */
/* #define TIOCSBRK _IO('t', 123) [TIOCSBRK] */
/* #define DIOCSSTEP _IOW('d', 107, int) [DIOCSSTEP] */
/* #define SIOCDIFPHYADDR _IOW('i', 73, struct ifreq) [SIOCDIFPHYADDR] */
/* #define SIOCGLIFADDR _IOWR('i', 29, struct if_laddrreq) [SIOCGLIFADDR] */
/* #define SIOCGDRVSPEC _IOWR('i', 123, struct ifdrv) [SIOCGDRVSPEC] */
/* #define SIOCDIFADDR _IOW('i', 25, struct ifreq) [SIOCDIFADDR] */
/* #define TIOCGRANTPT _IO('t', 71) [TIOCGRANTPT] */
/* #define SIOCGIFMTU _IOWR('i', 126, struct ifreq) [SIOCGIFMTU] */
/* #define SIOCGIFDLT _IOWR('i', 119, struct ifreq) [SIOCGIFDLT] */
/* #define TIOCSIG _IO('t', 95) [TIOCSIG] */
/* #define PTIOCNETBSD _IOW('Z', 0, struct ioctl_pt) [PTIOCNETBSD] */
/* #define _IOWR(g,n,t) _IOC(IOC_INOUT, (g), (n), sizeof(t)) [_IOWR(g,n,t)] */
/* #define TIOCGETA _IOR('t', 19, struct termios) [TIOCGETA] */
/* #define TIOCGETD _IOR('t', 26, int) [TIOCGETD] */
/* #define PTIOCULTRIX _IOW('Z', 6, struct ioctl_pt) [PTIOCULTRIX] */
/* #define SIOCSIFNETMASK _IOW('i', 22, struct ifreq) [SIOCSIFNETMASK] */
/* #define SIOCAIFADDR _IOW('i', 26, struct ifaliasreq) [SIOCAIFADDR] */
/* #define DIOCCACHESYNC _IOW('d', 118, int) [DIOCCACHESYNC] */
/* #define TIOCSDTR _IO('t', 121) [TIOCSDTR] */
/* #define SIOCDLIFADDR _IOW('i', 30, struct if_laddrreq) [SIOCDLIFADDR] */
/* #define DIOCGWEDGEINFO _IOR('d', 122, struct dkwedge_info) [DIOCGWEDGEINFO] */
/* #define TIOCXMTFRAME _IOW('t', 68, struct mbuf *) [TIOCXMTFRAME] */
/* #define TIOCSFLAGS _IOW('t', 92, int) [TIOCSFLAGS] */
/* #define DIOCWLABEL _IOW('d', 109, int) [DIOCWLABEL] */
/* #define TIOCSTI _IOW('t', 114, char) [TIOCSTI] */
/* #define SIOCSIFBRDADDR _IOW('i', 19, struct ifreq) [SIOCSIFBRDADDR] */
/* #define SIOCGLIFPHYADDR _IOWR('i', 75, struct if_laddrreq) [SIOCGLIFPHYADDR] */
/* #define DIOCWFORMAT _IOWR('d', 106, struct format_op) [DIOCWFORMAT] */
/* #define SIOCSPGRP _IOW('s', 8, int) [SIOCSPGRP] */
/* #define TIOCPTSNAME _IOR('t', 72, struct ptmget) [TIOCPTSNAME] */
/* #define PTIOCSUNOS _IOW('Z', 1, struct ioctl_pt) [PTIOCSUNOS] */
/* #define SIOCADDMULTI _IOW('i', 49, struct ifreq) [SIOCADDMULTI] */
/* #define SIOCZIFDATA _IOWR('i', 129, struct ifdatareq) [SIOCZIFDATA] */
/* #define SIOCGETSGCNT _IOWR('u', 52, struct sioc_sg_req) [SIOCGETSGCNT] */
/* #define SIOCALIFADDR _IOW('i', 28, struct if_laddrreq) [SIOCALIFADDR] */
/* #define TIOCSLINED _IOW('t', 67, linedn_t) [TIOCSLINED] */
/* #define SIOCGETVIFCNT _IOWR('u', 51, struct sioc_vif_req) [SIOCGETVIFCNT] */
/* #define TIOCSTART _IO('t', 110) [TIOCSTART] */
/* #define DIOCAWEDGE _IOWR('d', 121, struct dkwedge_info) [DIOCAWEDGE] */
/* #define SIOCIFGCLONERS _IOWR('i', 120, struct if_clonereq) [SIOCIFGCLONERS] */
/* #define SIOCDELRT _IOW('r', 11, struct ortentry) [SIOCDELRT] */
/* #define FIOSETOWN _IOW('f', 124, int) [FIOSETOWN] */
/* #define DIOCSCACHE _IOW('d', 117, int) [DIOCSCACHE] */
/* #define DIOCRFORMAT _IOWR('d', 105, struct format_op) [DIOCRFORMAT] */
/* #define SIOCGIFDSTADDR _IOWR('i', 34, struct ifreq) [SIOCGIFDSTADDR] */
/* #define BIOGTYPE _IOR('b', 4, int) [BIOGTYPE] */
	case BIOGTYPE:
		return "BIOGTYPE";
/* #define FIONREAD _IOR('f', 127, int) [FIONREAD] */
/* #define TIOCSTAT _IOW('t', 101, int) [TIOCSTAT] */
/* #define SIOCGHIWAT _IOR('s', 1, int) [SIOCGHIWAT] */
/* #define DIOCGPART _IOW('d', 104, struct partinfo) [DIOCGPART] */
/* #define TIOCMGET _IOR('t', 106, int) [TIOCMGET] */
/* #define TIOCNXCL _IO('t', 14) [TIOCNXCL] */
/* #define SIOCSIFFLAGS _IOW('i', 16, struct ifreq) [SIOCSIFFLAGS] */
/* #define SIOCGLOWAT _IOR('s', 3, int) [SIOCGLOWAT] */
/* #define SIOCSIFMTU _IOW('i', 127, struct ifreq) [SIOCSIFMTU] */
/* #define SIOCGIFMEDIA _IOWR('i', 54, struct ifmediareq) [SIOCGIFMEDIA] */
/* #define TIOCFLUSH _IOW('t', 16, int) [TIOCFLUSH] */
/* #define DIOCEJECT _IOW('d', 112, int) [DIOCEJECT] */
/* #define TIOCGWINSZ _IOR('t', 104, struct winsize) [TIOCGWINSZ] */
/* #define SIOCGIFCAP _IOWR('i', 118, struct ifcapreq) [SIOCGIFCAP] */
/* #define TIOCGPGRP _IOR('t', 119, int) [TIOCGPGRP] */
/* #define SIOCGIFNETMASK _IOWR('i', 37, struct ifreq) [SIOCGIFNETMASK] */
/* #define TIOCPKT _IOW('t', 112, int) [TIOCPKT] */
/* #define OSIOCGIFDSTADDR _IOWR('i', 15, struct ifreq) [OSIOCGIFDSTADDR] */
/* #define DIOCBSFLUSH _IO('d', 120) [DIOCBSFLUSH] */
/* #define FIONCLEX _IO('f', 2) [FIONCLEX] */
/* #define SIOCSLIFPHYADDR _IOW('i', 74, struct if_laddrreq) [SIOCSLIFPHYADDR] */
/* #define FIOCLEX _IO('f', 1) [FIOCLEX] */
/* #define SIOCGIFBRDADDR _IOWR('i', 35, struct ifreq) [SIOCGIFBRDADDR] */
/* #define OSIOCGIFCONF _IOWR('i', 20, struct ifconf) [OSIOCGIFCONF] */
/* #define TIOCPTMGET _IOR('t', 70, struct ptmget) [TIOCPTMGET] */
/* #define TIOCEXT _IOW('t', 96, int) [TIOCEXT] */
/* #define SIOCIFDESTROY _IOW('i', 121, struct ifreq) [SIOCIFDESTROY] */
/* #define DIOCGDINFO _IOR('d', 101, struct disklabel) [DIOCGDINFO] */
/* #define SIOCGIFCONF _IOWR('i', 36, struct ifconf) [SIOCGIFCONF] */
/* #define OFIOGETBMAP _IOWR('f', 122, uint32_t) [OFIOGETBMAP] */
/* #define TIOCSCTTY _IO('t', 97) [TIOCSCTTY] */
/* #define BIOSTYPE _IOW('b', 3, int) [BIOSTYPE] */
	case BIOSTYPE:
		return "BIOSTYPE";
/* #define FIONSPACE _IOR('f', 120, int) [FIONSPACE] */
/* #define DIOCCLRLABEL _IO('d', 115) [DIOCCLRLABEL] */
/* #define FIOGETBMAP _IOWR('f', 122, daddr_t) [FIOGETBMAP] */
/* #define TIOCOUTQ _IOR('t', 115, int) [TIOCOUTQ] */
/* #define TIOCCBRK _IO('t', 122) [TIOCCBRK] */
/* #define OSIOCGIFNETMASK _IOWR('i', 21, struct ifreq) [OSIOCGIFNETMASK] */
/* #define TIOCREMOTE _IOW('t', 105, int) [TIOCREMOTE] */
/* #define SIOCADDRT _IOW('r', 10, struct ortentry) [SIOCADDRT] */
/* #define SIOCGIFDATA _IOWR('i', 128, struct ifdatareq) [SIOCGIFDATA] */
/* #define DIOCWDINFO _IOW('d', 103, struct disklabel) [DIOCWDINFO] */
/* #define SIOCDELMULTI _IOW('i', 50, struct ifreq) [SIOCDELMULTI] */
/* #define OSIOCGIFBRDADDR _IOWR('i', 18, struct ifreq) [OSIOCGIFBRDADDR] */
/* #define TIOCMBIC _IOW('t', 107, int) [TIOCMBIC] */
/* #define TIOCMBIS _IOW('t', 108, int) [TIOCMBIS] */
/* #define SIOCSIFGENERIC _IOW('i', 57, struct ifreq) [SIOCSIFGENERIC] */
/* #define SIOCSDRVSPEC _IOW('i', 123, struct ifdrv) [SIOCSDRVSPEC] */
/* #define TIOCSETA _IOW('t', 20, struct termios) [TIOCSETA] */
/* #define TIOCSETD _IOW('t', 27, int) [TIOCSETD] */
/* #define TIOCCONS _IOW('t', 98, int) [TIOCCONS] */
/* #define TIOCSPGRP _IOW('t', 118, int) [TIOCSPGRP] */
/* #define TIOCGSID _IOR('t', 99, int) [TIOCGSID] */
/* #define _IO(g,n) _IOC(IOC_VOID, (g), (n), 0) [_IO(g,n)] */
/* #define TIOCCDTR _IO('t', 120) [TIOCCDTR] */
/* #define SIOCATMARK _IOR('s', 7, int) [SIOCATMARK] */
/* #define SIOCSLOWAT _IOW('s', 2, int) [SIOCSLOWAT] */
/* #define PTIOCLINUX _IOW('Z', 3, struct ioctl_pt) [PTIOCLINUX] */
/* #define DIOCSBAD _IOW('d', 110, struct dkbad) [DIOCSBAD] */
/* #define BIOGMODE _IOR('b', 2, int) [BIOGMODE] */
	case BIOGMODE:
		return "BIOGMODE";
/* #define SIOCSIFADDR _IOW('i', 12, struct ifreq) [SIOCSIFADDR] */
/* #define TIOCSWINSZ _IOW('t', 103, struct winsize) [TIOCSWINSZ] */
/* #define SIOCSIFCAP _IOW('i', 117, struct ifcapreq) [SIOCSIFCAP] */
/* #define SIOCGIFFLAGS _IOWR('i', 17, struct ifreq) [SIOCGIFFLAGS] */
/* #define TIOCSETAF _IOW('t', 22, struct termios) [TIOCSETAF] */
/* #define TIOCSETAW _IOW('t', 21, struct termios) [TIOCSETAW] */
/* #define _IOC(inout,group,num,len) (inout | ((len & IOCPARM_MASK) << 16) | ((group) << 8) | (num)) [_IOC(inout,group,num,len)] */
/* #define SIOCSIFMETRIC _IOW('i', 24, struct ifreq) [SIOCSIFMETRIC] */
/* #define _IOR(g,n,t) _IOC(IOC_OUT, (g), (n), sizeof(t)) [_IOR(g,n,t)] */
/* #define _IOW(g,n,t) _IOC(IOC_IN, (g), (n), sizeof(t)) [_IOW(g,n,t)] */
/* #define ODIOCEJECT _IO('d', 112) [ODIOCEJECT] */
/* #define SIOCGIFALIAS _IOWR('i', 27, struct ifaliasreq) [SIOCGIFALIAS] */
/* #define TIOCGFLAGS _IOR('t', 93, int) [TIOCGFLAGS] */
/* #define DIOCKLABEL _IOW('d', 119, int) [DIOCKLABEL] */
/* #define DIOCDWEDGE _IOW('d', 123, struct dkwedge_info) [DIOCDWEDGE] */
/* #define TIOCNOTTY _IO('t', 113) [TIOCNOTTY] */
/* #define TIOCSTOP _IO('t', 111) [TIOCSTOP] */
/* #define PTIOCFREEBSD _IOW('Z', 4, struct ioctl_pt) [PTIOCFREEBSD] */
/* #define DIOCSDINFO _IOW('d', 102, struct disklabel) [DIOCSDINFO] */
/* #define TIOCDCDTIMESTAMP _IOR('t', 88, struct timeval) [TIOCDCDTIMESTAMP] */
/* #define SIOCIFCREATE _IOW('i', 122, struct ifreq) [SIOCIFCREATE] */
/* #define DIOCLOCK _IOW('d', 113, int) [DIOCLOCK] */
/* #define SIOCSIFPHYADDR _IOW('i', 70, struct ifaliasreq) [SIOCSIFPHYADDR] */
/* #define DIOCBSLIST _IOWR('d', 119, struct disk_badsecinfo) [DIOCBSLIST] */
/* #define DIOCSRETRIES _IOW('d', 108, int) [DIOCSRETRIES] */
/* #define FIONBIO _IOW('f', 126, int) [FIONBIO] */
/* #define DIOCLWEDGES _IOWR('d', 124, struct dkwedge_list) [DIOCLWEDGES] */
/* #define TIOCGLINED _IOR('t', 66, linedn_t) [TIOCGLINED] */
/* #define FIONWRITE _IOR('f', 121, int) [FIONWRITE] */
/* #define TIOCMSET _IOW('t', 109, int) [TIOCMSET] */
/* #define TIOCRCVFRAME _IOW('t', 69, struct mbuf *) [TIOCRCVFRAME] */
/* #define TIOCDRAIN _IO('t', 94) [TIOCDRAIN] */
/* #define TIOCEXCL _IO('t', 13) [TIOCEXCL] */
/* #define TIOCPKT_IOCTL 0x40 [TIOCPKT_IOCTL] */
/* #define FIOGETOWN _IOR('f', 123, int) [FIOGETOWN] */
/* #define SIOCGPGRP _IOR('s', 9, int) [SIOCGPGRP] */
/* #define DIOCGCACHE _IOR('d', 116, int) [DIOCGCACHE] */
/* #define TIOCUCNTL _IOW('t', 102, int) [TIOCUCNTL] */
/* #define SIOCGIFADDR _IOWR('i', 33, struct ifreq) [SIOCGIFADDR] */
/* #define SIOCGIFMETRIC _IOWR('i', 23, struct ifreq) [SIOCGIFMETRIC] */
/* #define OSIOCGIFADDR _IOWR('i', 13, struct ifreq) [OSIOCGIFADDR] */
/* #define SIOCGIFGENERIC _IOWR('i', 58, struct ifreq) [SIOCGIFGENERIC] */
/* #define SIOCGIFPSRCADDR _IOWR('i', 71, struct ifreq) [SIOCGIFPSRCADDR] */
/* #define SIOCSIFDSTADDR _IOW('i', 14, struct ifreq) [SIOCSIFDSTADDR] */
/* #define SIOCGIFPDSTADDR _IOWR('i', 72, struct ifreq) [SIOCGIFPDSTADDR] */
/* #define DIOCGDEFLABEL _IOR('d', 114, struct disklabel) [DIOCGDEFLABEL] */
/* #define FIOASYNC _IOW('f', 125, int) [FIOASYNC] */
/* #define SIOCSHIWAT _IOW('s', 0, int) [SIOCSHIWAT] */
/* #define SIOCSIFMEDIA _IOWR('i', 53, struct ifreq) [SIOCSIFMEDIA] */
/* #define TIOCSBRK _IO('t', 123) [TIOCSBRK] */
/* #define DIOCSSTEP _IOW('d', 107, int) [DIOCSSTEP] */
/* #define SIOCDIFPHYADDR _IOW('i', 73, struct ifreq) [SIOCDIFPHYADDR] */
/* #define SIOCGLIFADDR _IOWR('i', 29, struct if_laddrreq) [SIOCGLIFADDR] */
/* #define SIOCGDRVSPEC _IOWR('i', 123, struct ifdrv) [SIOCGDRVSPEC] */
/* #define SIOCDIFADDR _IOW('i', 25, struct ifreq) [SIOCDIFADDR] */
/* #define TIOCGRANTPT _IO('t', 71) [TIOCGRANTPT] */
/* #define SIOCGIFMTU _IOWR('i', 126, struct ifreq) [SIOCGIFMTU] */
/* #define SIOCGIFDLT _IOWR('i', 119, struct ifreq) [SIOCGIFDLT] */
/* #define TIOCSIG _IO('t', 95) [TIOCSIG] */
/* #define _IOWR(g,n,t) _IOC(IOC_INOUT, (g), (n), sizeof(t)) [_IOWR(g,n,t)] */
/* #define TIOCGETA _IOR('t', 19, struct termios) [TIOCGETA] */
/* #define TIOCGETD _IOR('t', 26, int) [TIOCGETD] */
/* #define PTIOCULTRIX _IOW('Z', 6, struct ioctl_pt) [PTIOCULTRIX] */
/* #define SIOCSIFNETMASK _IOW('i', 22, struct ifreq) [SIOCSIFNETMASK] */
/* #define SIOCAIFADDR _IOW('i', 26, struct ifaliasreq) [SIOCAIFADDR] */
/* #define DIOCCACHESYNC _IOW('d', 118, int) [DIOCCACHESYNC] */
/* #define TIOCSETD _IOW('t', 27, int) [TIOCSETD] */
/* #define TIOCSDTR _IO('t', 121) [TIOCSDTR] */
/* #define SIOCDLIFADDR _IOW('i', 30, struct if_laddrreq) [SIOCDLIFADDR] */
/* #define OSIOCGIFCONF _IOWR('i', 20, struct ifconf) [OSIOCGIFCONF] */
/* #define PPS_IOC_DESTROY _IO('1', 2) [PPS_IOC_DESTROY] */
	case PPS_IOC_DESTROY:
		return "PPS_IOC_DESTROY";
/* #define DIOCGWEDGEINFO _IOR('d', 122, struct dkwedge_info) [DIOCGWEDGEINFO] */
/* #define TIOCXMTFRAME _IOW('t', 68, struct mbuf *) [TIOCXMTFRAME] */
/* #define TIOCSFLAGS _IOW('t', 92, int) [TIOCSFLAGS] */
/* #define DIOCWLABEL _IOW('d', 109, int) [DIOCWLABEL] */
/* #define TIOCSTI _IOW('t', 114, char) [TIOCSTI] */
/* #define SIOCSIFBRDADDR _IOW('i', 19, struct ifreq) [SIOCSIFBRDADDR] */
/* #define SIOCGLIFPHYADDR _IOWR('i', 75, struct if_laddrreq) [SIOCGLIFPHYADDR] */
/* #define DIOCWFORMAT _IOWR('d', 106, struct format_op) [DIOCWFORMAT] */
/* #define SIOCSPGRP _IOW('s', 8, int) [SIOCSPGRP] */
/* #define TIOCPTSNAME _IOR('t', 72, struct ptmget) [TIOCPTSNAME] */
/* #define SIOCADDMULTI _IOW('i', 49, struct ifreq) [SIOCADDMULTI] */
/* #define PPS_IOC_KCBIND _IOW('1', 7, int) [PPS_IOC_KCBIND] */
	case PPS_IOC_KCBIND:
		return "PPS_IOC_KCBIND";
/* #define SIOCZIFDATA _IOWR('i', 129, struct ifdatareq) [SIOCZIFDATA] */
/* #define SIOCGETSGCNT _IOWR('u', 52, struct sioc_sg_req) [SIOCGETSGCNT] */
/* #define SIOCALIFADDR _IOW('i', 28, struct if_laddrreq) [SIOCALIFADDR] */
/* #define TIOCSLINED _IOW('t', 67, linedn_t) [TIOCSLINED] */
/* #define SIOCGETVIFCNT _IOWR('u', 51, struct sioc_vif_req) [SIOCGETVIFCNT] */
/* #define PTIOCSUNOS _IOW('Z', 1, struct ioctl_pt) [PTIOCSUNOS] */
/* #define TIOCSTART _IO('t', 110) [TIOCSTART] */
/* #define DIOCAWEDGE _IOWR('d', 121, struct dkwedge_info) [DIOCAWEDGE] */
/* #define SIOCIFGCLONERS _IOWR('i', 120, struct if_clonereq) [SIOCIFGCLONERS] */
/* #define SIOCDELRT _IOW('r', 11, struct ortentry) [SIOCDELRT] */
/* #define FIOSETOWN _IOW('f', 124, int) [FIOSETOWN] */
/* #define DIOCSCACHE _IOW('d', 117, int) [DIOCSCACHE] */
/* #define DIOCRFORMAT _IOWR('d', 105, struct format_op) [DIOCRFORMAT] */
/* #define PPS_IOC_CREATE _IO('1', 1) [PPS_IOC_CREATE] */
	case PPS_IOC_CREATE:
		return "PPS_IOC_CREATE";
/* #define SIOCGIFDSTADDR _IOWR('i', 34, struct ifreq) [SIOCGIFDSTADDR] */
/* #define FIONREAD _IOR('f', 127, int) [FIONREAD] */
/* #define TIOCSTAT _IOW('t', 101, int) [TIOCSTAT] */
/* #define SIOCGHIWAT _IOR('s', 1, int) [SIOCGHIWAT] */
/* #define DIOCGPART _IOW('d', 104, struct partinfo) [DIOCGPART] */
/* #define TIOCMGET _IOR('t', 106, int) [TIOCMGET] */
/* #define TIOCNXCL _IO('t', 14) [TIOCNXCL] */
/* #define SIOCSIFFLAGS _IOW('i', 16, struct ifreq) [SIOCSIFFLAGS] */
/* #define SIOCGLOWAT _IOR('s', 3, int) [SIOCGLOWAT] */
/* #define SIOCSIFMTU _IOW('i', 127, struct ifreq) [SIOCSIFMTU] */
/* #define SIOCGIFMEDIA _IOWR('i', 54, struct ifmediareq) [SIOCGIFMEDIA] */
/* #define TIOCFLUSH _IOW('t', 16, int) [TIOCFLUSH] */
/* #define DIOCEJECT _IOW('d', 112, int) [DIOCEJECT] */
/* #define TIOCGWINSZ _IOR('t', 104, struct winsize) [TIOCGWINSZ] */
/* #define SIOCGIFCAP _IOWR('i', 118, struct ifcapreq) [SIOCGIFCAP] */
/* #define TIOCGPGRP _IOR('t', 119, int) [TIOCGPGRP] */
/* #define SIOCGIFNETMASK _IOWR('i', 37, struct ifreq) [SIOCGIFNETMASK] */
/* #define TIOCPKT _IOW('t', 112, int) [TIOCPKT] */
/* #define OSIOCGIFDSTADDR _IOWR('i', 15, struct ifreq) [OSIOCGIFDSTADDR] */
/* #define DIOCBSFLUSH _IO('d', 120) [DIOCBSFLUSH] */
/* #define FIONCLEX _IO('f', 2) [FIONCLEX] */
/* #define SIOCSLIFPHYADDR _IOW('i', 74, struct if_laddrreq) [SIOCSLIFPHYADDR] */
/* #define FIOCLEX _IO('f', 1) [FIOCLEX] */
/* #define SIOCGIFBRDADDR _IOWR('i', 35, struct ifreq) [SIOCGIFBRDADDR] */
/* #define TIOCPTMGET _IOR('t', 70, struct ptmget) [TIOCPTMGET] */
/* #define TIOCEXT _IOW('t', 96, int) [TIOCEXT] */
/* #define SIOCIFDESTROY _IOW('i', 121, struct ifreq) [SIOCIFDESTROY] */
/* #define DIOCGDINFO _IOR('d', 101, struct disklabel) [DIOCGDINFO] */
/* #define SIOCGIFCONF _IOWR('i', 36, struct ifconf) [SIOCGIFCONF] */
/* #define OFIOGETBMAP _IOWR('f', 122, uint32_t) [OFIOGETBMAP] */
/* #define TIOCSCTTY _IO('t', 97) [TIOCSCTTY] */
/* #define FIONSPACE _IOR('f', 120, int) [FIONSPACE] */
/* #define DIOCCLRLABEL _IO('d', 115) [DIOCCLRLABEL] */
/* #define FIOGETBMAP _IOWR('f', 122, daddr_t) [FIOGETBMAP] */
/* #define TIOCOUTQ _IOR('t', 115, int) [TIOCOUTQ] */
/* #define TIOCCBRK _IO('t', 122) [TIOCCBRK] */
/* #define OSIOCGIFNETMASK _IOWR('i', 21, struct ifreq) [OSIOCGIFNETMASK] */
/* #define TIOCREMOTE _IOW('t', 105, int) [TIOCREMOTE] */
/* #define SIOCADDRT _IOW('r', 10, struct ortentry) [SIOCADDRT] */
/* #define PPS_IOC_GETCAP _IOR('1', 5, int) [PPS_IOC_GETCAP] */
	case PPS_IOC_GETCAP:
		return "PPS_IOC_GETCAP";
/* #define SIOCGIFDATA _IOWR('i', 128, struct ifdatareq) [SIOCGIFDATA] */
/* #define PPS_IOC_SETPARAMS _IOW('1', 3, pps_params_t) [PPS_IOC_SETPARAMS] */
	case PPS_IOC_SETPARAMS:
		return "PPS_IOC_SETPARAMS";
/* #define DIOCWDINFO _IOW('d', 103, struct disklabel) [DIOCWDINFO] */
/* #define SIOCDELMULTI _IOW('i', 50, struct ifreq) [SIOCDELMULTI] */
/* #define OSIOCGIFBRDADDR _IOWR('i', 18, struct ifreq) [OSIOCGIFBRDADDR] */
/* #define TIOCMBIC _IOW('t', 107, int) [TIOCMBIC] */
/* #define TIOCMBIS _IOW('t', 108, int) [TIOCMBIS] */
/* #define SIOCSIFGENERIC _IOW('i', 57, struct ifreq) [SIOCSIFGENERIC] */
/* #define SIOCSDRVSPEC _IOW('i', 123, struct ifdrv) [SIOCSDRVSPEC] */
/* #define TIOCSETA _IOW('t', 20, struct termios) [TIOCSETA] */
/* #define TIOCCONS _IOW('t', 98, int) [TIOCCONS] */
/* #define TIOCSPGRP _IOW('t', 118, int) [TIOCSPGRP] */
/* #define TIOCGSID _IOR('t', 99, int) [TIOCGSID] */
/* #define _IO(g,n) _IOC(IOC_VOID, (g), (n), 0) [_IO(g,n)] */
/* #define TIOCCDTR _IO('t', 120) [TIOCCDTR] */
/* #define SIOCATMARK _IOR('s', 7, int) [SIOCATMARK] */
/* #define PTIOCNETBSD _IOW('Z', 0, struct ioctl_pt) [PTIOCNETBSD] */
/* #define SIOCSLOWAT _IOW('s', 2, int) [SIOCSLOWAT] */
/* #define PTIOCLINUX _IOW('Z', 3, struct ioctl_pt) [PTIOCLINUX] */
/* #define DIOCSBAD _IOW('d', 110, struct dkbad) [DIOCSBAD] */
/* #define SIOCSIFADDR _IOW('i', 12, struct ifreq) [SIOCSIFADDR] */
/* #define TIOCSWINSZ _IOW('t', 103, struct winsize) [TIOCSWINSZ] */
/* #define SIOCSIFCAP _IOW('i', 117, struct ifcapreq) [SIOCSIFCAP] */
/* #define SIOCGIFFLAGS _IOWR('i', 17, struct ifreq) [SIOCGIFFLAGS] */
/* #define PPS_IOC_GETPARAMS _IOR('1', 4, pps_params_t) [PPS_IOC_GETPARAMS] */
	case PPS_IOC_GETPARAMS:
		return "PPS_IOC_GETPARAMS";
/* #define TIOCSETAF _IOW('t', 22, struct termios) [TIOCSETAF] */
/* #define TIOCSETAW _IOW('t', 21, struct termios) [TIOCSETAW] */
/* #define PPS_IOC_FETCH _IOWR('1', 6, pps_info_t) [PPS_IOC_FETCH] */
	case PPS_IOC_FETCH:
		return "PPS_IOC_FETCH";
/* #define _IOC(inout,group,num,len) (inout | ((len & IOCPARM_MASK) << 16) | ((group) << 8) | (num)) [_IOC(inout,group,num,len)] */
/* #define SIOCSIFMETRIC _IOW('i', 24, struct ifreq) [SIOCSIFMETRIC] */
/* #define _IOR(g,n,t) _IOC(IOC_OUT, (g), (n), sizeof(t)) [_IOR(g,n,t)] */
/* #define _IOW(g,n,t) _IOC(IOC_IN, (g), (n), sizeof(t)) [_IOW(g,n,t)] */
/* #define ODIOCEJECT _IO('d', 112) [ODIOCEJECT] */
/* #define SIOCGIFALIAS _IOWR('i', 27, struct ifaliasreq) [SIOCGIFALIAS] */
/* #define TIOCGFLAGS _IOR('t', 93, int) [TIOCGFLAGS] */
/* #define DIOCKLABEL _IOW('d', 119, int) [DIOCKLABEL] */
/* #define DIOCDWEDGE _IOW('d', 123, struct dkwedge_info) [DIOCDWEDGE] */
/* #define TIOCNOTTY _IO('t', 113) [TIOCNOTTY] */
/* #define TIOCSTOP _IO('t', 111) [TIOCSTOP] */
/* #define PTIOCFREEBSD _IOW('Z', 4, struct ioctl_pt) [PTIOCFREEBSD] */
/* #define DIOCSDINFO _IOW('d', 102, struct disklabel) [DIOCSDINFO] */
/* #define TIOCDCDTIMESTAMP _IOR('t', 88, struct timeval) [TIOCDCDTIMESTAMP] */
/* #define SIOCIFCREATE _IOW('i', 122, struct ifreq) [SIOCIFCREATE] */
/* #define DIOCLOCK _IOW('d', 113, int) [DIOCLOCK] */
/* #define SIOCSIFPHYADDR _IOW('i', 70, struct ifaliasreq) [SIOCSIFPHYADDR] */
/* #define DIOCBSLIST _IOWR('d', 119, struct disk_badsecinfo) [DIOCBSLIST] */
/* #define TIOCGLINED _IOR('t', 66, linedn_t) [TIOCGLINED] */
/* #define TIOCMSET _IOW('t', 109, int) [TIOCMSET] */
/* #define TIOCRCVFRAME _IOW('t', 69, struct mbuf *) [TIOCRCVFRAME] */
/* #define TIOCDRAIN _IO('t', 94) [TIOCDRAIN] */
/* #define TIOCEXCL _IO('t', 13) [TIOCEXCL] */
/* #define TIOCPKT_IOCTL 0x40 [TIOCPKT_IOCTL] */
/* #define TIOCUCNTL _IOW('t', 102, int) [TIOCUCNTL] */
/* #define TIOCSBRK _IO('t', 123) [TIOCSBRK] */
/* #define TIOCGRANTPT _IO('t', 71) [TIOCGRANTPT] */
/* #define TIOCSIG _IO('t', 95) [TIOCSIG] */
/* #define _IOWR(g,n,t) _IOC(IOC_INOUT, (g), (n), sizeof(t)) [_IOWR(g,n,t)] */
/* #define TIOCGETA _IOR('t', 19, struct termios) [TIOCGETA] */
/* #define TIOCGETD _IOR('t', 26, int) [TIOCGETD] */
/* #define TIOCSDTR _IO('t', 121) [TIOCSDTR] */
/* #define TIOCXMTFRAME _IOW('t', 68, struct mbuf *) [TIOCXMTFRAME] */
/* #define TIOCSFLAGS _IOW('t', 92, int) [TIOCSFLAGS] */
/* #define TIOCSTI _IOW('t', 114, char) [TIOCSTI] */
/* #define TIOCPTSNAME _IOR('t', 72, struct ptmget) [TIOCPTSNAME] */
/* #define TIOCSLINED _IOW('t', 67, linedn_t) [TIOCSLINED] */
/* #define TIOCSTART _IO('t', 110) [TIOCSTART] */
/* #define TIOCSTAT _IOW('t', 101, int) [TIOCSTAT] */
/* #define TIOCMGET _IOR('t', 106, int) [TIOCMGET] */
/* #define TIOCNXCL _IO('t', 14) [TIOCNXCL] */
/* #define TIOCFLUSH _IOW('t', 16, int) [TIOCFLUSH] */
/* #define TIOCGWINSZ _IOR('t', 104, struct winsize) [TIOCGWINSZ] */
/* #define TIOCGPGRP _IOR('t', 119, int) [TIOCGPGRP] */
/* #define TIOCPKT _IOW('t', 112, int) [TIOCPKT] */
/* #define TIOCPTMGET _IOR('t', 70, struct ptmget) [TIOCPTMGET] */
/* #define TIOCEXT _IOW('t', 96, int) [TIOCEXT] */
/* #define TIOCSCTTY _IO('t', 97) [TIOCSCTTY] */
/* #define TIOCOUTQ _IOR('t', 115, int) [TIOCOUTQ] */
/* #define TIOCCBRK _IO('t', 122) [TIOCCBRK] */
/* #define TIOCREMOTE _IOW('t', 105, int) [TIOCREMOTE] */
/* #define TIOCMBIC _IOW('t', 107, int) [TIOCMBIC] */
/* #define TIOCMBIS _IOW('t', 108, int) [TIOCMBIS] */
/* #define TIOCSETA _IOW('t', 20, struct termios) [TIOCSETA] */
/* #define TIOCSETD _IOW('t', 27, int) [TIOCSETD] */
/* #define TIOCCONS _IOW('t', 98, int) [TIOCCONS] */
/* #define TIOCSPGRP _IOW('t', 118, int) [TIOCSPGRP] */
/* #define TIOCGSID _IOR('t', 99, int) [TIOCGSID] */
/* #define _IO(g,n) _IOC(IOC_VOID, (g), (n), 0) [_IO(g,n)] */
/* #define TIOCCDTR _IO('t', 120) [TIOCCDTR] */
/* #define TIOCSWINSZ _IOW('t', 103, struct winsize) [TIOCSWINSZ] */
/* #define TIOCSETAF _IOW('t', 22, struct termios) [TIOCSETAF] */
/* #define TIOCSETAW _IOW('t', 21, struct termios) [TIOCSETAW] */
/* #define _IOC(inout,group,num,len) (inout | ((len & IOCPARM_MASK) << 16) | ((group) << 8) | (num)) [_IOC(inout,group,num,len)] */
/* #define _IOR(g,n,t) _IOC(IOC_OUT, (g), (n), sizeof(t)) [_IOR(g,n,t)] */
/* #define _IOW(g,n,t) _IOC(IOC_IN, (g), (n), sizeof(t)) [_IOW(g,n,t)] */
/* #define TIOCGFLAGS _IOR('t', 93, int) [TIOCGFLAGS] */
/* #define TIOCNOTTY _IO('t', 113) [TIOCNOTTY] */
/* #define TIOCSTOP _IO('t', 111) [TIOCSTOP] */
/* #define TIOCDCDTIMESTAMP _IOR('t', 88, struct timeval) [TIOCDCDTIMESTAMP] */
/* #define BLKDEV_IOSIZE 2048 [BLKDEV_IOSIZE] */
/* #define _IOWR(g,n,t) _IOC(IOC_INOUT, (g), (n), sizeof(t)) [_IOWR(g,n,t)] */
/* #define _IO(g,n) _IOC(IOC_VOID, (g), (n), 0) [_IO(g,n)] */
/* #define _IOC(inout,group,num,len) (inout | ((len & IOCPARM_MASK) << 16) | ((group) << 8) | (num)) [_IOC(inout,group,num,len)] */
/* #define _IOR(g,n,t) _IOC(IOC_OUT, (g), (n), sizeof(t)) [_IOR(g,n,t)] */
/* #define _IOW(g,n,t) _IOC(IOC_IN, (g), (n), sizeof(t)) [_IOW(g,n,t)] */
	default:
		return NULL;

	}

}
