/*
** ktrace.h for  in /home/nico/lang/c
**
** Made by Nicolas Cormier
** Login   <nico@epita.fr>
**
** Started on  Wed Oct 26 18:28:37 2005 Nicolas Cormier
** Last update Wed Nov  2 14:09:25 2005 Nicolas Cormier
*/

#ifndef __KTRACE_H__
#define __KTRACE_H__

#define DEFAULT_TRACEFILE	"ktrace.out"
#define DEFAULT_LINE_WIDTH	80

#define MASKOPT_DECIMAL		0x000001
#define MASKOPT_LOOPREADING	0x000010
#define MASKOPT_HOCSUPPR	0x000100
#define MASKOPT_TIMESTAMPS	0x001000
#define MASKOPT_RETIMESTAMPS	0x010000
#define MASKOPT_ABSTIMESTAMPS	0x100000

#define TABIDX_FILE		0
#define TABIDX_MAXDATA		1
#define TABIDX_PID		2

#define MASKOPT_TR_C		0x0000001
#define MASKOPT_TR_N		0x0000010
#define MASKOPT_TR_I		0x0000100
#define MASKOPT_TR_S		0x0001000
#define MASKOPT_TR_U		0x0010000
#define MASKOPT_TR_W		0x0100000
#define MASKOPT_TR_PLUS		0x1000000

#define MASKOPT_TR_DEF		MASKOPT_TR_C|MASKOPT_TR_N|MASKOPT_TR_I|MASKOPT_TR_S| MASKOPT_TR_U

#endif /* __KTRACE_H__ */
