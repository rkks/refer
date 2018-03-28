/*
** coffobj.h for  in /u/ept3/parsi_u/ftrace/src/lib/tru64_alpha/bin
**
** Made by ugo parsi
** Login   <parsi_u@epita.fr>
**
** Started on  Wed Apr  5 11:15:39 2006 ugo parsi
** Last update Thu Apr 27 11:19:10 2006 nicolas rimbeau
*/

#ifndef __COFFOBJ_H__
#define __COFFOBJ_H__

#define MAGIC_COFF_HDR 0x0183
#define MAGIC_COFF_SYM_HDR 0x1992
#define MAGIC_COFF_CORE "Core"

#include <sys/core.h>

typedef struct	fdr
{
  unsigned long	adr;
  long		cbLineOffset;
  long		cbLine;
  long		cbSs;
  int		rss;
  int		issBase;
  int		isymBase;
  int		csym;
  int		ilineBase;
  int		cline;
  int		ioptBase;
  int		copt;
  int		ipdFirst;
  int		cpd;
  int		iauxBase;
  int		caux;
  int		rfdBase;
  int		crfd;
  unsigned int	lang : 5;
  unsigned int	fMerge : 1;
  unsigned int	fReadin : 1;
  unsigned int	fBigendian : 1;
  unsigned int	glevel : 2;
  unsigned int	fTrim : 1;
#ifndef TANDEMSYM
  unsigned int	reserved : 5;
#else
  unsigned int	platform : 3;
  unsigned int	reserved : 2;
#endif
  unsigned short vstamp;
  unsigned int	reserved2;
}		fdr_t;

typedef struct	filehdr
{
  unsigned char	f_magic[2];     /* magic number                 */
  unsigned char f_nscns[2];     /* number of sections           */
  unsigned char f_timdat[4];    /* time & date stamp            */
  unsigned char f_symptr[8];    /* file pointer to symtab       */
  unsigned char f_nsyms[4];     /* number of symtab entries     */
  unsigned char f_opthdr[2];    /* sizeof(optional hdr)         */
  unsigned char f_flags[2];     /* flags                        */
}		filehdr_t;

typedef struct	hdr_ext
{
  unsigned char h_magic[2];
  unsigned char h_vstamp[2];
  unsigned char h_ilineMax[4];
  unsigned char h_idnMax[4];
  unsigned char h_ipdMax[4];
  unsigned char h_isymMax[4];
  unsigned char h_ioptMax[4];
  unsigned char h_iauxMax[4];
  unsigned char h_issMax[4];
  unsigned char h_issExtMax[4];
  unsigned char h_ifdMax[4];
  unsigned char h_crfd[4];
  unsigned char h_iextMax[4];
  unsigned char h_cbLine[8];
  unsigned char h_cbLineOffset[8];
  unsigned char h_cbDnOffset[8];
  unsigned char h_cbPdOffset[8];
  unsigned char h_cbSymOffset[8];
  unsigned char h_cbOptOffset[8];
  unsigned char h_cbAuxOffset[8];
  unsigned char h_cbSsOffset[8];
  unsigned char h_cbSsExtOffset[8];
  unsigned char h_cbFdOffset[8];
  unsigned char h_cbRfdOffset[8];
  unsigned char h_cbExtOffset[8];
}		hdrext_t;

typedef struct	base_sym
{
  unsigned long	value;
  unsigned int	iss;
  unsigned int	st : 6;
  unsigned int	sc : 5;
  unsigned int	reserved : 1;
  unsigned int	index : 20;
}		basesym_t;

typedef struct	ext_sym
{
  basesym_t	es_asym;
  unsigned char	es_bits1[1];
  unsigned char	es_bits2[3];
  unsigned char	es_ifd[4];
}		extsym_t;

typedef struct		dynsym
{
  unsigned int		st_name;
  unsigned int		reserved;
  unsigned long		st_value;
  unsigned int		st_size;
  unsigned char		st_info[1];
  unsigned char		st_other[1];
  unsigned short	st_shndx;
}			dynsym_t;

typedef struct	dynhdr
{
  int		d_tag;
  unsigned int	reserved;
  union
  {
    unsigned int	d_val;
    unsigned long	d_ptr;
  }			d_un;
}		dynhdr_t;

typedef struct	liblist
{
  unsigned int	l_name;
  unsigned int	l_time_stamp;
  unsigned int	l_checksum;
  unsigned int	l_version;
  unsigned int	l_flags;
}		liblist_t;

typedef struct	aouthdr
{
  unsigned char magic[2];       /* type of file                         */
  unsigned char vstamp[2];      /* version stamp                        */
  unsigned char bldrev[2];      /* ?? */
  unsigned char padding[2];     /* pad to quadword boundary             */
  unsigned char tsize[8];       /* text size in bytes                   */
  unsigned char dsize[8];       /* initialized data "  "                */
  unsigned char bsize[8];       /* uninitialized data "   "             */
  unsigned char entry[8];       /* entry pt.                            */
  unsigned char text_start[8];  /* base of text used for this file */
  unsigned char data_start[8];  /* base of data used for this file */
  unsigned char bss_start[8];   /* base of bss used for this file */
  unsigned char gprmask[4];     /* bitmask of general registers used */
  unsigned char fprmask[4];     /* bitmask of floating point registers used */
  unsigned char gp_value[8];    /* value for gp register */
}		aouthdr_t;

typedef struct	scnhdr
{
  unsigned char s_name[8];
  unsigned char s_paddr[8];
  unsigned char s_vaddr[8];
  unsigned char s_size[8];
  unsigned char s_scnptr[8];
  unsigned char s_relptr[8];
  unsigned char s_lnnoptr[8];
  unsigned char s_nreloc[2];
  unsigned char	s_nlnno[2];
  unsigned char s_flags[4];
}		scnhdr_t;

typedef struct
{
  filehdr_t*	header;
  scnhdr_t**	section_headers;
}		coffobj_t;

typedef struct
{
  struct core_filehdr*	core_header;
  struct core_scnhdr**	core_section_headers;
}		osfcoreobj_t;

#endif
