/* XXX obnoxious prerequisites. */
#define COMPAT_43
#include <sys/param.h>
#include <sys/devicestat.h>
#include <sys/disklabel.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/tty.h>
#include <net/ethernet.h>
#include <net/if.h>
#include <net/if_var.h>
#include <net/pfvar.h>
#include <net/route.h>
#include <netatm/atm.h>
#include <netatm/atm_if.h>
#include <netatm/atm_sap.h>
#include <netatm/atm_sys.h>
#include <netinet/in.h>
#include <netinet/ip_mroute.h>
#include <netinet6/in6_var.h>
#include <netinet6/nd6.h>
#include <netinet6/ip6_mroute.h>
#include <stdio.h>
#include <cam/cam.h>

const char *ioctlname(u_long val);

#include <altq/altq.h>
#include <altq/altq_cbq.h>
#include <altq/altq_cdnr.h>
#include <altq/altq_hfsc.h>
#include <altq/altq_priq.h>
#include <altq/altq_red.h>
#include <altq/altq_rio.h>
#include <cam/scsi/scsi_pass.h>
#include <cam/scsi/scsi_ses.h>
#include <cam/scsi/scsi_targetio.h>
#include <crypto/cryptodev.h>
#include <dev/acpica/acpiio.h>
#include <dev/bktr/ioctl_bt848.h>
#include <dev/bktr/ioctl_meteor.h>
#include <dev/firewire/firewire.h>
#include <dev/ieee488/ibfoo_int.h>
#include <dev/iicbus/iic.h>
#include <dev/ofw/openfirmio.h>
#include <dev/ofw/openpromio.h>
#include <dev/pbio/pbioio.h>
#include <dev/ppbus/lptio.h>
#include <dev/ppbus/ppi.h>
#include <dev/smbus/smb.h>
#include <dev/usb/dsbr100io.h>
#include <dev/usb/rio500_usb.h>
#include <dev/usb/usb.h>
#include <dev/wi/if_wavelan_ieee.h>
#include <fs/devfs/devfs.h>
#include <fs/nwfs/nwfs.h>
#include <geom/gate/g_gate.h>
#include <geom/geom_ctl.h>
#include <machine/apm_bios.h>
#include <machine/asc_ioctl.h>
#include <machine/cdk.h>
#include <machine/comstats.h>
#include <machine/cserial.h>
#include <machine/gsc.h>
#include <machine/i4b_debug.h>
#include <machine/i4b_ioctl.h>
#include <machine/i4b_rbch_ioctl.h>
#include <machine/i4b_tel_ioctl.h>
#include <machine/i4b_trace.h>
#include <machine/if_wl_wavelan.h>
#include <machine/ioctl_ctx.h>
#include <machine/pcaudioio.h>
#include <machine/pcvt_ioctl.h>
#include <machine/perfmon.h>
#include <machine/smapi.h>
#include <machine/speaker.h>
#include <machine/spigot.h>
#include <net/bpf.h>
#include <net/if_atm.h>
#include <net/if_gre.h>
#include <net/if_pfsync.h>
#include <net/if_ppp.h>
#include <net/if_tap.h>
#include <net/if_tun.h>
#include <net/pfvar.h>
#include <net/slip.h>
#include <net80211/ieee80211_ioctl.h>
#include <netatalk/phase2.h>
#include <netatm/atm_ioctl.h>
#include <netgraph/ng_message.h>
#include <netinet/ip_carp.h>
#include <netinet/ip_fil.h>
#include <netinet/ip_lookup.h>
#include <netinet/ip_nat.h>
#include <netinet/ip_proxy.h>
#include <netinet/ip_scan.h>
#include <netinet/ip_state.h>
#include <netinet6/in6_var.h>
#include <netncp/ncpio.h>
#include <netsmb/smb_dev.h>
#include <pccard/cardinfo.h>
#include <rpc/des.h>
#include <sys/aac_ioctl.h>
#include <sys/agpio.h>
#include <sys/ata.h>
#include <sys/cdio.h>
#include <sys/cdrio.h>
#include <sys/chio.h>
#include <sys/consio.h>
#include <sys/dataacq.h>
#include <sys/digiio.h>
#include <sys/disk.h>
#include <sys/disklabel.h>
#include <sys/diskmbr.h>
#include <sys/diskpc98.h>
#include <sys/dvdio.h>
#include <sys/fbio.h>
#include <sys/fdcio.h>
#include <sys/filio.h>
#include <sys/ioctl_compat.h>
#include <sys/joystick.h>
#include <sys/kbio.h>
#include <sys/mdioctl.h>
#include <sys/memrange.h>
#include <sys/mouse.h>
#include <sys/mtio.h>
#include <sys/pciio.h>
#include <sys/pioctl.h>
#include <sys/ptio.h>
#include <sys/snoop.h>
#include <sys/sockio.h>
#include <sys/soundcard.h>
#include <sys/tiio.h>
#include <sys/timepps.h>
#include <sys/ttycom.h>
#include <sys/watchdog.h>
#include <sys/xrpuio.h>


const char *
ioctlname(u_long val)
{

	if (val ==  SW_TEXT_80x60)
		return("SW_TEXT_80x60");
	if (val ==  IOCATARAIDREBUILD)
		return("IOCATARAIDREBUILD");
	if (val ==  PIOCSIO)
		return("PIOCSIO");
	if (val ==  SIOCGPRISM2DEBUG)
		return("SIOCGPRISM2DEBUG");
	if (val ==  CTX_SET_LUT)
		return("CTX_SET_LUT");
	if (val ==  CDNR_TCM_STATS)
		return("CDNR_TCM_STATS");
	if (val ==  SNDCTL_MIDI_PRETIME)
		return("SNDCTL_MIDI_PRETIME");
	if (val ==  RADIO_GETFREQ)
		return("RADIO_GETFREQ");
	if (val ==  DIGIIO_GETALTPIN)
		return("DIGIIO_GETALTPIN");
	if (val ==  CDIOCSETLEFT)
		return("CDIOCSETLEFT");
	if (val ==  SNDCTL_COPR_LOAD)
		return("SNDCTL_COPR_LOAD");
	if (val ==  CIOCFSESSION)
		return("CIOCFSESSION");
	if (val ==  METEORGETGEO)
		return("METEORGETGEO");
	if (val ==  SIOCPHASE1)
		return("SIOCPHASE1");
	if (val ==  CONS_FINDMODE)
		return("CONS_FINDMODE");
	if (val ==  CDRIOCREADSPEED)
		return("CDRIOCREADSPEED");
	if (val ==  METEORGVIDEO)
		return("METEORGVIDEO");
	if (val ==  CONS_ADPINFO)
		return("CONS_ADPINFO");
	if (val ==  SW_CG640x480)
		return("SW_CG640x480");
	if (val ==  SIOCGPRIVATE_1)
		return("SIOCGPRIVATE_1");
	if (val ==  PMIOGET)
		return("PMIOGET");
	if (val ==  I4B_CTL_SET_DEBUG)
		return("I4B_CTL_SET_DEBUG");
	if (val ==  FSACTL_PROBE_CONTAINERS)
		return("FSACTL_PROBE_CONTAINERS");
	if (val ==  SOUND_MIXER_INFO)
		return("SOUND_MIXER_INFO");
	if (val ==  GPIB_IBFOO)
		return("GPIB_IBFOO");
	if (val ==  SW_VGA_M90x25)
		return("SW_VGA_M90x25");
	if (val ==  SIOCSTGET)
		return("SIOCSTGET");
	if (val ==  MOUSE_GETVARS)
		return("MOUSE_GETVARS");
	if (val ==  DIOCCLRRULECTRS)
		return("DIOCCLRRULECTRS");
	if (val ==  CDRIOCSENDCUE)
		return("CDRIOCSENDCUE");
	if (val ==  SW_VGA_M90x43)
		return("SW_VGA_M90x43");
	if (val ==  METEORGFMT)
		return("METEORGFMT");
	if (val ==  DIOCRGETTABLES)
		return("DIOCRGETTABLES");
	if (val ==  SW_TEXT_80x25)
		return("SW_TEXT_80x25");
	if (val ==  SIOCSPFXFLUSH_IN6)
		return("SIOCSPFXFLUSH_IN6");
	if (val ==  CDNR_IF_ATTACH)
		return("CDNR_IF_ATTACH");
	if (val ==  BT848_SUSAT)
		return("BT848_SUSAT");
	if (val ==  SW_TEXT_80x30)
		return("SW_TEXT_80x30");
	if (val ==  SW_VGA_M90x60)
		return("SW_VGA_M90x60");
	if (val ==  APMIO_HALTCPU)
		return("APMIO_HALTCPU");
	if (val ==  SIOCGIFPSRCADDR)
		return("SIOCGIFPSRCADDR");
	if (val ==  SIOCIPFSET)
		return("SIOCIPFSET");
	if (val ==  G_GATE_CMD_CREATE)
		return("G_GATE_CMD_CREATE");
	if (val ==  COM_GETBRDSTATS)
		return("COM_GETBRDSTATS");
	if (val ==  VMIO_SIOCNETIF)
		return("VMIO_SIOCNETIF");
	if (val ==  DIOCGETRULESETS)
		return("DIOCGETRULESETS");
	if (val ==  SIOCGIFADDR)
		return("SIOCGIFADDR");
	if (val ==  SERIAL_GETUSE16)
		return("SERIAL_GETUSE16");
	if (val ==  FSACTL_SET_VAR)
		return("FSACTL_SET_VAR");
	if (val ==  SIOCADNAT)
		return("SIOCADNAT");
	if (val ==  SOUND_PCM_WRITE_FILTER)
		return("SOUND_PCM_WRITE_FILTER");
	if (val ==  KDGKBTYPE)
		return("KDGKBTYPE");
	if (val ==  SIOCGIFCONF)
		return("SIOCGIFCONF");
	if (val ==  SNDCTL_DSP_SETFRAGMENT)
		return("SNDCTL_DSP_SETFRAGMENT");
	if (val ==  TUNSDEBUG)
		return("TUNSDEBUG");
	if (val ==  SIOCGIFADDR_IN6)
		return("SIOCGIFADDR_IN6");
	if (val ==  DIOCGETSTATUS)
		return("DIOCGETSTATUS");
	if (val ==  SIOCGIFMEDIA)
		return("SIOCGIFMEDIA");
	if (val ==  PPPIOCGNPMODE)
		return("PPPIOCGNPMODE");
	if (val ==  SESIOC_GETOBJMAP)
		return("SESIOC_GETOBJMAP");
	if (val ==  CDIOCPITCH)
		return("CDIOCPITCH");
	if (val ==  SIOCLOOKUPDELNODE)
		return("SIOCLOOKUPDELNODE");
	if (val ==  SIOCSTGSZ)
		return("SIOCSTGSZ");
	if (val ==  CDRIOCWRITESPEED)
		return("CDRIOCWRITESPEED");
	if (val ==  IOCATAGPARM)
		return("IOCATAGPARM");
	if (val ==  SERIAL_SETLOOP)
		return("SERIAL_SETLOOP");
	if (val ==  AIOCCFG)
		return("AIOCCFG");
	if (val ==  SNDCTL_COPR_RCVMSG)
		return("SNDCTL_COPR_RCVMSG");
	if (val ==  SNDCTL_FM_4OP_ENABLE)
		return("SNDCTL_FM_4OP_ENABLE");
	if (val ==  SW_VESA_64K_320)
		return("SW_VESA_64K_320");
	if (val ==  SW_VESA_CG640x400)
		return("SW_VESA_CG640x400");
	if (val ==  SW_VGA_M80x25)
		return("SW_VGA_M80x25");
	if (val ==  SERIAL_GETMODE)
		return("SERIAL_GETMODE");
	if (val ==  SW_VGA_M80x30)
		return("SW_VGA_M80x30");
	if (val ==  SIOCSIFNETMASK)
		return("SIOCSIFNETMASK");
	if (val ==  TIOCGETD)
		return("TIOCGETD");
	if (val ==  MOUSE_READSTATE)
		return("MOUSE_READSTATE");
	if (val ==  METEORGINPUT)
		return("METEORGINPUT");
	if (val ==  CONS_GSAVER)
		return("CONS_GSAVER");
	if (val ==  SW_VGA_M80x50)
		return("SW_VGA_M80x50");
	if (val ==  FWOHCI_RDREG)
		return("FWOHCI_RDREG");
	if (val ==  FW_GTSTREAM)
		return("FW_GTSTREAM");
	if (val ==  VT_GETINDEX)
		return("VT_GETINDEX");
	if (val ==  USB_SETDEBUG)
		return("USB_SETDEBUG");
	if (val ==  FBIOGETCMAP)
		return("FBIOGETCMAP");
	if (val ==  SNDCTL_TMR_SOURCE)
		return("SNDCTL_TMR_SOURCE");
	if (val ==  DIOCRCLRASTATS)
		return("DIOCRCLRASTATS");
	if (val ==  TIOCSDTR)
		return("TIOCSDTR");
	if (val ==  I4B_CTRL_DOWNLOAD)
		return("I4B_CTRL_DOWNLOAD");
	if (val ==  METEORGCONT)
		return("METEORGCONT");
	if (val ==  FIOGETLBA)
		return("FIOGETLBA");
	if (val ==  STL_BRESET)
		return("STL_BRESET");
	if (val ==  SW_CG320)
		return("SW_CG320");
	if (val ==  I4B_DIALOUT_RESP)
		return("I4B_DIALOUT_RESP");
	if (val ==  SIOCSDEFIFACE_IN6)
		return("SIOCSDEFIFACE_IN6");
	if (val ==  TIOCMSDTRWAIT)
		return("TIOCMSDTRWAIT");
	if (val ==  I2CSTART)
		return("I2CSTART");
	if (val ==  PPS_IOC_DESTROY)
		return("PPS_IOC_DESTROY");
	if (val ==  SW_VESA_CG640x480)
		return("SW_VESA_CG640x480");
	if (val ==  SERIAL_GETMONITOR)
		return("SERIAL_GETMONITOR");
	if (val ==  STL_GETPFLAG)
		return("STL_GETPFLAG");
	if (val ==  SERIAL_RESET)
		return("SERIAL_RESET");
	if (val ==  SESIOC_GETNOBJ)
		return("SESIOC_GETNOBJ");
	if (val ==  DIOCGMEDIASIZE)
		return("DIOCGMEDIASIZE");
	if (val ==  FBIOPUTCMAPI)
		return("FBIOPUTCMAPI");
	if (val ==  I4B_DISCONNECT_REQ)
		return("I4B_DISCONNECT_REQ");
	if (val ==  TVTUNER_SETFREQ)
		return("TVTUNER_SETFREQ");
	if (val ==  SW_CG640_E)
		return("SW_CG640_E");
	if (val ==  SW_VESA_64K_1280)
		return("SW_VESA_64K_1280");
	if (val ==  GSC_SBTIME)
		return("GSC_SBTIME");
	if (val ==  APMIO_STANDBY)
		return("APMIO_STANDBY");
	if (val ==  SIOCSIFINFO_IN6)
		return("SIOCSIFINFO_IN6");
	if (val ==  METEORGCHCV)
		return("METEORGCHCV");
	if (val ==  AIOCDEL)
		return("AIOCDEL");
	if (val ==  BT848_REEPROM)
		return("BT848_REEPROM");
	if (val ==  FSACTL_QUERY_DISK)
		return("FSACTL_QUERY_DISK");
	if (val ==  SIOCGETFF)
		return("SIOCGETFF");
	if (val ==  KDSETLED)
		return("KDSETLED");
	if (val ==  TIOCLSET)
		return("TIOCLSET");
	if (val ==  PMIOTSTAMP)
		return("PMIOTSTAMP");
	if (val ==  CONS_RELKBD)
		return("CONS_RELKBD");
	if (val ==  AIOGFMT)
		return("AIOGFMT");
	if (val ==  GSC_GRES)
		return("GSC_GRES");
	if (val ==  SIOCGSCOPE6)
		return("SIOCGSCOPE6");
	if (val ==  SW_VGA_M80x60)
		return("SW_VGA_M80x60");
	if (val ==  SERIAL_GETCABLEN)
		return("SERIAL_GETCABLEN");
	if (val ==  FSACTL_GET_VAR)
		return("FSACTL_GET_VAR");
	if (val ==  ASC_SWIDTH)
		return("ASC_SWIDTH");
	if (val ==  METEORCAPTUR)
		return("METEORCAPTUR");
	if (val ==  SIOCADIFR)
		return("SIOCADIFR");
	if (val ==  GSC_GWIDTH)
		return("GSC_GWIDTH");
	if (val ==  BIOCSETF)
		return("BIOCSETF");
	if (val ==  CDNR_ADD_FILTER)
		return("CDNR_ADD_FILTER");
	if (val ==  CDRIOCGETBLOCKSIZE)
		return("CDRIOCGETBLOCKSIZE");
	if (val ==  CDNR_TBM_STATS)
		return("CDNR_TBM_STATS");
	if (val ==  DIOCRGETTSTATS)
		return("DIOCRGETTSTATS");
	if (val ==  BIOCGBLEN)
		return("BIOCGBLEN");
	if (val ==  SW_BG320)
		return("SW_BG320");
	if (val ==  SW_VESA_FULL_1280)
		return("SW_VESA_FULL_1280");
	if (val ==  SW_PC98_PEGC640x400)
		return("SW_PC98_PEGC640x400");
	if (val ==  SERIAL_GETINVCLK)
		return("SERIAL_GETINVCLK");
	if (val ==  FSACTL_SENDFIB)
		return("FSACTL_SENDFIB");
	if (val ==  FD_STYPE)
		return("FD_STYPE");
	if (val ==  BT848_GVSAT)
		return("BT848_GVSAT");
	if (val ==  SIOCSIFALIFETIME_IN6)
		return("SIOCSIFALIFETIME_IN6");
	if (val ==  SNDCTL_DSP_MAPINBUF)
		return("SNDCTL_DSP_MAPINBUF");
	if (val ==  DVDIOCREPORTKEY)
		return("DVDIOCREPORTKEY");
	if (val ==  SNPSTTY)
		return("SNPSTTY");
	if (val ==  USB_GET_REPORT_ID)
		return("USB_GET_REPORT_ID");
	if (val ==  DIOCADDALTQ)
		return("DIOCADDALTQ");
	if (val ==  APMIO_BIOS)
		return("APMIO_BIOS");
	if (val ==  SIOCZRLST)
		return("SIOCZRLST");
	if (val ==  MOUSE_GETLEVEL)
		return("MOUSE_GETLEVEL");
	if (val ==  TIOCMGDTRWAIT)
		return("TIOCMGDTRWAIT");
	if (val ==  SERIAL_GETCABLE)
		return("SERIAL_GETCABLE");
	if (val ==  BIOCSSEESENT)
		return("BIOCSSEESENT");
	if (val ==  PIOCSVIR)
		return("PIOCSVIR");
	if (val ==  SNDCTL_SEQ_GETOUTCOUNT)
		return("SNDCTL_SEQ_GETOUTCOUNT");
	if (val ==  CDIOCRESET)
		return("CDIOCRESET");
	if (val ==  SNDCTL_COPR_RDATA)
		return("SNDCTL_COPR_RDATA");
	if (val ==  USB_DEVICESTATS)
		return("USB_DEVICESTATS");
	if (val ==  PPS_IOC_FETCH)
		return("PPS_IOC_FETCH");
	if (val ==  COM_GETPORTSTATS)
		return("COM_GETPORTSTATS");
	if (val ==  SPIGOT_SETINT)
		return("SPIGOT_SETINT");
	if (val ==  VMIO_SIOCSETMACADDR)
		return("VMIO_SIOCSETMACADDR");
	if (val ==  METEORGCOUNT)
		return("METEORGCOUNT");
	if (val ==  PIO_DEADKEYMAP)
		return("PIO_DEADKEYMAP");
	if (val ==  OFIOCGETNEXT)
		return("OFIOCGETNEXT");
	if (val ==  VT_SETMODE)
		return("VT_SETMODE");
	if (val ==  JOY_GET_Y_OFFSET)
		return("JOY_GET_Y_OFFSET");
	if (val ==  I4B_TIMEOUT_UPD)
		return("I4B_TIMEOUT_UPD");
	if (val ==  G_GATE_CMD_CANCEL)
		return("G_GATE_CMD_CANCEL");
	if (val ==  FD_GDTYPE)
		return("FD_GDTYPE");
	if (val ==  TUNSIFHEAD)
		return("TUNSIFHEAD");
	if (val ==  DVDIOCSENDKEY)
		return("DVDIOCSENDKEY");
	if (val ==  WDIOCPATPAT)
		return("WDIOCPATPAT");
	if (val ==  TVTUNER_GETFREQ)
		return("TVTUNER_GETFREQ");
	if (val ==  MTIOCHLOCATE)
		return("MTIOCHLOCATE");
	if (val ==  SERIAL_SETCFG)
		return("SERIAL_SETCFG");
	if (val ==  USB_SET_REPORT)
		return("USB_SET_REPORT");
	if (val ==  SIOCSIFINFO_FLAGS)
		return("SIOCSIFINFO_FLAGS");
	if (val ==  SIOCDWLCACHE)
		return("SIOCDWLCACHE");
	if (val ==  SIOCIPFGET)
		return("SIOCIPFGET");
	if (val ==  VGASETFONTATTR)
		return("VGASETFONTATTR");
	if (val ==  VMIO_SIOCSIFFLAGS)
		return("VMIO_SIOCSIFFLAGS");
	if (val ==  AIOCSET)
		return("AIOCSET");
	if (val ==  SERIAL_HARDRESET)
		return("SERIAL_HARDRESET");
	if (val ==  SIOCGIFNETMASK)
		return("SIOCGIFNETMASK");
	if (val ==  I4B_TEL_SETAUDIOFMT)
		return("I4B_TEL_SETAUDIOFMT");
	if (val ==  DIOCSTART)
		return("DIOCSTART");
	if (val ==  VGASETSCREEN)
		return("VGASETSCREEN");
	if (val ==  FW_GDEVLST)
		return("FW_GDEVLST");
	if (val ==  SERIAL_SETBAUD)
		return("SERIAL_SETBAUD");
	if (val ==  METEORSCSAT)
		return("METEORSCSAT");
	if (val ==  BT848_GHUE)
		return("BT848_GHUE");
	if (val ==  OPROMCHILD)
		return("OPROMCHILD");
	if (val ==  PMIORESET)
		return("PMIORESET");
	if (val ==  SNDCTL_DSP_SETFMT)
		return("SNDCTL_DSP_SETFMT");
	if (val ==  BT848GCLIP)
		return("BT848GCLIP");
	if (val ==  VGAGETSCREEN)
		return("VGAGETSCREEN");
	if (val ==  PPISEPPA)
		return("PPISEPPA");
	if (val ==  DIOCRDELADDRS)
		return("DIOCRDELADDRS");
	if (val ==  SNDCTL_SEQ_PANIC)
		return("SNDCTL_SEQ_PANIC");
	if (val ==  AIONWRITE)
		return("AIONWRITE");
	if (val ==  FWOHCI_WRPHYREG)
		return("FWOHCI_WRPHYREG");
	if (val ==  PIOCSREG)
		return("PIOCSREG");
	if (val ==  SIOCLOOKUPDELTABLE)
		return("SIOCLOOKUPDELTABLE");
	if (val ==  USB_GET_NO_ALT)
		return("USB_GET_NO_ALT");
	if (val ==  SIOCSIFLLADDR)
		return("SIOCSIFLLADDR");
	if (val ==  FD_FORM)
		return("FD_FORM");
	if (val ==  ASC_GHEIGHT)
		return("ASC_GHEIGHT");
	if (val ==  DIOCCHANGERULE)
		return("DIOCCHANGERULE");
	if (val ==  KBDSLEDS)
		return("KBDSLEDS");
	if (val ==  GIO_SCRNMAP)
		return("GIO_SCRNMAP");
	if (val ==  SIOCDELMULTI)
		return("SIOCDELMULTI");
	if (val ==  FSACTL_GET_FIBTIMES)
		return("FSACTL_GET_FIBTIMES");
	if (val ==  FIONCLEX)
		return("FIONCLEX");
	if (val ==  GEOM_CTL)
		return("GEOM_CTL");
	if (val ==  SW_C40x25)
		return("SW_C40x25");
	if (val ==  DIOCGETRULES)
		return("DIOCGETRULES");
	if (val ==  SW_EGAMONOAPA)
		return("SW_EGAMONOAPA");
	if (val ==  SW_ENH_B80x25)
		return("SW_ENH_B80x25");
	if (val ==  PPIGECR)
		return("PPIGECR");
	if (val ==  SERIAL_GETCRC4)
		return("SERIAL_GETCRC4");
	if (val ==  TARGIOCDISABLE)
		return("TARGIOCDISABLE");
	if (val ==  CHIOGPARAMS)
		return("CHIOGPARAMS");
	if (val ==  METEORSFMT)
		return("METEORSFMT");
	if (val ==  AGPIOC_SETUP)
		return("AGPIOC_SETUP");
	if (val ==  BT848_SIGNATURE)
		return("BT848_SIGNATURE");
	if (val ==  JOY_SET_X_OFFSET)
		return("JOY_SET_X_OFFSET");
	if (val ==  CDRIOCFLUSH)
		return("CDRIOCFLUSH");
	if (val ==  MOUSE_SYN_GETHWINFO)
		return("MOUSE_SYN_GETHWINFO");
	if (val ==  SW_ENH_B80x43)
		return("SW_ENH_B80x43");
	if (val ==  TIOCMGET)
		return("TIOCMGET");
	if (val ==  DIOCRSETADDRS)
		return("DIOCRSETADDRS");
	if (val ==  SW_VGA_C80x60)
		return("SW_VGA_C80x60");
	if (val ==  TIIOCGETTRACE)
		return("TIIOCGETTRACE");
	if (val ==  METEORSFPS)
		return("METEORSFPS");
	if (val ==  CDIOCPREVENT)
		return("CDIOCPREVENT");
	if (val ==  SW_CG640x350)
		return("SW_CG640x350");
	if (val ==  DIOCGETQSTATS)
		return("DIOCGETQSTATS");
	if (val ==  CHIOIELEM)
		return("CHIOIELEM");
	if (val ==  TIOCCBRK)
		return("TIOCCBRK");
	if (val ==  FBIOGCURPOS)
		return("FBIOGCURPOS");
	if (val ==  MIOGPASSTHRU)
		return("MIOGPASSTHRU");
	if (val ==  FM_GET_STAT)
		return("FM_GET_STAT");
	if (val ==  FWOHCI_RDPHYREG)
		return("FWOHCI_RDPHYREG");
	if (val ==  BT848_SCBARS)
		return("BT848_SCBARS");
	if (val ==  FW_STSTREAM)
		return("FW_STSTREAM");
	if (val ==  BIOCFLUSH)
		return("BIOCFLUSH");
	if (val ==  TVTUNER_SETCHNL)
		return("TVTUNER_SETCHNL");
	if (val ==  MOUSE_GETHWINFO)
		return("MOUSE_GETHWINFO");
	if (val ==  METEORGVWS)
		return("METEORGVWS");
	if (val ==  SW_PC98_PEGC640x480)
		return("SW_PC98_PEGC640x480");
	if (val ==  SIOCCIFPREFIX_IN6)
		return("SIOCCIFPREFIX_IN6");
	if (val ==  DIOCRINADEFINE)
		return("DIOCRINADEFINE");
	if (val ==  BT848_GCONT)
		return("BT848_GCONT");
	if (val ==  VT_GETMODE)
		return("VT_GETMODE");
	if (val ==  OSIOCGIFBRDADDR)
		return("OSIOCGIFBRDADDR");
	if (val ==  FBIOGETCMAPI)
		return("FBIOGETCMAPI");
	if (val ==  KDGETLED)
		return("KDGETLED");
	if (val ==  TUNGIFHEAD)
		return("TUNGIFHEAD");
	if (val ==  SIOCSNDFLUSH_IN6)
		return("SIOCSNDFLUSH_IN6");
	if (val ==  IOCATASMODE)
		return("IOCATASMODE");
	if (val ==  KBDRMKEY)
		return("KBDRMKEY");
	if (val ==  KBDDEFAULT)
		return("KBDDEFAULT");
	if (val ==  GSC_SRESSW)
		return("GSC_SRESSW");
	if (val ==  SIOCSGIFPREFIX_IN6)
		return("SIOCSGIFPREFIX_IN6");
	if (val ==  TIOCMSET)
		return("TIOCMSET");
	if (val ==  APMIO_ENABLE)
		return("APMIO_ENABLE");
	if (val ==  PBIO_GETDIFF)
		return("PBIO_GETDIFF");
	if (val ==  TIOCSPGRP)
		return("TIOCSPGRP");
	if (val ==  I4B_CONNECT_REQ)
		return("I4B_CONNECT_REQ");
	if (val ==  SIOCLOOKUPSTAT)
		return("SIOCLOOKUPSTAT");
	if (val ==  SERIAL_SETHIGAIN)
		return("SERIAL_SETHIGAIN");
	if (val ==  SW_VESA_CG800x600)
		return("SW_VESA_CG800x600");
	if (val ==  PIOCSRESOURCE)
		return("PIOCSRESOURCE");
	if (val ==  SERIAL_GETBAUD)
		return("SERIAL_GETBAUD");
	if (val ==  AUDIO_SETINFO)
		return("AUDIO_SETINFO");
	if (val ==  CONS_SETCURSORSHAPE)
		return("CONS_SETCURSORSHAPE");
	if (val ==  SIOCSLOWAT)
		return("SIOCSLOWAT");
	if (val ==  METEORGBT254)
		return("METEORGBT254");
	if (val ==  PCIOCREAD)
		return("PCIOCREAD");
	if (val ==  CDRIOCFIXATE)
		return("CDRIOCFIXATE");
	if (val ==  CONS_SCRSHOT)
		return("CONS_SCRSHOT");
	if (val ==  CONS_IO)
		return("CONS_IO");
	if (val ==  KDRASTER)
		return("KDRASTER");
	if (val ==  CDIOCSETSTEREO)
		return("CDIOCSETSTEREO");
	if (val ==  CDNR_IF_DETACH)
		return("CDNR_IF_DETACH");
	if (val ==  CDIOCCLOSE)
		return("CDIOCCLOSE");
	if (val ==  BIOCGSEESENT)
		return("BIOCGSEESENT");
	if (val ==  MTIOCRDHPOS)
		return("MTIOCRDHPOS");
	if (val ==  DIOCRTSTADDRS)
		return("DIOCRTSTADDRS");
	if (val ==  JOY_GET_X_OFFSET)
		return("JOY_GET_X_OFFSET");
	if (val ==  TIOCSDRAINWAIT)
		return("TIOCSDRAINWAIT");
	if (val ==  I4B_UPDOWN_IND)
		return("I4B_UPDOWN_IND");
	if (val ==  SERIAL_SETRLOOP)
		return("SERIAL_SETRLOOP");
	if (val ==  I4B_TEL_EMPTYINPUTQUEUE)
		return("I4B_TEL_EMPTYINPUTQUEUE");
	if (val ==  KDENABIO)
		return("KDENABIO");
	if (val ==  GIO_KEYMAP)
		return("GIO_KEYMAP");
	if (val ==  SIOCSIFFLAGS)
		return("SIOCSIFFLAGS");
	if (val ==  SW_VGA_C80x25)
		return("SW_VGA_C80x25");
	if (val ==  SIOCGIFPDSTADDR_IN6)
		return("SIOCGIFPDSTADDR_IN6");
	if (val ==  SERIAL_GETLEVEL)
		return("SERIAL_GETLEVEL");
	if (val ==  AIOSFMT)
		return("AIOSFMT");
	if (val ==  MOUSE_SETVARS)
		return("MOUSE_SETVARS");
	if (val ==  DIOCRGETASTATS)
		return("DIOCRGETASTATS");
	if (val ==  GSC_SRES)
		return("GSC_SRES");
	if (val ==  MDIOCLIST)
		return("MDIOCLIST");
	if (val ==  SW_VGA_C90x60)
		return("SW_VGA_C90x60");
	if (val ==  FSACTL_DELETE_DISK)
		return("FSACTL_DELETE_DISK");
	if (val ==  FW_IBUSRST)
		return("FW_IBUSRST");
	if (val ==  SIOCSIFPHYS)
		return("SIOCSIFPHYS");
	if (val ==  KDSKBSTATE)
		return("KDSKBSTATE");
	if (val ==  I4B_VR_REQ)
		return("I4B_VR_REQ");
	if (val ==  SERIAL_SETMONITOR)
		return("SERIAL_SETMONITOR");
	if (val ==  SW_VGA_MODEX)
		return("SW_VGA_MODEX");
	if (val ==  SNDCTL_TMR_METRONOME)
		return("SNDCTL_TMR_METRONOME");
	if (val ==  PPPIOCSACTIVE)
		return("PPPIOCSACTIVE");
	if (val ==  SMB_WRITEB)
		return("SMB_WRITEB");
	if (val ==  SMB_WRITEW)
		return("SMB_WRITEW");
	if (val ==  BT848_MSP_WRITE)
		return("BT848_MSP_WRITE");
	if (val ==  SNDCTL_DSP_SETDUPLEX)
		return("SNDCTL_DSP_SETDUPLEX");
	if (val ==  AD_NCHANS_GET)
		return("AD_NCHANS_GET");
	if (val ==  CDNR_MOD_TSW)
		return("CDNR_MOD_TSW");
	if (val ==  CDIOCSETVOL)
		return("CDIOCSETVOL");
	if (val ==  TVTUNER_GETCHNL)
		return("TVTUNER_GETCHNL");
	if (val ==  CONS_SAVERSTART)
		return("CONS_SAVERSTART");
	if (val ==  DEVFSIO_RAPPLY)
		return("DEVFSIO_RAPPLY");
	if (val ==  NCPIOC_CONNECT)
		return("NCPIOC_CONNECT");
	if (val ==  SIOCSETFF)
		return("SIOCSETFF");
	if (val ==  PPPIOCGASYNCMAP)
		return("PPPIOCGASYNCMAP");
	if (val ==  ACPIIO_ACAD_GET_STATUS)
		return("ACPIIO_ACAD_GET_STATUS");
	if (val ==  CONS_GETTERM)
		return("CONS_GETTERM");
	if (val ==  FBIO_SETMODE)
		return("FBIO_SETMODE");
	if (val ==  CDNR_MOD_TBM)
		return("CDNR_MOD_TBM");
	if (val ==  MTIOCIEOT)
		return("MTIOCIEOT");
	if (val ==  SNDCTL_DSP_POST)
		return("SNDCTL_DSP_POST");
	if (val ==  GIO_FONT8x16)
		return("GIO_FONT8x16");
	if (val ==  CDNR_MOD_TCM)
		return("CDNR_MOD_TCM");
	if (val ==  BT848_GCAPAREA)
		return("BT848_GCAPAREA");
	if (val ==  VGALOADCHAR)
		return("VGALOADCHAR");
	if (val ==  SIOCSIFMETRIC)
		return("SIOCSIFMETRIC");
	if (val ==  SIOCGLOWAT)
		return("SIOCGLOWAT");
	if (val ==  DIGIIO_MODEL)
		return("DIGIIO_MODEL");
	if (val ==  MOUSE_READDATA)
		return("MOUSE_READDATA");
	if (val ==  SIOCG80211STATS)
		return("SIOCG80211STATS");
	if (val ==  USB_GET_REPORT_DESC)
		return("USB_GET_REPORT_DESC");
	if (val ==  DIOCRGETADDRS)
		return("DIOCRGETADDRS");
	if (val ==  FBIO_GETLINEWIDTH)
		return("FBIO_GETLINEWIDTH");
	if (val ==  STL_BSTART)
		return("STL_BSTART");
	if (val ==  FM_START)
		return("FM_START");
	if (val ==  SIOCGIFBRDADDR)
		return("SIOCGIFBRDADDR");
	if (val ==  BT848_SBTSC)
		return("BT848_SBTSC");
	if (val ==  AUDIO_GETINFO)
		return("AUDIO_GETINFO");
	if (val ==  CDIOCRESUME)
		return("CDIOCRESUME");
	if (val ==  BT848_SCSAT)
		return("BT848_SCSAT");
	if (val ==  BIOCIMMEDIATE)
		return("BIOCIMMEDIATE");
	if (val ==  FSACTL_GET_NEXT_ADAPTER_FIB)
		return("FSACTL_GET_NEXT_ADAPTER_FIB");
	if (val ==  TIOCSBRK)
		return("TIOCSBRK");
	if (val ==  SNDCTL_SYNTH_MEMAVL)
		return("SNDCTL_SYNTH_MEMAVL");
	if (val ==  CTX_H_ORGANIZE)
		return("CTX_H_ORGANIZE");
	if (val ==  PBIO_SETOPACE)
		return("PBIO_SETOPACE");
	if (val ==  VT_RELDISP)
		return("VT_RELDISP");
	if (val ==  SERIAL_SETTIMESLOTS)
		return("SERIAL_SETTIMESLOTS");
	if (val ==  AUDIO_COMPAT_DRAIN)
		return("AUDIO_COMPAT_DRAIN");
	if (val ==  CRIOGET)
		return("CRIOGET");
	if (val ==  SIOCSTPUT)
		return("SIOCSTPUT");
	if (val ==  OSIOCGIFADDR)
		return("OSIOCGIFADDR");
	if (val ==  SIOCAADDRCTL_POLICY)
		return("SIOCAADDRCTL_POLICY");
	if (val ==  SERIAL_SETUNFRAM)
		return("SERIAL_SETUNFRAM");
	if (val ==  G_GATE_CMD_DESTROY)
		return("G_GATE_CMD_DESTROY");
	if (val ==  METEORGBRIG)
		return("METEORGBRIG");
	if (val ==  VMIO_SIOCSLADRF)
		return("VMIO_SIOCSLADRF");
	if (val ==  CDNR_ADD_TBM)
		return("CDNR_ADD_TBM");
	if (val ==  PIO_KEYMAPENT)
		return("PIO_KEYMAPENT");
	if (val ==  SW_VGA11)
		return("SW_VGA11");
	if (val ==  SW_VGA12)
		return("SW_VGA12");
	if (val ==  SW_VGA13)
		return("SW_VGA13");
	if (val ==  PIOCRWFLAG)
		return("PIOCRWFLAG");
	if (val ==  CDNR_ADD_TCM)
		return("CDNR_ADD_TCM");
	if (val ==  SNDCTL_SEQ_NRSYNTHS)
		return("SNDCTL_SEQ_NRSYNTHS");
	if (val ==  BIOCLOCK)
		return("BIOCLOCK");
	if (val ==  CAMIOCOMMAND)
		return("CAMIOCOMMAND");
	if (val ==  TIOCSETD)
		return("TIOCSETD");
	if (val ==  FBIOGATTR)
		return("FBIOGATTR");
	if (val ==  FSACTL_ZERO_FIBTIMES)
		return("FSACTL_ZERO_FIBTIMES");
	if (val ==  SW_TEXT_132x30)
		return("SW_TEXT_132x30");
	if (val ==  VMIO_SIOCPORT)
		return("VMIO_SIOCPORT");
	if (val ==  ASC_SBLEN)
		return("ASC_SBLEN");
	if (val ==  MTIOCSLOCATE)
		return("MTIOCSLOCATE");
	if (val ==  ASC_GRES)
		return("ASC_GRES");
	if (val ==  SIOCGDRVSPEC)
		return("SIOCGDRVSPEC");
	if (val ==  APMIO_NOTHALTCPU)
		return("APMIO_NOTHALTCPU");
	if (val ==  DIOCRADDADDRS)
		return("DIOCRADDADDRS");
	if (val ==  DIOCGFWHEADS)
		return("DIOCGFWHEADS");
	if (val ==  NWFSIOC_GETNS)
		return("NWFSIOC_GETNS");
	if (val ==  SW_VGA_CG320)
		return("SW_VGA_CG320");
	if (val ==  SIOCGIFMAC)
		return("SIOCGIFMAC");
	if (val ==  TARGIOCENABLE)
		return("TARGIOCENABLE");
	if (val ==  FBIOSCURPOS)
		return("FBIOSCURPOS");
	if (val ==  AUDIO_DRAIN)
		return("AUDIO_DRAIN");
	if (val ==  OSIOCGIFNETMASK)
		return("OSIOCGIFNETMASK");
	if (val ==  MTIOCRDSPOS)
		return("MTIOCRDSPOS");
	if (val ==  OTIOCCONS)
		return("OTIOCCONS");
	if (val ==  TIOCSIG)
		return("TIOCSIG");
	if (val ==  KBDGREPSW)
		return("KBDGREPSW");
	if (val ==  G_GATE_CMD_START)
		return("G_GATE_CMD_START");
	if (val ==  BIOCSETIF)
		return("BIOCSETIF");
	if (val ==  SIOCS80211)
		return("SIOCS80211");
	if (val ==  TIOCEXCL)
		return("TIOCEXCL");
	if (val ==  PPISECR)
		return("PPISECR");
	if (val ==  USB_GET_DEVICEINFO)
		return("USB_GET_DEVICEINFO");
	if (val ==  SMB_BREAD)
		return("SMB_BREAD");
	if (val ==  TIOCGETP)
		return("TIOCGETP");
	if (val ==  CTX_V_ORGANIZE)
		return("CTX_V_ORGANIZE");
	if (val ==  SW_ENH_B40x25)
		return("SW_ENH_B40x25");
	if (val ==  SERIAL_SETDXC)
		return("SERIAL_SETDXC");
	if (val ==  DUMPDMA)
		return("DUMPDMA");
	if (val ==  SIOCSWAPA)
		return("SIOCSWAPA");
	if (val ==  SIOCSIFBRDADDR)
		return("SIOCSIFBRDADDR");
	if (val ==  METEORGHUE)
		return("METEORGHUE");
	if (val ==  PPPIOCSMAXCID)
		return("PPPIOCSMAXCID");
	if (val ==  DIOCBEGINADDRS)
		return("DIOCBEGINADDRS");
	if (val ==  I4B_CTRL_INFO_REQ)
		return("I4B_CTRL_INFO_REQ");
	if (val ==  CONS_GETINFO)
		return("CONS_GETINFO");
	if (val ==  METEORGHWS)
		return("METEORGHWS");
	if (val ==  MEMRANGE_SET)
		return("MEMRANGE_SET");
	if (val ==  FSACTL_SIM_IO_TEST)
		return("FSACTL_SIM_IO_TEST");
	if (val ==  FBIO_SETLINEWIDTH)
		return("FBIO_SETLINEWIDTH");
	if (val ==  SIOCDLIFADDR)
		return("SIOCDLIFADDR");
	if (val ==  SIOCATMGVCCS)
		return("SIOCATMGVCCS");
	if (val ==  GSC_GHEIGHT)
		return("GSC_GHEIGHT");
	if (val ==  TIOCNXCL)
		return("TIOCNXCL");
	if (val ==  CDNR_ADD_TSW)
		return("CDNR_ADD_TSW");
	if (val ==  FW_GTPMAP)
		return("FW_GTPMAP");
	if (val ==  SNDCTL_DSP_SETTRIGGER)
		return("SNDCTL_DSP_SETTRIGGER");
	if (val ==  SERIAL_GETRLOOP)
		return("SERIAL_GETRLOOP");
	if (val ==  TVTUNER_SETAFC)
		return("TVTUNER_SETAFC");
	if (val ==  KBDSTPMAT)
		return("KBDSTPMAT");
	if (val ==  DIGIIO_REINIT)
		return("DIGIIO_REINIT");
	if (val ==  PPISSTATUS)
		return("PPISSTATUS");
	if (val ==  SIOCGLIFPHYADDR)
		return("SIOCGLIFPHYADDR");
	if (val ==  DIOCSTARTALTQ)
		return("DIOCSTARTALTQ");
	if (val ==  KDGETREPEAT)
		return("KDGETREPEAT");
	if (val ==  CHIOMOVE)
		return("CHIOMOVE");
	if (val ==  I4B_RBCH_VR_REQ)
		return("I4B_RBCH_VR_REQ");
	if (val ==  OFIOCGET)
		return("OFIOCGET");
	if (val ==  BT848_CCBARS)
		return("BT848_CCBARS");
	if (val ==  KDSKBMODE)
		return("KDSKBMODE");
	if (val ==  BIOCSETWF)
		return("BIOCSETWF");
	if (val ==  SERIAL_GETNAME)
		return("SERIAL_GETNAME");
	if (val ==  CTX_GET_LUT)
		return("CTX_GET_LUT");
	if (val ==  SERIAL_GETVERSIONSTRING)
		return("SERIAL_GETVERSIONSTRING");
	if (val ==  CDNR_GETSTATS)
		return("CDNR_GETSTATS");
	if (val ==  SERIAL_GETREGISTERED)
		return("SERIAL_GETREGISTERED");
	if (val ==  CONS_CURRENTADP)
		return("CONS_CURRENTADP");
	if (val ==  SERIAL_GETTIMESLOTS)
		return("SERIAL_GETTIMESLOTS");
	if (val ==  METEORGSIGNAL)
		return("METEORGSIGNAL");
	if (val ==  DIOCGETALTQS)
		return("DIOCGETALTQS");
	if (val ==  SIOCSIFMAC)
		return("SIOCSIFMAC");
	if (val ==  SIOCGIFALIFETIME_IN6)
		return("SIOCGIFALIFETIME_IN6");
	if (val ==  SIOCGIFDSTADDR_IN6)
		return("SIOCGIFDSTADDR_IN6");
	if (val ==  SIOCGETSGCNT)
		return("SIOCGETSGCNT");
	if (val ==  PPS_IOC_GETCAP)
		return("PPS_IOC_GETCAP");
	if (val ==  APMIO_REJECTLASTREQ)
		return("APMIO_REJECTLASTREQ");
	if (val ==  CDIOREADTOCENTRY)
		return("CDIOREADTOCENTRY");
	if (val ==  VGASETCOLMS)
		return("VGASETCOLMS");
	if (val ==  SIOCSWLPSA)
		return("SIOCSWLPSA");
	if (val ==  PPISCTRL)
		return("PPISCTRL");
	if (val ==  SW_VESA_1024x768)
		return("SW_VESA_1024x768");
	if (val ==  PCIOCATTACHED)
		return("PCIOCATTACHED");
	if (val ==  SNDCTL_DSP_SYNC)
		return("SNDCTL_DSP_SYNC");
	if (val ==  SMAPIOGHEADER)
		return("SMAPIOGHEADER");
	if (val ==  PMIOSTART)
		return("PMIOSTART");
	if (val ==  PPISFIFO)
		return("PPISFIFO");
	if (val ==  DIOCXROLLBACK)
		return("DIOCXROLLBACK");
	if (val ==  AGPIOC_RELEASE)
		return("AGPIOC_RELEASE");
	if (val ==  GREGPROTO)
		return("GREGPROTO");
	if (val ==  PPPIOCSFLAGS)
		return("PPPIOCSFLAGS");
	if (val ==  DEVFSIO_RGETNEXT)
		return("DEVFSIO_RGETNEXT");
	if (val ==  FBIO_SETWINORG)
		return("FBIO_SETWINORG");
	if (val ==  TARGIOCDEBUG)
		return("TARGIOCDEBUG");
	if (val ==  SETFKEY)
		return("SETFKEY");
	if (val ==  SIOCGIFDSTADDR)
		return("SIOCGIFDSTADDR");
	if (val ==  BIOCGRTIMEOUT)
		return("BIOCGRTIMEOUT");
	if (val ==  TAPSDEBUG)
		return("TAPSDEBUG");
	if (val ==  SIOCSIFNAME)
		return("SIOCSIFNAME");
	if (val ==  SERIAL_SETCABLEN)
		return("SERIAL_SETCABLEN");
	if (val ==  USB_GET_FULL_DESC)
		return("USB_GET_FULL_DESC");
	if (val ==  USB_DEVICEINFO)
		return("USB_DEVICEINFO");
	if (val ==  DIOCADDRULE)
		return("DIOCADDRULE");
	if (val ==  SNDCTL_TMR_SELECT)
		return("SNDCTL_TMR_SELECT");
	if (val ==  DIOCSETSTATUSIF)
		return("DIOCSETSTATUSIF");
	if (val ==  SNDCTL_DSP_SUBDIVIDE)
		return("SNDCTL_DSP_SUBDIVIDE");
	if (val ==  SW_VESA_CG1280x1024)
		return("SW_VESA_CG1280x1024");
	if (val ==  BT848SCBUF)
		return("BT848SCBUF");
	if (val ==  SW_TEXT_132x50)
		return("SW_TEXT_132x50");
	if (val ==  JOY_SETTIMEOUT)
		return("JOY_SETTIMEOUT");
	if (val ==  SPKRTUNE)
		return("SPKRTUNE");
	if (val ==  FSACTL_MINIPORT_REV_CHECK)
		return("FSACTL_MINIPORT_REV_CHECK");
	if (val ==  SIOCGWLEEPROM)
		return("SIOCGWLEEPROM");
	if (val ==  SIOCGIFNETMASK_IN6)
		return("SIOCGIFNETMASK_IN6");
	if (val ==  DIOCGFWSECTORS)
		return("DIOCGFWSECTORS");
	if (val ==  METEORGFPS)
		return("METEORGFPS");
	if (val ==  SNDCTL_DSP_SPEED)
		return("SNDCTL_DSP_SPEED");
	if (val ==  KBDSCKEY)
		return("KBDSCKEY");
	if (val ==  TIOCSETAF)
		return("TIOCSETAF");
	if (val ==  PBIO_GETOPACE)
		return("PBIO_GETOPACE");
	if (val ==  FWOHCI_WRREG)
		return("FWOHCI_WRREG");
	if (val ==  SIOCGIFINDEX)
		return("SIOCGIFINDEX");
	if (val ==  STL_BSTOP)
		return("STL_BSTOP");
	if (val ==  FBIOGVIDEO)
		return("FBIOGVIDEO");
	if (val ==  CONS_CLRHIST)
		return("CONS_CLRHIST");
	if (val ==  MTIOCSETEOTMODEL)
		return("MTIOCSETEOTMODEL");
	if (val ==  SIOCSIFMTU)
		return("SIOCSIFMTU");
	if (val ==  SIOCIPFGETNEXT)
		return("SIOCIPFGETNEXT");
	if (val ==  KDSETMODE)
		return("KDSETMODE");
	if (val ==  FBIO_GETPALETTE)
		return("FBIO_GETPALETTE");
	if (val ==  DIOCSETDEBUG)
		return("DIOCSETDEBUG");
	if (val ==  DEVFSIO_RDEL)
		return("DEVFSIO_RDEL");
	if (val ==  CHIOGSTATUS)
		return("CHIOGSTATUS");
	if (val ==  DIOCGETLIMIT)
		return("DIOCGETLIMIT");
	if (val ==  MOUSE_GETSTATUS)
		return("MOUSE_GETSTATUS");
	if (val ==  SNDCTL_SEQ_TESTMIDI)
		return("SNDCTL_SEQ_TESTMIDI");
	if (val ==  SIOCSIFADDR_IN6)
		return("SIOCSIFADDR_IN6");
	if (val ==  SIOCSETLG)
		return("SIOCSETLG");
	if (val ==  MTIOCERRSTAT)
		return("MTIOCERRSTAT");
	if (val ==  TIOCGWINSZ)
		return("TIOCGWINSZ");
	if (val ==  CDIOCSTART)
		return("CDIOCSTART");
	if (val ==  SW_VESA_C132x43)
		return("SW_VESA_C132x43");
	if (val ==  DIOCSDINFO)
		return("DIOCSDINFO");
	if (val ==  SW_VESA_C132x50)
		return("SW_VESA_C132x50");
	if (val ==  USB_DISCOVER)
		return("USB_DISCOVER");
	if (val ==  SW_VGA_M90x50)
		return("SW_VGA_M90x50");
	if (val ==  TIOCDRAIN)
		return("TIOCDRAIN");
	if (val ==  I2CRSTCARD)
		return("I2CRSTCARD");
	if (val ==  DEVFSIO_SAPPLY)
		return("DEVFSIO_SAPPLY");
	if (val ==  DVDIOCREADSTRUCTURE)
		return("DVDIOCREADSTRUCTURE");
	if (val ==  SESIOC_GETOBJSTAT)
		return("SESIOC_GETOBJSTAT");
	if (val ==  SIOCGIFCAP)
		return("SIOCGIFCAP");
	if (val ==  SERIAL_GETIRQ)
		return("SERIAL_GETIRQ");
	if (val ==  SPIGOT_IOPL_ON)
		return("SPIGOT_IOPL_ON");
	if (val ==  PPPIOCGRASYNCMAP)
		return("PPPIOCGRASYNCMAP");
	if (val ==  TIOCEXT)
		return("TIOCEXT");
	if (val ==  TUNSIFMODE)
		return("TUNSIFMODE");
	if (val ==  SW_VESA_32K_800)
		return("SW_VESA_32K_800");
	if (val ==  DIOCRCLRTABLES)
		return("DIOCRCLRTABLES");
	if (val ==  I4B_TEL_GETAUDIOFMT)
		return("I4B_TEL_GETAUDIOFMT");
	if (val ==  SNDCTL_DSP_GETTRIGGER)
		return("SNDCTL_DSP_GETTRIGGER");
	if (val ==  SNDCTL_MIDI_MPUMODE)
		return("SNDCTL_MIDI_MPUMODE");
	if (val ==  CDIOCSETMUTE)
		return("CDIOCSETMUTE");
	if (val ==  OSIOCGIFCONF)
		return("OSIOCGIFCONF");
	if (val ==  TIOCSLTC)
		return("TIOCSLTC");
	if (val ==  PIO_FONT8x16)
		return("PIO_FONT8x16");
	if (val ==  BT848_SLNOTCH)
		return("BT848_SLNOTCH");
	if (val ==  CTX_LIVE)
		return("CTX_LIVE");
	if (val ==  BT848_GBRIG)
		return("BT848_GBRIG");
	if (val ==  DIOCGETADDR)
		return("DIOCGETADDR");
	if (val ==  DIOCGETSTATES)
		return("DIOCGETSTATES");
	if (val ==  DEVFSIO_RAPPLYID)
		return("DEVFSIO_RAPPLYID");
	if (val ==  CDRIOCINITTRACK)
		return("CDRIOCINITTRACK");
	if (val ==  CONS_HISTORY)
		return("CONS_HISTORY");
	if (val ==  FW_CBINDADDR)
		return("FW_CBINDADDR");
	if (val ==  XRPU_IOC_TIMECOUNTING)
		return("XRPU_IOC_TIMECOUNTING");
	if (val ==  I4B_CONNECT_RESP)
		return("I4B_CONNECT_RESP");
	if (val ==  SIOCRMIFR)
		return("SIOCRMIFR");
	if (val ==  TUNGIFINFO)
		return("TUNGIFINFO");
	if (val ==  SIOCSPRISM2DEBUG)
		return("SIOCSPRISM2DEBUG");
	if (val ==  OFIOCFINDDEVICE)
		return("OFIOCFINDDEVICE");
	if (val ==  SIOCIFDESTROY)
		return("SIOCIFDESTROY");
	if (val ==  CONS_GETVERS)
		return("CONS_GETVERS");
	if (val ==  SIOCLOOKUPSTATW)
		return("SIOCLOOKUPSTATW");
	if (val ==  KDGKBSTATE)
		return("KDGKBSTATE");
	if (val ==  FW_SDEUI64)
		return("FW_SDEUI64");
	if (val ==  VT_LOCKSWITCH)
		return("VT_LOCKSWITCH");
	if (val ==  FIODGNAME)
		return("FIODGNAME");
	if (val ==  SNDCTL_SEQ_CTRLRATE)
		return("SNDCTL_SEQ_CTRLRATE");
	if (val ==  METEORSTS)
		return("METEORSTS");
	if (val ==  SIOCGIFPHYS)
		return("SIOCGIFPHYS");
	if (val ==  GSC_SBLEN)
		return("GSC_SBLEN");
	if (val ==  CDRIOCFORMAT)
		return("CDRIOCFORMAT");
	if (val ==  SIOCGPRIVATE_0)
		return("SIOCGPRIVATE_0");
	if (val ==  SIOCDELRT)
		return("SIOCDELRT");
	if (val ==  SERIAL_SETTIMO)
		return("SERIAL_SETTIMO");
	if (val ==  FW_SSTBUF)
		return("FW_SSTBUF");
	if (val ==  SIOCSVH)
		return("SIOCSVH");
	if (val ==  FSACTL_DOWNLOAD)
		return("FSACTL_DOWNLOAD");
	if (val ==  OFIOCGETOPTNODE)
		return("OFIOCGETOPTNODE");
	if (val ==  SIOCGETMIFCNT_IN6)
		return("SIOCGETMIFCNT_IN6");
	if (val ==  KDGETMODE)
		return("KDGETMODE");
	if (val ==  TIOCLGET)
		return("TIOCLGET");
	if (val ==  SIOCRMSCA)
		return("SIOCRMSCA");
	if (val ==  USB_SET_ALTINTERFACE)
		return("USB_SET_ALTINTERFACE");
	if (val ==  BT848_MSP_RESET)
		return("BT848_MSP_RESET");
	if (val ==  SIOCGIFPDSTADDR)
		return("SIOCGIFPDSTADDR");
	if (val ==  CONS_SETTERM)
		return("CONS_SETTERM");
	if (val ==  NCPIOC_CONNSCAN)
		return("NCPIOC_CONNSCAN");
	if (val ==  DIOCRDELTABLES)
		return("DIOCRDELTABLES");
	if (val ==  TIOCSETA)
		return("TIOCSETA");
	if (val ==  TIOCSETP)
		return("TIOCSETP");
	if (val ==  AGPIOC_INFO)
		return("AGPIOC_INFO");
	if (val ==  KBADDKBD)
		return("KBADDKBD");
	if (val ==  SW_EGAMONO80x25)
		return("SW_EGAMONO80x25");
	if (val ==  OPROMNXTPROP)
		return("OPROMNXTPROP");
	if (val ==  PBIO_SETDIFF)
		return("PBIO_SETDIFF");
	if (val ==  TIOCGDRAINWAIT)
		return("TIOCGDRAINWAIT");
	if (val ==  SERIAL_SETPHONY)
		return("SERIAL_SETPHONY");
	if (val ==  FW_SBINDADDR)
		return("FW_SBINDADDR");
	if (val ==  SIOCSIFCAP)
		return("SIOCSIFCAP");
	if (val ==  SNDCTL_COPR_SENDMSG)
		return("SNDCTL_COPR_SENDMSG");
	if (val ==  METEORSHUE)
		return("METEORSHUE");
	if (val ==  SNDCTL_PMGR_IFACE)
		return("SNDCTL_PMGR_IFACE");
	if (val ==  TIOCCDTR)
		return("TIOCCDTR");
	if (val ==  I4B_TRC_RESETA)
		return("I4B_TRC_RESETA");
	if (val ==  BT848_I2CWR)
		return("BT848_I2CWR");
	if (val ==  GIO_FONT8x8)
		return("GIO_FONT8x8");
	if (val ==  TUNGDEBUG)
		return("TUNGDEBUG");
	if (val ==  METEORSHWS)
		return("METEORSHWS");
	if (val ==  G_GATE_CMD_DONE)
		return("G_GATE_CMD_DONE");
	if (val ==  TUNSIFPID)
		return("TUNSIFPID");
	if (val ==  METEORCAPFRM)
		return("METEORCAPFRM");
	if (val ==  AIOSTOP)
		return("AIOSTOP");
	if (val ==  BT848_GAUDIO)
		return("BT848_GAUDIO");
	if (val ==  SIOCSIFDSTADDR)
		return("SIOCSIFDSTADDR");
	if (val ==  FBIO_GETWINORG)
		return("FBIO_GETWINORG");
	if (val ==  SNDCTL_SEQ_RESET)
		return("SNDCTL_SEQ_RESET");
	if (val ==  I4B_CTL_GET_LAPDSTAT)
		return("I4B_CTL_GET_LAPDSTAT");
	if (val ==  METEORSTATUS)
		return("METEORSTATUS");
	if (val ==  SIOCGETPFSYNC)
		return("SIOCGETPFSYNC");
	if (val ==  CDIOCPLAYBLOCKS)
		return("CDIOCPLAYBLOCKS");
	if (val ==  SIOCGLIFADDR)
		return("SIOCGLIFADDR");
	if (val ==  SW_VESA_64K_640)
		return("SW_VESA_64K_640");
	if (val ==  BT848_GPIO_SET_EN)
		return("BT848_GPIO_SET_EN");
	if (val ==  DIOCGETSTATE)
		return("DIOCGETSTATE");
	if (val ==  BT848_GLNOTCH)
		return("BT848_GLNOTCH");
	if (val ==  GRESADDRD)
		return("GRESADDRD");
	if (val ==  CONS_MODEINFO)
		return("CONS_MODEINFO");
	if (val ==  PPPIOCXFERUNIT)
		return("PPPIOCXFERUNIT");
	if (val ==  VT_WAITACTIVE)
		return("VT_WAITACTIVE");
	if (val ==  SMBIOC_SETFLAGS)
		return("SMBIOC_SETFLAGS");
	if (val ==  OFIOCGETPROPLEN)
		return("OFIOCGETPROPLEN");
	if (val ==  GRESADDRS)
		return("GRESADDRS");
	if (val ==  SERIAL_GETPROTO)
		return("SERIAL_GETPROTO");
	if (val ==  SIOCDIFADDR_IN6)
		return("SIOCDIFADDR_IN6");
	if (val ==  SERIAL_SETPORT)
		return("SERIAL_SETPORT");
	if (val ==  PIOCSDRVOLD)
		return("PIOCSDRVOLD");
	if (val ==  SERIAL_SETSCRAMBLER)
		return("SERIAL_SETSCRAMBLER");
	if (val ==  SIOCSIFPREFIX_IN6)
		return("SIOCSIFPREFIX_IN6");
	if (val ==  DIOCRCLRTSTATS)
		return("DIOCRCLRTSTATS");
	if (val ==  KIOCSOUND)
		return("KIOCSOUND");
	if (val ==  SNDCTL_TMR_TEMPO)
		return("SNDCTL_TMR_TEMPO");
	if (val ==  CHIOSPICKER)
		return("CHIOSPICKER");
	if (val ==  CHIOPOSITION)
		return("CHIOPOSITION");
	if (val ==  FM_STOP)
		return("FM_STOP");
	if (val ==  SIOCAIFPREFIX_IN6)
		return("SIOCAIFPREFIX_IN6");
	if (val ==  BIOCSRSIG)
		return("BIOCSRSIG");
	if (val ==  SW_TEXT_80x50)
		return("SW_TEXT_80x50");
	if (val ==  IOCATAREINIT)
		return("IOCATAREINIT");
	if (val ==  APMIO_DISABLE)
		return("APMIO_DISABLE");
	if (val ==  FSACTL_CLOSE_ADAPTER_CONFIG)
		return("FSACTL_CLOSE_ADAPTER_CONFIG");
	if (val ==  SW_BG640x480)
		return("SW_BG640x480");
	if (val ==  GIO_COLOR)
		return("GIO_COLOR");
	if (val ==  BT848_GPIO_GET_DATA)
		return("BT848_GPIO_GET_DATA");
	if (val ==  SIOCGFRST)
		return("SIOCGFRST");
	if (val ==  FW_GDEUI64)
		return("FW_GDEUI64");
	if (val ==  TIIOCGETPARAMS)
		return("TIIOCGETPARAMS");
	if (val ==  AD_MICRO_PERIOD_SET)
		return("AD_MICRO_PERIOD_SET");
	if (val ==  SMBIOC_READ)
		return("SMBIOC_READ");
	if (val ==  DIOCSTOPALTQ)
		return("DIOCSTOPALTQ");
	if (val ==  PBIO_SETIPACE)
		return("PBIO_SETIPACE");
	if (val ==  GSC_GBTIME)
		return("GSC_GBTIME");
	if (val ==  DIOCADDSTATE)
		return("DIOCADDSTATE");
	if (val ==  SERIAL_GETTIMO)
		return("SERIAL_GETTIMO");
	if (val ==  SW_VESA_C132x60)
		return("SW_VESA_C132x60");
	if (val ==  FIONBIO)
		return("FIONBIO");
	if (val ==  DIOCGSECTORSIZE)
		return("DIOCGSECTORSIZE");
	if (val ==  USB_SET_IMMED)
		return("USB_SET_IMMED");
	if (val ==  KDSBORDER)
		return("KDSBORDER");
	if (val ==  PIO_FONT8x14)
		return("PIO_FONT8x14");
	if (val ==  BT848_SBRIG)
		return("BT848_SBRIG");
	if (val ==  SERIAL_GETMASTER)
		return("SERIAL_GETMASTER");
	if (val ==  DIGIIO_SETALTPIN)
		return("DIGIIO_SETALTPIN");
	if (val ==  SIOCSDRVSPEC)
		return("SIOCSDRVSPEC");
	if (val ==  BT848SFMT)
		return("BT848SFMT");
	if (val ==  SW_TEXT_132x25)
		return("SW_TEXT_132x25");
	if (val ==  SOUND_PCM_WRITE_CHANNELS)
		return("SOUND_PCM_WRITE_CHANNELS");
	if (val ==  FSACTL_OPEN_ADAPTER_CONFIG)
		return("FSACTL_OPEN_ADAPTER_CONFIG");
	if (val ==  SW_BG640)
		return("SW_BG640");
	if (val ==  DIOCIGETIFACES)
		return("DIOCIGETIFACES");
	if (val ==  SIOCGNBRINFO_IN6)
		return("SIOCGNBRINFO_IN6");
	if (val ==  SW_TEXT_132x43)
		return("SW_TEXT_132x43");
	if (val ==  SERIAL_GETINVRCLK)
		return("SERIAL_GETINVRCLK");
	if (val ==  PPPIOCSCOMPRESS)
		return("PPPIOCSCOMPRESS");
	if (val ==  FBIOGTYPE)
		return("FBIOGTYPE");
	if (val ==  KDDISABIO)
		return("KDDISABIO");
	if (val ==  SERIAL_GETSUBCHAN)
		return("SERIAL_GETSUBCHAN");
	if (val ==  KBDSLOCK)
		return("KBDSLOCK");
	if (val ==  SNDCTL_TMR_START)
		return("SNDCTL_TMR_START");
	if (val ==  SNDCTL_SEQ_SYNC)
		return("SNDCTL_SEQ_SYNC");
	if (val ==  SIOCAUTHR)
		return("SIOCAUTHR");
	if (val ==  SERIAL_SETINVCLK)
		return("SERIAL_SETINVCLK");
	if (val ==  CDNR_DEL_FILTER)
		return("CDNR_DEL_FILTER");
	if (val ==  FIOASYNC)
		return("FIOASYNC");
	if (val ==  PPS_IOC_KCBIND)
		return("PPS_IOC_KCBIND");
	if (val ==  CIOCKEY)
		return("CIOCKEY");
	if (val ==  SIOCIPFFB)
		return("SIOCIPFFB");
	if (val ==  SIOCIPFFL)
		return("SIOCIPFFL");
	if (val ==  AD_SUPPORTED_GAINS)
		return("AD_SUPPORTED_GAINS");
	if (val ==  SW_VGA_C90x30)
		return("SW_VGA_C90x30");
	if (val ==  PIOCSTATUS)
		return("PIOCSTATUS");
	if (val ==  SMBIOC_LOOKUP)
		return("SMBIOC_LOOKUP");
	if (val ==  SW_VGA_C90x43)
		return("SW_VGA_C90x43");
	if (val ==  SERIAL_GETSCRAMBLER)
		return("SERIAL_GETSCRAMBLER");
	if (val ==  SNDCTL_DSP_RESET)
		return("SNDCTL_DSP_RESET");
	if (val ==  SW_VGA_C90x50)
		return("SW_VGA_C90x50");
	if (val ==  SERIAL_SETDPLL)
		return("SERIAL_SETDPLL");
	if (val ==  FSACTL_NULL_IO_TEST)
		return("FSACTL_NULL_IO_TEST");
	if (val ==  CDIOCCAPABILITY)
		return("CDIOCCAPABILITY");
	if (val ==  DIOCGETALTQ)
		return("DIOCGETALTQ");
	if (val ==  SIOCGDRLST_IN6)
		return("SIOCGDRLST_IN6");
	if (val ==  FD_GTYPE)
		return("FD_GTYPE");
	if (val ==  MTIOCEEOT)
		return("MTIOCEEOT");
	if (val ==  KDGKBINFO)
		return("KDGKBINFO");
	if (val ==  SIOCIPFL6)
		return("SIOCIPFL6");
	if (val ==  SESIOC_INIT)
		return("SESIOC_INIT");
	if (val ==  CHIOGPICKER)
		return("CHIOGPICKER");
	if (val ==  JOY_GETTIMEOUT)
		return("JOY_GETTIMEOUT");
	if (val ==  BT848_WEEPROM)
		return("BT848_WEEPROM");
	if (val ==  SPKRTONE)
		return("SPKRTONE");
	if (val ==  KDSETRAD)
		return("KDSETRAD");
	if (val ==  I4B_CTL_GET_DEBUG)
		return("I4B_CTL_GET_DEBUG");
	if (val ==  DIOCXCOMMIT)
		return("DIOCXCOMMIT");
	if (val ==  FBIO_SETPALETTE)
		return("FBIO_SETPALETTE");
	if (val ==  SIOCLOOKUPADDNODE)
		return("SIOCLOOKUPADDNODE");
	if (val ==  BT848_MSP_READ)
		return("BT848_MSP_READ");
	if (val ==  SERIAL_GETCFG)
		return("SERIAL_GETCFG");
	if (val ==  TVTUNER_SETTYPE)
		return("TVTUNER_SETTYPE");
	if (val ==  DIOCCLRSRCNODES)
		return("DIOCCLRSRCNODES");
	if (val ==  PIOCCONT)
		return("PIOCCONT");
	if (val ==  IOCATAREQUEST)
		return("IOCATAREQUEST");
	if (val ==  PIOCSDRV)
		return("PIOCSDRV");
	if (val ==  CDRIOCBLANK)
		return("CDRIOCBLANK");
	if (val ==  VGAREADPEL)
		return("VGAREADPEL");
	if (val ==  CDIOREADTOCHEADER)
		return("CDIOREADTOCHEADER");
	if (val ==  CONS_SETKBD)
		return("CONS_SETKBD");
	if (val ==  IOCATADETACH)
		return("IOCATADETACH");
	if (val ==  ACPIIO_BATT_GET_BIF)
		return("ACPIIO_BATT_GET_BIF");
	if (val ==  DIGIIO_DEBUG)
		return("DIGIIO_DEBUG");
	if (val ==  SERIAL_GETCLK)
		return("SERIAL_GETCLK");
	if (val ==  BIOCPROMISC)
		return("BIOCPROMISC");
	if (val ==  PPS_IOC_GETPARAMS)
		return("PPS_IOC_GETPARAMS");
	if (val ==  CDIOCPLAYTRACKS)
		return("CDIOCPLAYTRACKS");
	if (val ==  CDIOCEJECT)
		return("CDIOCEJECT");
	if (val ==  SESIOC_SETOBJSTAT)
		return("SESIOC_SETOBJSTAT");
	if (val ==  ALT_READ_TG_REG)
		return("ALT_READ_TG_REG");
	if (val ==  PPPIOCSRASYNCMAP)
		return("PPPIOCSRASYNCMAP");
	if (val ==  SMAPIOCGFUNCTION)
		return("SMAPIOCGFUNCTION");
	if (val ==  SLIOCSKEEPAL)
		return("SLIOCSKEEPAL");
	if (val ==  SW_VGA_C80x30)
		return("SW_VGA_C80x30");
	if (val ==  SIOCGETLG)
		return("SIOCGETLG");
	if (val ==  DIOCICLRISTATS)
		return("DIOCICLRISTATS");
	if (val ==  SW_PC98_80x25)
		return("SW_PC98_80x25");
	if (val ==  CDIOCSETMONO)
		return("CDIOCSETMONO");
	if (val ==  SLIOCGUNIT)
		return("SLIOCGUNIT");
	if (val ==  DIOCGIFSPEED)
		return("DIOCGIFSPEED");
	if (val ==  SW_VGA_C80x50)
		return("SW_VGA_C80x50");
	if (val ==  SMBIOC_T2RQ)
		return("SMBIOC_T2RQ");
	if (val ==  SNDCTL_TMR_TIMEBASE)
		return("SNDCTL_TMR_TIMEBASE");
	if (val ==  PIO_SCRNMAP)
		return("PIO_SCRNMAP");
	if (val ==  CDIOCSETSTERIO)
		return("CDIOCSETSTERIO");
	if (val ==  CDNR_DEL_ELEM)
		return("CDNR_DEL_ELEM");
	if (val ==  SNDCTL_COPR_HALT)
		return("SNDCTL_COPR_HALT");
	if (val ==  SIOCAIFADDR)
		return("SIOCAIFADDR");
	if (val ==  FBIO_BLANK)
		return("FBIO_BLANK");
	if (val ==  SW_MCAMODE)
		return("SW_MCAMODE");
	if (val ==  MTIOCGETEOTMODEL)
		return("MTIOCGETEOTMODEL");
	if (val ==  CDRIOCINITWRITER)
		return("CDRIOCINITWRITER");
	if (val ==  FBIO_SETDISPSTART)
		return("FBIO_SETDISPSTART");
	if (val ==  PIOCGREG)
		return("PIOCGREG");
	if (val ==  METEORGCSAT)
		return("METEORGCSAT");
	if (val ==  ACPIIO_BATT_GET_BST)
		return("ACPIIO_BATT_GET_BST");
	if (val ==  SIOCSRTRFLUSH_IN6)
		return("SIOCSRTRFLUSH_IN6");
	if (val ==  DIOCRSETTFLAGS)
		return("DIOCRSETTFLAGS");
	if (val ==  GIO_ATTR)
		return("GIO_ATTR");
	if (val ==  SIOCINIFR)
		return("SIOCINIFR");
	if (val ==  TIIOCSETPARAMS)
		return("TIIOCSETPARAMS");
	if (val ==  SMB_PCALL)
		return("SMB_PCALL");
	if (val ==  SIOCGIFSTAT_ICMP6)
		return("SIOCGIFSTAT_ICMP6");
	if (val ==  METEORSETGEO)
		return("METEORSETGEO");
	if (val ==  PPIGEPPA)
		return("PPIGEPPA");
	if (val ==  FBIO_ADAPTER)
		return("FBIO_ADAPTER");
	if (val ==  APMIO_GETINFO_OLD)
		return("APMIO_GETINFO_OLD");
	if (val ==  SIOCGETFS)
		return("SIOCGETFS");
	if (val ==  SIOCGIFPREFIX_IN6)
		return("SIOCGIFPREFIX_IN6");
	if (val ==  OFIOCNEXTPROP)
		return("OFIOCNEXTPROP");
	if (val ==  SIOCGNATL)
		return("SIOCGNATL");
	if (val ==  FM_GET_FREQ)
		return("FM_GET_FREQ");
	if (val ==  METEORSVIDEO)
		return("METEORSVIDEO");
	if (val ==  PBIO_GETIPACE)
		return("PBIO_GETIPACE");
	if (val ==  USB_SET_TIMEOUT)
		return("USB_SET_TIMEOUT");
	if (val ==  KBDGLEDS)
		return("KBDGLEDS");
	if (val ==  SIOCIFGCLONERS)
		return("SIOCIFGCLONERS");
	if (val ==  SIOCGSCOPE6DEF)
		return("SIOCGSCOPE6DEF");
	if (val ==  SERIAL_GETDPLL)
		return("SERIAL_GETDPLL");
	if (val ==  AIOGSIZE)
		return("AIOGSIZE");
	if (val ==  DIGIIO_IDENT)
		return("DIGIIO_IDENT");
	if (val ==  SERIAL_ADDDLCI)
		return("SERIAL_ADDDLCI");
	if (val ==  SMB_SENDB)
		return("SMB_SENDB");
	if (val ==  FIODTYPE)
		return("FIODTYPE");
	if (val ==  SIOCGETSGCNT_IN6)
		return("SIOCGETSGCNT_IN6");
	if (val ==  CDRIOCNEXTWRITEABLEADDR)
		return("CDRIOCNEXTWRITEABLEADDR");
	if (val ==  BT848_SHUE)
		return("BT848_SHUE");
	if (val ==  CHIOEXCHANGE)
		return("CHIOEXCHANGE");
	if (val ==  AIOCINFO)
		return("AIOCINFO");
	if (val ==  SW_VGA_C90x25)
		return("SW_VGA_C90x25");
	if (val ==  NWFSIOC_GETEINFO)
		return("NWFSIOC_GETEINFO");
	if (val ==  FBIO_ADPTYPE)
		return("FBIO_ADPTYPE");
	if (val ==  IOCATARAIDCREATE)
		return("IOCATARAIDCREATE");
	if (val ==  SIOCDADDRCTL_POLICY)
		return("SIOCDADDRCTL_POLICY");
	if (val ==  OSIOCGIFDSTADDR)
		return("OSIOCGIFDSTADDR");
	if (val ==  SIOCGHIWAT)
		return("SIOCGHIWAT");
	if (val ==  SIOCGIFSTAT_IN6)
		return("SIOCGIFSTAT_IN6");
	if (val ==  DIOCCLRIFFLAG)
		return("DIOCCLRIFFLAG");
	if (val ==  SIOCADDMULTI)
		return("SIOCADDMULTI");
	if (val ==  ASC_SRESSW)
		return("ASC_SRESSW");
	if (val ==  SIOCGWLCITEM)
		return("SIOCGWLCITEM");
	if (val ==  SW_VESA_32K_1024)
		return("SW_VESA_32K_1024");
	if (val ==  SERIAL_SETCRC4)
		return("SERIAL_SETCRC4");
	if (val ==  TVTUNER_GETTYPE)
		return("TVTUNER_GETTYPE");
	if (val ==  CDIOCGETVOL)
		return("CDIOCGETVOL");
	if (val ==  SNDCTL_SEQ_PERCMODE)
		return("SNDCTL_SEQ_PERCMODE");
	if (val ==  OPROMNEXT)
		return("OPROMNEXT");
	if (val ==  SERIAL_GETPORT)
		return("SERIAL_GETPORT");
	if (val ==  FW_ASYREQ)
		return("FW_ASYREQ");
	if (val ==  I4B_ACTIVE_DIAGNOSTIC)
		return("I4B_ACTIVE_DIAGNOSTIC");
	if (val ==  CONS_BLANKTIME)
		return("CONS_BLANKTIME");
	if (val ==  PPIGEPPD)
		return("PPIGEPPD");
	if (val ==  SNDCTL_DSP_MAPOUTBUF)
		return("SNDCTL_DSP_MAPOUTBUF");
	if (val ==  VGACURSOR)
		return("VGACURSOR");
	if (val ==  I4B_TRC_SET)
		return("I4B_TRC_SET");
	if (val ==  CDIOCREADSUBCHANNEL_SYSSPACE)
		return("CDIOCREADSUBCHANNEL_SYSSPACE");
	if (val ==  SW_VGA_M90x30)
		return("SW_VGA_M90x30");
	if (val ==  PIOCGSTATE)
		return("PIOCGSTATE");
	if (val ==  BT848_GPIO_SET_DATA)
		return("BT848_GPIO_SET_DATA");
	if (val ==  METEORSCONT)
		return("METEORSCONT");
	if (val ==  USB_SET_SHORT_XFER)
		return("USB_SET_SHORT_XFER");
	if (val ==  PIOCGIO)
		return("PIOCGIO");
	if (val ==  PTIOCSETTIMEOUT)
		return("PTIOCSETTIMEOUT");
	if (val ==  SIOCFUNCL)
		return("SIOCFUNCL");
	if (val ==  SNDCTL_SEQ_NRMIDIS)
		return("SNDCTL_SEQ_NRMIDIS");
	if (val ==  CHIOSETVOLTAG)
		return("CHIOSETVOLTAG");
	if (val ==  SIOCATMCLOSEVCC)
		return("SIOCATMCLOSEVCC");
	if (val ==  SIOCSIFMEDIA)
		return("SIOCSIFMEDIA");
	if (val ==  SERIAL_SETNRZI)
		return("SERIAL_SETNRZI");
	if (val ==  SERIAL_GETE3STAT)
		return("SERIAL_GETE3STAT");
	if (val ==  BT848_GSTATUS)
		return("BT848_GSTATUS");
	if (val ==  VMIO_SIOCSIFBR)
		return("VMIO_SIOCSIFBR");
	if (val ==  BIOCGHDRCMPLT)
		return("BIOCGHDRCMPLT");
	if (val ==  PIOCSFL)
		return("PIOCSFL");
	if (val ==  I4B_ALERT_REQ)
		return("I4B_ALERT_REQ");
	if (val ==  TIOCSTOP)
		return("TIOCSTOP");
	if (val ==  SLIOCGKEEPAL)
		return("SLIOCGKEEPAL");
	if (val ==  SIOCALIFADDR)
		return("SIOCALIFADDR");
	if (val ==  IOCATARAIDSTATUS)
		return("IOCATARAIDSTATUS");
	if (val ==  PPPIOCGXASYNCMAP)
		return("PPPIOCGXASYNCMAP");
	if (val ==  GSC_SHEIGHT)
		return("GSC_SHEIGHT");
	if (val ==  BT848_SCAPAREA)
		return("BT848_SCAPAREA");
	if (val ==  SNDCTL_TMR_STOP)
		return("SNDCTL_TMR_STOP");
	if (val ==  AGPIOC_BIND)
		return("AGPIOC_BIND");
	if (val ==  METEORSINPUT)
		return("METEORSINPUT");
	if (val ==  SNDCTL_SEQ_THRESHOLD)
		return("SNDCTL_SEQ_THRESHOLD");
	if (val ==  SNDCTL_SEQ_OUTOFBAND)
		return("SNDCTL_SEQ_OUTOFBAND");
	if (val ==  TIOCGPGRP)
		return("TIOCGPGRP");
	if (val ==  COM_CLRPORTSTATS)
		return("COM_CLRPORTSTATS");
	if (val ==  SIOCAUTHW)
		return("SIOCAUTHW");
	if (val ==  AACIO_STATS)
		return("AACIO_STATS");
	if (val ==  METEORSCHCV)
		return("METEORSCHCV");
	if (val ==  BIOCGDLTLIST)
		return("BIOCGDLTLIST");
	if (val ==  CDIOCSETRIGHT)
		return("CDIOCSETRIGHT");
	if (val ==  SERIAL_SETCLK)
		return("SERIAL_SETCLK");
	if (val ==  LPT_IRQ)
		return("LPT_IRQ");
	if (val ==  SNDCTL_COPR_WCODE)
		return("SNDCTL_COPR_WCODE");
	if (val ==  SW_VESA_CG1024x768)
		return("SW_VESA_CG1024x768");
	if (val ==  DIOCSPC98)
		return("DIOCSPC98");
	if (val ==  SNDCTL_SEQ_GETTIME)
		return("SNDCTL_SEQ_GETTIME");
	if (val ==  NWFSIOC_GETCONN)
		return("NWFSIOC_GETCONN");
	if (val ==  PCIOCWRITE)
		return("PCIOCWRITE");
	if (val ==  FD_GSTAT)
		return("FD_GSTAT");
	if (val ==  CONS_BELLTYPE)
		return("CONS_BELLTYPE");
	if (val ==  AIOGCAP)
		return("AIOGCAP");
	if (val ==  SIOCGPPPCSTATS)
		return("SIOCGPPPCSTATS");
	if (val ==  TIOCNOTTY)
		return("TIOCNOTTY");
	if (val ==  CONS_MOUSECTL)
		return("CONS_MOUSECTL");
	if (val ==  SW_VGA_CG640)
		return("SW_VGA_CG640");
	if (val ==  OTIOCGETD)
		return("OTIOCGETD");
	if (val ==  USB_GET_DEVICE_DESC)
		return("USB_GET_DEVICE_DESC");
	if (val ==  DIOCOSFPGET)
		return("DIOCOSFPGET");
	if (val ==  I4B_PROT_IND)
		return("I4B_PROT_IND");
	if (val ==  SNDCTL_DSP_SETSYNCRO)
		return("SNDCTL_DSP_SETSYNCRO");
	if (val ==  FBIOVERTICAL)
		return("FBIOVERTICAL");
	if (val ==  SW_PC98_EGC640x400)
		return("SW_PC98_EGC640x400");
	if (val ==  METEORGACTPIXFMT)
		return("METEORGACTPIXFMT");
	if (val ==  BT848_GUSAT)
		return("BT848_GUSAT");
	if (val ==  VMIO_SIOCBRIDGE)
		return("VMIO_SIOCBRIDGE");
	if (val ==  SW_VESA_32K_640)
		return("SW_VESA_32K_640");
	if (val ==  USB_GET_ALTINTERFACE)
		return("USB_GET_ALTINTERFACE");
	if (val ==  MDIOCATTACH)
		return("MDIOCATTACH");
	if (val ==  VGAPCVTID)
		return("VGAPCVTID");
	if (val ==  USB_DO_REQUEST)
		return("USB_DO_REQUEST");
	if (val ==  BIOCSBLEN)
		return("BIOCSBLEN");
	if (val ==  CAMGETPASSTHRU)
		return("CAMGETPASSTHRU");
	if (val ==  TIOCSCTTY)
		return("TIOCSCTTY");
	if (val ==  APMIO_DISPLAY)
		return("APMIO_DISPLAY");
	if (val ==  CDIOCREADSUBCHANNEL)
		return("CDIOCREADSUBCHANNEL");
	if (val ==  SMBIOC_OPENSHARE)
		return("SMBIOC_OPENSHARE");
	if (val ==  BIOCSHDRCMPLT)
		return("BIOCSHDRCMPLT");
	if (val ==  ALT_WRITE_TG_REG)
		return("ALT_WRITE_TG_REG");
	if (val ==  SIOCPHASE2)
		return("SIOCPHASE2");
	if (val ==  PIOCGFL)
		return("PIOCGFL");
	if (val ==  AIOCADD)
		return("AIOCADD");
	if (val ==  FIOGETOWN)
		return("FIOGETOWN");
	if (val ==  GETFKEY)
		return("GETFKEY");
	if (val ==  NCPIOC_REQUEST)
		return("NCPIOC_REQUEST");
	if (val ==  SMB_BWRITE)
		return("SMB_BWRITE");
	if (val ==  I2CREAD)
		return("I2CREAD");
	if (val ==  TIOCSETC)
		return("TIOCSETC");
	if (val ==  DIOCGETTIMEOUT)
		return("DIOCGETTIMEOUT");
	if (val ==  USB_GET_CM_OVER_DATA)
		return("USB_GET_CM_OVER_DATA");
	if (val ==  AGPIOC_ACQUIRE)
		return("AGPIOC_ACQUIRE");
	if (val ==  SLIOCSOUTFILL)
		return("SLIOCSOUTFILL");
	if (val ==  MDIOCDETACH)
		return("MDIOCDETACH");
	if (val ==  GSC_SWIDTH)
		return("GSC_SWIDTH");
	if (val ==  FBIOGXINFO)
		return("FBIOGXINFO");
	if (val ==  CDRIOCGETPROGRESS)
		return("CDRIOCGETPROGRESS");
	if (val ==  TIOCGETA)
		return("TIOCGETA");
	if (val ==  SW_TEXT_132x60)
		return("SW_TEXT_132x60");
	if (val ==  SNDCTL_MIDI_INFO)
		return("SNDCTL_MIDI_INFO");
	if (val ==  DIOCOSFPFLUSH)
		return("DIOCOSFPFLUSH");
	if (val ==  SERIAL_GETPHONY)
		return("SERIAL_GETPHONY");
	if (val ==  DIOCGETADDRS)
		return("DIOCGETADDRS");
	if (val ==  ALT_WRITE_TG_MEM)
		return("ALT_WRITE_TG_MEM");
	if (val ==  TIOCGETC)
		return("TIOCGETC");
	if (val ==  BT848GFMT)
		return("BT848GFMT");
	if (val ==  PPPIOCSPASS)
		return("PPPIOCSPASS");
	if (val ==  SNDCTL_DSP_GETIPTR)
		return("SNDCTL_DSP_GETIPTR");
	if (val ==  SW_VESA_FULL_320)
		return("SW_VESA_FULL_320");
	if (val ==  AD_GAINS_SET)
		return("AD_GAINS_SET");
	if (val ==  SMB_RECVB)
		return("SMB_RECVB");
	if (val ==  ASC_SHEIGHT)
		return("ASC_SHEIGHT");
	if (val ==  SERIAL_GETNRZI)
		return("SERIAL_GETNRZI");
	if (val ==  SERIAL_SETINVRCLK)
		return("SERIAL_SETINVRCLK");
	if (val ==  FSACTL_QUERY_ADAPTER_CONFIG)
		return("FSACTL_QUERY_ADAPTER_CONFIG");
	if (val ==  SIOCRMAFR)
		return("SIOCRMAFR");
	if (val ==  SERIAL_SETSUBCHAN)
		return("SERIAL_SETSUBCHAN");
	if (val ==  KDMKTONE)
		return("KDMKTONE");
	if (val ==  FW_SRSTREAM)
		return("FW_SRSTREAM");
	if (val ==  NGIOCSETNAME)
		return("NGIOCSETNAME");
	if (val ==  FBIO_GETMODE)
		return("FBIO_GETMODE");
	if (val ==  DIOCSETIFFLAG)
		return("DIOCSETIFFLAG");
	if (val ==  SIOCSTLCK)
		return("SIOCSTLCK");
	if (val ==  SERIAL_GETDIR)
		return("SERIAL_GETDIR");
	if (val ==  RIO_SEND_COMMAND)
		return("RIO_SEND_COMMAND");
	if (val ==  FW_GCROM)
		return("FW_GCROM");
	if (val ==  SNDCTL_SEQ_RESETSAMPLES)
		return("SNDCTL_SEQ_RESETSAMPLES");
	if (val ==  SERIAL_GETESTAT)
		return("SERIAL_GETESTAT");
	if (val ==  APMIO_GETINFO)
		return("APMIO_GETINFO");
	if (val ==  DIOCGETRULESET)
		return("DIOCGETRULESET");
	if (val ==  DEVFSIO_SGETNEXT)
		return("DEVFSIO_SGETNEXT");
	if (val ==  SIOCSIFPHYADDR)
		return("SIOCSIFPHYADDR");
	if (val ==  SIOCATMARK)
		return("SIOCATMARK");
	if (val ==  METEORSCOUNT)
		return("METEORSCOUNT");
	if (val ==  CTX_GRAB)
		return("CTX_GRAB");
	if (val ==  FBIOSCURSOR)
		return("FBIOSCURSOR");
	if (val ==  USB_GET_ENDPOINT_DESC)
		return("USB_GET_ENDPOINT_DESC");
	if (val ==  BT848SCLIP)
		return("BT848SCLIP");
	if (val ==  SERIAL_SETKEEPALIVE)
		return("SERIAL_SETKEEPALIVE");
	if (val ==  BT848_GCSAT)
		return("BT848_GCSAT");
	if (val ==  FD_CLRERR)
		return("FD_CLRERR");
	if (val ==  SW_ENH_C80x43)
		return("SW_ENH_C80x43");
	if (val ==  TIOCSTART)
		return("TIOCSTART");
	if (val ==  SNDCTL_PMGR_ACCESS)
		return("SNDCTL_PMGR_ACCESS");
	if (val ==  SW_VGA_C40x25)
		return("SW_VGA_C40x25");
	if (val ==  SW_VESA_1280x1024)
		return("SW_VESA_1280x1024");
	if (val ==  PPISEPPD)
		return("PPISEPPD");
	if (val ==  TIOCHPCL)
		return("TIOCHPCL");
	if (val ==  PPPIOCSASYNCMAP)
		return("PPPIOCSASYNCMAP");
	if (val ==  SIOCG80211)
		return("SIOCG80211");
	if (val ==  SW_VESA_32K_320)
		return("SW_VESA_32K_320");
	if (val ==  SESIOC_GETTEXT)
		return("SESIOC_GETTEXT");
	if (val ==  CDNR_ADD_ELEM)
		return("CDNR_ADD_ELEM");
	if (val ==  CONS_SAVERMODE)
		return("CONS_SAVERMODE");
	if (val ==  MIOSPASSTHRU)
		return("MIOSPASSTHRU");
	if (val ==  CONS_GET)
		return("CONS_GET");
	if (val ==  BT848_SAUDIO)
		return("BT848_SAUDIO");
	if (val ==  SW_B40x25)
		return("SW_B40x25");
	if (val ==  SERIAL_SETDIR)
		return("SERIAL_SETDIR");
	if (val ==  SERIAL_GETDXC)
		return("SERIAL_GETDXC");
	if (val ==  DIOCGETSRCNODES)
		return("DIOCGETSRCNODES");
	if (val ==  SNDCTL_SEQ_GETINCOUNT)
		return("SNDCTL_SEQ_GETINCOUNT");
	if (val ==  METEORGSUPPIXFMT)
		return("METEORGSUPPIXFMT");
	if (val ==  ASC_GBLEN)
		return("ASC_GBLEN");
	if (val ==  CONS_CURSORTYPE)
		return("CONS_CURSORTYPE");
	if (val ==  IOCATARAIDADDSPARE)
		return("IOCATARAIDADDSPARE");
	if (val ==  SW_VESA_64K_800)
		return("SW_VESA_64K_800");
	if (val ==  AD_MICRO_PERIOD_GET)
		return("AD_MICRO_PERIOD_GET");
	if (val ==  PCIOCGETCONF)
		return("PCIOCGETCONF");
	if (val ==  I4B_CTL_CLR_CHIPSTAT)
		return("I4B_CTL_CLR_CHIPSTAT");
	if (val ==  SERIAL_CLRSTAT)
		return("SERIAL_CLRSTAT");
	if (val ==  PIOCGMEM)
		return("PIOCGMEM");
	if (val ==  SNDCTL_COPR_RESET)
		return("SNDCTL_COPR_RESET");
	if (val ==  SERIAL_GETHIGAIN)
		return("SERIAL_GETHIGAIN");
	if (val ==  SIOCGNATS)
		return("SIOCGNATS");
	if (val ==  BIOCGETIF)
		return("BIOCGETIF");
	if (val ==  MTIOCTOP)
		return("MTIOCTOP");
	if (val ==  TVTUNER_GETAFC)
		return("TVTUNER_GETAFC");
	if (val ==  KBRELKBD)
		return("KBRELKBD");
	if (val ==  FBIO_GETDISPSTART)
		return("FBIO_GETDISPSTART");
	if (val ==  KBDGTPMAT)
		return("KBDGTPMAT");
	if (val ==  CONS_IDLE)
		return("CONS_IDLE");
	if (val ==  DIOCRCLRADDRS)
		return("DIOCRCLRADDRS");
	if (val ==  SIOCSSCOPE6)
		return("SIOCSSCOPE6");
	if (val ==  JOY_SET_Y_OFFSET)
		return("JOY_SET_Y_OFFSET");
	if (val ==  CDIOCPLAYMSF)
		return("CDIOCPLAYMSF");
	if (val ==  BT848_SCONT)
		return("BT848_SCONT");
	if (val ==  CDIOREADTOCENTRYS)
		return("CDIOREADTOCENTRYS");
	if (val ==  VGAWRITEPEL)
		return("VGAWRITEPEL");
	if (val ==  TIOCGLTC)
		return("TIOCGLTC");
	if (val ==  SIOCRMNAT)
		return("SIOCRMNAT");
	if (val ==  APMIO_SUSPEND)
		return("APMIO_SUSPEND");
	if (val ==  SNDCTL_SYNTH_INFO)
		return("SNDCTL_SYNTH_INFO");
	if (val ==  METEORSVWS)
		return("METEORSVWS");
	if (val ==  FD_SOPTS)
		return("FD_SOPTS");
	if (val ==  CIOCASYMFEAT)
		return("CIOCASYMFEAT");
	if (val ==  FBIO_FINDMODE)
		return("FBIO_FINDMODE");
	if (val ==  SIOCGSCST)
		return("SIOCGSCST");
	if (val ==  IOCATARAIDDELETE)
		return("IOCATARAIDDELETE");
	if (val ==  MOUSE_SETLEVEL)
		return("MOUSE_SETLEVEL");
	if (val ==  USB_GET_CONFIG)
		return("USB_GET_CONFIG");
	if (val ==  SW_VESA_800x600)
		return("SW_VESA_800x600");
	if (val ==  KDSETREPEAT)
		return("KDSETREPEAT");
	if (val ==  SNDCTL_MIDI_MPUCMD)
		return("SNDCTL_MIDI_MPUCMD");
	if (val ==  PPIGDATA)
		return("PPIGDATA");
	if (val ==  SPIGOT_IOPL_OFF)
		return("SPIGOT_IOPL_OFF");
	if (val ==  PIO_FONT8x8)
		return("PIO_FONT8x8");
	if (val ==  TIOCLBIC)
		return("TIOCLBIC");
	if (val ==  SIOCGWLPSA)
		return("SIOCGWLPSA");
	if (val ==  FBIOMONINFO)
		return("FBIOMONINFO");
	if (val ==  FBIO_ADPINFO)
		return("FBIO_ADPINFO");
	if (val ==  SIOCADSCA)
		return("SIOCADSCA");
	if (val ==  SIOCFRZST)
		return("SIOCFRZST");
	if (val ==  SIOCGPGRP)
		return("SIOCGPGRP");
	if (val ==  TIOCLBIS)
		return("TIOCLBIS");
	if (val ==  PPPIOCGMRU)
		return("PPPIOCGMRU");
	if (val ==  SIOCGIFSTATUS)
		return("SIOCGIFSTATUS");
	if (val ==  SLIOCGOUTFILL)
		return("SLIOCGOUTFILL");
	if (val ==  SNDCTL_FM_LOAD_INSTR)
		return("SNDCTL_FM_LOAD_INSTR");
	if (val ==  FIOSETOWN)
		return("FIOSETOWN");
	if (val ==  SNDCTL_COPR_RUN)
		return("SNDCTL_COPR_RUN");
	if (val ==  MDIOCQUERY)
		return("MDIOCQUERY");
	if (val ==  TIOCSWINSZ)
		return("TIOCSWINSZ");
	if (val ==  SIOCGIFGENERIC)
		return("SIOCGIFGENERIC");
	if (val ==  TIOCSTI)
		return("TIOCSTI");
	if (val ==  DIOCCHANGEADDR)
		return("DIOCCHANGEADDR");
	if (val ==  TAPGDEBUG)
		return("TAPGDEBUG");
	if (val ==  SW_ENH_C80x25)
		return("SW_ENH_C80x25");
	if (val ==  METEORSSIGNAL)
		return("METEORSSIGNAL");
	if (val ==  FBIO_MODEINFO)
		return("FBIO_MODEINFO");
	if (val ==  TIOCSETN)
		return("TIOCSETN");
	if (val ==  I4B_TEL_TONES)
		return("I4B_TEL_TONES");
	if (val ==  STL_BINTR)
		return("STL_BINTR");
	if (val ==  CIOCCRYPT)
		return("CIOCCRYPT");
	if (val ==  ASC_SRES)
		return("ASC_SRES");
	if (val ==  REMOTE_GETKEY)
		return("REMOTE_GETKEY");
	if (val ==  MOUSE_GETMODE)
		return("MOUSE_GETMODE");
	if (val ==  SNDCTL_DSP_SETBLKSIZE)
		return("SNDCTL_DSP_SETBLKSIZE");
	if (val ==  RADIO_SETMODE)
		return("RADIO_SETMODE");
	if (val ==  PIOCRWMEM)
		return("PIOCRWMEM");
	if (val ==  CDIOCPAUSE)
		return("CDIOCPAUSE");
	if (val ==  PPIGCTRL)
		return("PPIGCTRL");
	if (val ==  SIOCDIFPHYADDR)
		return("SIOCDIFPHYADDR");
	if (val ==  SESIOC_GETENCSTAT)
		return("SESIOC_GETENCSTAT");
	if (val ==  GIO_KEYMAPENT)
		return("GIO_KEYMAPENT");
	if (val ==  DIOCSETHOSTID)
		return("DIOCSETHOSTID");
	if (val ==  METEORSBT254)
		return("METEORSBT254");
	if (val ==  PPIGFIFO)
		return("PPIGFIFO");
	if (val ==  METEORGTS)
		return("METEORGTS");
	if (val ==  PPPIOCGMTU)
		return("PPPIOCGMTU");
	if (val ==  VGAPCVTINFO)
		return("VGAPCVTINFO");
	if (val ==  FW_GRSTREAM)
		return("FW_GRSTREAM");
	if (val ==  SOUND_PCM_READ_FILTER)
		return("SOUND_PCM_READ_FILTER");
	if (val ==  DIOCGETRULE)
		return("DIOCGETRULE");
	if (val ==  SIOCDIFADDR)
		return("SIOCDIFADDR");
	if (val ==  DIOCCLRSTATES)
		return("DIOCCLRSTATES");
	if (val ==  OTIOCSETD)
		return("OTIOCSETD");
	if (val ==  I2CWRITE)
		return("I2CWRITE");
	if (val ==  SIOCGIFMTU)
		return("SIOCGIFMTU");
	if (val ==  BIOCGDLT)
		return("BIOCGDLT");
	if (val ==  VT_ACTIVATE)
		return("VT_ACTIVATE");
	if (val ==  CONS_CURRENT)
		return("CONS_CURRENT");
	if (val ==  SIOCADDRT)
		return("SIOCADDRT");
	if (val ==  PPPIOCSXASYNCMAP)
		return("PPPIOCSXASYNCMAP");
	if (val ==  OFIOCGETCHILD)
		return("OFIOCGETCHILD");
	if (val ==  SIOCDELST)
		return("SIOCDELST");
	if (val ==  BT848GCBUF)
		return("BT848GCBUF");
	if (val ==  SERIAL_GETSTAT)
		return("SERIAL_GETSTAT");
	if (val ==  TIOCTIMESTAMP)
		return("TIOCTIMESTAMP");
	if (val ==  DIGIIO_RING)
		return("DIGIIO_RING");
	if (val ==  SERIAL_SETPROTO)
		return("SERIAL_SETPROTO");
	if (val ==  NGIOCGINFO)
		return("NGIOCGINFO");
	if (val ==  SERIAL_GETKEEPALIVE)
		return("SERIAL_GETKEEPALIVE");
	if (val ==  VGAGETFONTATTR)
		return("VGAGETFONTATTR");
	if (val ==  CDIOCCLRDEBUG)
		return("CDIOCCLRDEBUG");
	if (val ==  KBDGCKEY)
		return("KBDGCKEY");
	if (val ==  AIOSMIX)
		return("AIOSMIX");
	if (val ==  PIOCWAIT)
		return("PIOCWAIT");
	if (val ==  SERIAL_GETDEBUG)
		return("SERIAL_GETDEBUG");
	if (val ==  OPROMGETPROP)
		return("OPROMGETPROP");
	if (val ==  SERIAL_GETUNFRAM)
		return("SERIAL_GETUNFRAM");
	if (val ==  SIOCFRSYN)
		return("SIOCFRSYN");
	if (val ==  SIOCLOOKUPDELNODEW)
		return("SIOCLOOKUPDELNODEW");
	if (val ==  BIOCSRTIMEOUT)
		return("BIOCSRTIMEOUT");
	if (val ==  DIOCKILLSTATES)
		return("DIOCKILLSTATES");
	if (val ==  AD_NGAINS_GET)
		return("AD_NGAINS_GET");
	if (val ==  CDRIOCREADFORMATCAPS)
		return("CDRIOCREADFORMATCAPS");
	if (val ==  IOCATAGMAXCHANNEL)
		return("IOCATAGMAXCHANNEL");
	if (val ==  SIOCSWLCNWID)
		return("SIOCSWLCNWID");
	if (val ==  USB_GET_STRING_DESC)
		return("USB_GET_STRING_DESC");
	if (val ==  SIOCDIFPREFIX_IN6)
		return("SIOCDIFPREFIX_IN6");
	if (val ==  DIOCCLRSTATUS)
		return("DIOCCLRSTATUS");
	if (val ==  SERIAL_SETINVTCLK)
		return("SERIAL_SETINVTCLK");
	if (val ==  DIOCOSFPADD)
		return("DIOCOSFPADD");
	if (val ==  VT_OPENQRY)
		return("VT_OPENQRY");
	if (val ==  TIOCSTAT)
		return("TIOCSTAT");
	if (val ==  SIOCINAFR)
		return("SIOCINAFR");
	if (val ==  GIO_DEADKEYMAP)
		return("GIO_DEADKEYMAP");
	if (val ==  AIOSYNC)
		return("AIOSYNC");
	if (val ==  ASC_GWIDTH)
		return("ASC_GWIDTH");
	if (val ==  FBIOGCURSOR)
		return("FBIOGCURSOR");
	if (val ==  CDIOCSTOP)
		return("CDIOCSTOP");
	if (val ==  TIOCUCNTL)
		return("TIOCUCNTL");
	if (val ==  SIOCATHST)
		return("SIOCATHST");
	if (val ==  PPS_IOC_CREATE)
		return("PPS_IOC_CREATE");
	if (val ==  AGPIOC_UNBIND)
		return("AGPIOC_UNBIND");
	if (val ==  DIOCSETTIMEOUT)
		return("DIOCSETTIMEOUT");
	if (val ==  SW_B80x25)
		return("SW_B80x25");
	if (val ==  RADIO_GETMODE)
		return("RADIO_GETMODE");
	if (val ==  AGPIOC_ALLOCATE)
		return("AGPIOC_ALLOCATE");
	if (val ==  SIOCSIFPHYADDR_IN6)
		return("SIOCSIFPHYADDR_IN6");
	if (val ==  SNDCTL_DSP_GETODELAY)
		return("SNDCTL_DSP_GETODELAY");
	if (val ==  METEORSBRIG)
		return("METEORSBRIG");
	if (val ==  ACPIIO_SETSLPSTATE)
		return("ACPIIO_SETSLPSTATE");
	if (val ==  PMIOREAD)
		return("PMIOREAD");
	if (val ==  SIOCGVH)
		return("SIOCGVH");
	if (val ==  FBIOSATTR)
		return("FBIOSATTR");
	if (val ==  FSACTL_CLOSE_GET_ADAPTER_FIB)
		return("FSACTL_CLOSE_GET_ADAPTER_FIB");
	if (val ==  CONS_SETWINORG)
		return("CONS_SETWINORG");
	if (val ==  FBIOPUTCMAP)
		return("FBIOPUTCMAP");
	if (val ==  MEMRANGE_GET)
		return("MEMRANGE_GET");
	if (val ==  SIOCGDEFIFACE_IN6)
		return("SIOCGDEFIFACE_IN6");
	if (val ==  VT_GETACTIVE)
		return("VT_GETACTIVE");
	if (val ==  VMIO_SIOCSKEEP)
		return("VMIO_SIOCSKEEP");
	if (val ==  SW_ENH_C40x25)
		return("SW_ENH_C40x25");
	if (val ==  SMB_READB)
		return("SMB_READB");
	if (val ==  SMB_READW)
		return("SMB_READW");
	if (val ==  FW_GSTBUF)
		return("FW_GSTBUF");
	if (val ==  ACPIIO_BATT_GET_UNITS)
		return("ACPIIO_BATT_GET_UNITS");
	if (val ==  BIOCVERSION)
		return("BIOCVERSION");
	if (val ==  SIOCGWLCACHE)
		return("SIOCGWLCACHE");
	if (val ==  PPPIOCGIDLE)
		return("PPPIOCGIDLE");
	if (val ==  CONS_SSAVER)
		return("CONS_SSAVER");
	if (val ==  ACPIIO_BATT_GET_BATTINFO)
		return("ACPIIO_BATT_GET_BATTINFO");
	if (val ==  KBDRESET)
		return("KBDRESET");
	if (val ==  SIOCSHIWAT)
		return("SIOCSHIWAT");
	if (val ==  SMB_QUICK_WRITE)
		return("SMB_QUICK_WRITE");
	if (val ==  PPPIOCGFLAGS)
		return("PPPIOCGFLAGS");
	if (val ==  TIOCFLUSH)
		return("TIOCFLUSH");
	if (val ==  SIOCGIFMETRIC)
		return("SIOCGIFMETRIC");
	if (val ==  SIOCSIFADDR)
		return("SIOCSIFADDR");
	if (val ==  SOUND_PCM_READ_CHANNELS)
		return("SOUND_PCM_READ_CHANNELS");
	if (val ==  SNDCTL_DSP_STEREO)
		return("SNDCTL_DSP_STEREO");
	if (val ==  SIOCSWLTHR)
		return("SIOCSWLTHR");
	if (val ==  MTIOCGET)
		return("MTIOCGET");
	if (val ==  TVTUNER_GETCHNLSET)
		return("TVTUNER_GETCHNLSET");
	if (val ==  VGASCREENSAVER)
		return("VGASCREENSAVER");
	if (val ==  SW_TEXT_80x43)
		return("SW_TEXT_80x43");
	if (val ==  SNDCTL_DSP_GETBLKSIZE)
		return("SNDCTL_DSP_GETBLKSIZE");
	if (val ==  SERIAL_GETINVTCLK)
		return("SERIAL_GETINVTCLK");
	if (val ==  SMBIOC_WRITE)
		return("SMBIOC_WRITE");
	if (val ==  SIOCFRENB)
		return("SIOCFRENB");
	if (val ==  I4B_TRC_SETA)
		return("I4B_TRC_SETA");
	if (val ==  PIOCSMEM)
		return("PIOCSMEM");
	if (val ==  FSACTL_AIF_THREAD)
		return("FSACTL_AIF_THREAD");
	if (val ==  KBDSREPSW)
		return("KBDSREPSW");
	if (val ==  FSACTL_OPEN_GET_ADAPTER_FIB)
		return("FSACTL_OPEN_GET_ADAPTER_FIB");
	if (val ==  METEORSACTPIXFMT)
		return("METEORSACTPIXFMT");
	if (val ==  SNDCTL_DSP_GETCAPS)
		return("SNDCTL_DSP_GETCAPS");
	if (val ==  SIOCSLIFPHYADDR)
		return("SIOCSLIFPHYADDR");
	if (val ==  PIOCSPOW)
		return("PIOCSPOW");
	if (val ==  SMB_QUICK_READ)
		return("SMB_QUICK_READ");
	if (val ==  SIOCLOOKUPADDTABLE)
		return("SIOCLOOKUPADDTABLE");
	if (val ==  FSACTL_GET_COMM_PERF_DATA)
		return("FSACTL_GET_COMM_PERF_DATA");
	if (val ==  CDIOCALLOW)
		return("CDIOCALLOW");
	if (val ==  BT848_GPIO_GET_EN)
		return("BT848_GPIO_GET_EN");
	if (val ==  TIOCOUTQ)
		return("TIOCOUTQ");
	if (val ==  AIOSSIZE)
		return("AIOSSIZE");
	if (val ==  SIOCGIFPSRCADDR_IN6)
		return("SIOCGIFPSRCADDR_IN6");
	if (val ==  CDRIOCSETBLOCKSIZE)
		return("CDRIOCSETBLOCKSIZE");
	if (val ==  SIOCLOOKUPFLUSH)
		return("SIOCLOOKUPFLUSH");
	if (val ==  GREGADDRD)
		return("GREGADDRD");
	if (val ==  GREGADDRS)
		return("GREGADDRS");
	if (val ==  ASC_SBTIME)
		return("ASC_SBTIME");
	if (val ==  FSACTL_OPENCLS_COMM_PERF_DATA)
		return("FSACTL_OPENCLS_COMM_PERF_DATA");
	if (val ==  FD_GOPTS)
		return("FD_GOPTS");
	if (val ==  TIOCPKT)
		return("TIOCPKT");
	if (val ==  DIOCXBEGIN)
		return("DIOCXBEGIN");
	if (val ==  SIOCATMGETVCCS)
		return("SIOCATMGETVCCS");
	if (val ==  SIOCAIFADDR_IN6)
		return("SIOCAIFADDR_IN6");
	if (val ==  SMBIOC_OPENSESSION)
		return("SMBIOC_OPENSESSION");
	if (val ==  SW_PC98_80x30)
		return("SW_PC98_80x30");
	if (val ==  AUDIO_COMPAT_FLUSH)
		return("AUDIO_COMPAT_FLUSH");
	if (val ==  SERIAL_GETLOOP)
		return("SERIAL_GETLOOP");
	if (val ==  SW_VESA_32K_1280)
		return("SW_VESA_32K_1280");
	if (val ==  TIIOCSETTRACE)
		return("TIIOCSETTRACE");
	if (val ==  FIOCLEX)
		return("FIOCLEX");
	if (val ==  SERIAL_SETDEBUG)
		return("SERIAL_SETDEBUG");
	if (val ==  SIOCPROXY)
		return("SIOCPROXY");
	if (val ==  DIOCCHANGEALTQ)
		return("DIOCCHANGEALTQ");
	if (val ==  SW_VESA_FULL_1024)
		return("SW_VESA_FULL_1024");
	if (val ==  SNDCTL_DSP_GETOSPACE)
		return("SNDCTL_DSP_GETOSPACE");
	if (val ==  TVTUNER_GETSTATUS)
		return("TVTUNER_GETSTATUS");
	if (val ==  I4B_TEL_VR_REQ)
		return("I4B_TEL_VR_REQ");
	if (val ==  SIOCGPPPSTATS)
		return("SIOCGPPPSTATS");
	if (val ==  SOUND_PCM_READ_BITS)
		return("SOUND_PCM_READ_BITS");
	if (val ==  FM_SET_FREQ)
		return("FM_SET_FREQ");
	if (val ==  SW_ENH_CG640)
		return("SW_ENH_CG640");
	if (val ==  SIOCSPGRP)
		return("SIOCSPGRP");
	if (val ==  CDNR_DISABLE)
		return("CDNR_DISABLE");
	if (val ==  TIIOCGETSTATS)
		return("TIIOCGETSTATS");
	if (val ==  PIOCSBEEP)
		return("PIOCSBEEP");
	if (val ==  USB_SET_CM_OVER_DATA)
		return("USB_SET_CM_OVER_DATA");
	if (val ==  ASC_GBTIME)
		return("ASC_GBTIME");
	if (val ==  AUDIO_FLUSH)
		return("AUDIO_FLUSH");
	if (val ==  PIO_KEYMAP)
		return("PIO_KEYMAP");
	if (val ==  PPPIOCSMRU)
		return("PPPIOCSMRU");
	if (val ==  PPPIOCGUNIT)
		return("PPPIOCGUNIT");
	if (val ==  PPPIOCSMTU)
		return("PPPIOCSMTU");
	if (val ==  CIOCGSESSION)
		return("CIOCGSESSION");
	if (val ==  SIOCIFCREATE)
		return("SIOCIFCREATE");
	if (val ==  CONS_GETCURSORSHAPE)
		return("CONS_GETCURSORSHAPE");
	if (val ==  FBIOGCURMAX)
		return("FBIOGCURMAX");
	if (val ==  DEVFSIO_SUSE)
		return("DEVFSIO_SUSE");
	if (val ==  TUNSLMODE)
		return("TUNSLMODE");
	if (val ==  SW_CG320_D)
		return("SW_CG320_D");
	if (val ==  I4B_RBCH_DIALOUT)
		return("I4B_RBCH_DIALOUT");
	if (val ==  SOUND_PCM_READ_RATE)
		return("SOUND_PCM_READ_RATE");
	if (val ==  SIOCADAFR)
		return("SIOCADAFR");
	if (val ==  BIOCGRSIG)
		return("BIOCGRSIG");
	if (val ==  I4B_CDID_REQ)
		return("I4B_CDID_REQ");
	if (val ==  SIOCGWLCNWID)
		return("SIOCGWLCNWID");
	if (val ==  PMIOSETUP)
		return("PMIOSETUP");
	if (val ==  SW_VESA_64K_1024)
		return("SW_VESA_64K_1024");
	if (val ==  PPISDATA)
		return("PPISDATA");
	if (val ==  SIOCATMOPENVCC)
		return("SIOCATMOPENVCC");
	if (val ==  SNPGTTY)
		return("SNPGTTY");
	if (val ==  DIOCNATLOOK)
		return("DIOCNATLOOK");
	if (val ==  USB_REQUEST)
		return("USB_REQUEST");
	if (val ==  TAPSIFINFO)
		return("TAPSIFINFO");
	if (val ==  PPIGSTATUS)
		return("PPIGSTATUS");
	if (val ==  SPIGOT_GET_INFO)
		return("SPIGOT_GET_INFO");
	if (val ==  SIOCSETPFSYNC)
		return("SIOCSETPFSYNC");
	if (val ==  SW_VESA_FULL_640)
		return("SW_VESA_FULL_640");
	if (val ==  PTIOCGETTIMEOUT)
		return("PTIOCGETTIMEOUT");
	if (val ==  SNDCTL_COPR_RCODE)
		return("SNDCTL_COPR_RCODE");
	if (val ==  DIOCGDINFO)
		return("DIOCGDINFO");
	if (val ==  IOCATAATTACH)
		return("IOCATAATTACH");
	if (val ==  SERIAL_SETUSE16)
		return("SERIAL_SETUSE16");
	if (val ==  GRESPROTO)
		return("GRESPROTO");
	if (val ==  USB_SET_CONFIG)
		return("USB_SET_CONFIG");
	if (val ==  CDIOCSETPATCH)
		return("CDIOCSETPATCH");
	if (val ==  I2CSTOP)
		return("I2CSTOP");
	if (val ==  KDGKBMODE)
		return("KDGKBMODE");
	if (val ==  PPS_IOC_SETPARAMS)
		return("PPS_IOC_SETPARAMS");
	if (val ==  DIOCWDINFO)
		return("DIOCWDINFO");
	if (val ==  SNDCTL_COPR_WDATA)
		return("SNDCTL_COPR_WDATA");
	if (val ==  SW_VESA_FULL_800)
		return("SW_VESA_FULL_800");
	if (val ==  BIOCSDLT)
		return("BIOCSDLT");
	if (val ==  TIOCMBIC)
		return("TIOCMBIC");
	if (val ==  TIOCMBIS)
		return("TIOCMBIS");
	if (val ==  SIOCSIFGENERIC)
		return("SIOCSIFGENERIC");
	if (val ==  RADIO_SETFREQ)
		return("RADIO_SETFREQ");
	if (val ==  FSACTL_FORCE_DELETE_DISK)
		return("FSACTL_FORCE_DELETE_DISK");
	if (val ==  APMIO_GETPWSTATUS)
		return("APMIO_GETPWSTATUS");
	if (val ==  ALT_ATTACH)
		return("ALT_ATTACH");
	if (val ==  TIOCCONS)
		return("TIOCCONS");
	if (val ==  USB_GET_INTERFACE_DESC)
		return("USB_GET_INTERFACE_DESC");
	if (val ==  FIONREAD)
		return("FIONREAD");
	if (val ==  SW_VESA_C80x60)
		return("SW_VESA_C80x60");
	if (val ==  DIOCGFRONTSTUFF)
		return("DIOCGFRONTSTUFF");
	if (val ==  SW_VESA_C132x25)
		return("SW_VESA_C132x25");
	if (val ==  KBDGOKEY)
		return("KBDGOKEY");
	if (val ==  IOCATAGMODE)
		return("IOCATAGMODE");
	if (val ==  PMIOSTOP)
		return("PMIOSTOP");
	if (val ==  DIOCSTOP)
		return("DIOCSTOP");
	if (val ==  SESIOC_SETENCSTAT)
		return("SESIOC_SETENCSTAT");
	if (val ==  FBIOSVIDEO)
		return("FBIOSVIDEO");
	if (val ==  SNDCTL_DSP_GETFMTS)
		return("SNDCTL_DSP_GETFMTS");
	if (val ==  DIOCBSDBB)
		return("DIOCBSDBB");
	if (val ==  DIOCRADDTABLES)
		return("DIOCRADDTABLES");
	if (val ==  GIO_FONT8x14)
		return("GIO_FONT8x14");
	if (val ==  SMBIOC_REQUEST)
		return("SMBIOC_REQUEST");
	if (val ==  SW_C80x25)
		return("SW_C80x25");
	if (val ==  KBDGLOCK)
		return("KBDGLOCK");
	if (val ==  GSC_GBLEN)
		return("GSC_GBLEN");
	if (val ==  SIOCLOOKUPADDNODEW)
		return("SIOCLOOKUPADDNODEW");
	if (val ==  BIOCGSTATS)
		return("BIOCGSTATS");
	if (val ==  SIOCGIFAFLAG_IN6)
		return("SIOCGIFAFLAG_IN6");
	if (val ==  USB_GET_CONFIG_DESC)
		return("USB_GET_CONFIG_DESC");
	if (val ==  USB_GET_REPORT)
		return("USB_GET_REPORT");
	if (val ==  TUNSIFINFO)
		return("TUNSIFINFO");
	if (val ==  I4B_CTL_GET_CHIPSTAT)
		return("I4B_CTL_GET_CHIPSTAT");
	if (val ==  BT848_SVSAT)
		return("BT848_SVSAT");
	if (val ==  DEVFSIO_RADD)
		return("DEVFSIO_RADD");
	if (val ==  DIOCSMBR)
		return("DIOCSMBR");
	if (val ==  AIOGMIX)
		return("AIOGMIX");
	if (val ==  OFIOCSET)
		return("OFIOCSET");
	if (val ==  STL_SETPFLAG)
		return("STL_SETPFLAG");
	if (val ==  SERIAL_SETMASTER)
		return("SERIAL_SETMASTER");
	if (val ==  I4B_CTL_CLR_LAPDSTAT)
		return("I4B_CTL_CLR_LAPDSTAT");
	if (val ==  SNDCTL_DSP_GETISPACE)
		return("SNDCTL_DSP_GETISPACE");
	if (val ==  SNDCTL_DSP_GETOPTR)
		return("SNDCTL_DSP_GETOPTR");
	if (val ==  DIOCSKERNELDUMP)
		return("DIOCSKERNELDUMP");
	if (val ==  ALT_READ_TG_MEM)
		return("ALT_READ_TG_MEM");
	if (val ==  AD_GAINS_GET)
		return("AD_GAINS_GET");
	if (val ==  PPPIOCSNPMODE)
		return("PPPIOCSNPMODE");
	if (val ==  DIOCADDADDR)
		return("DIOCADDADDR");
	if (val ==  SIOCGIFINFO_IN6)
		return("SIOCGIFINFO_IN6");
	if (val ==  SIOCGIFFLAGS)
		return("SIOCGIFFLAGS");
	if (val ==  IOCATADEVICES)
		return("IOCATADEVICES");
	if (val ==  TIOCSETAW)
		return("TIOCSETAW");
	if (val ==  CDIOCSETDEBUG)
		return("CDIOCSETDEBUG");
	if (val ==  AGPIOC_DEALLOCATE)
		return("AGPIOC_DEALLOCATE");
	if (val ==  SNDCTL_DSP_NONBLOCK)
		return("SNDCTL_DSP_NONBLOCK");
	if (val ==  SNDCTL_TMR_CONTINUE)
		return("SNDCTL_TMR_CONTINUE");
	if (val ==  PIOCBIC)
		return("PIOCBIC");
	if (val ==  TAPGIFINFO)
		return("TAPGIFINFO");
	if (val ==  PIOCBIS)
		return("PIOCBIS");
	if (val ==  SLIOCSUNIT)
		return("SLIOCSUNIT");
	if (val ==  SERIAL_SETMODE)
		return("SERIAL_SETMODE");
	if (val ==  SIOCGETVIFCNT)
		return("SIOCGETVIFCNT");
	if (val ==  CDNR_ENABLE)
		return("CDNR_ENABLE");
	if (val ==  RIO_RECV_COMMAND)
		return("RIO_RECV_COMMAND");
	if (val ==  MOUSE_SETMODE)
		return("MOUSE_SETMODE");
	if (val ==  DIOCSETLIMIT)
		return("DIOCSETLIMIT");
	if (val ==  FSACTL_GET_PCI_INFO)
		return("FSACTL_GET_PCI_INFO");
	if (val ==  FD_READID)
		return("FD_READID");
	if (val ==  APMIO_NEXTEVENT)
		return("APMIO_NEXTEVENT");
	if (val ==  SW_ENH_MONOAPA2)
		return("SW_ENH_MONOAPA2");

	return(NULL);
}
