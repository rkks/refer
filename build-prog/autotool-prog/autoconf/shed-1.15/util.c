/*
$Id: util.c,v 1.9 2009/03/23 12:42:15 alexsisson Exp $

(C) Copyright 2002-2004 Alex Sisson (alexsisson@gmail.com)

This file is part of shed.

shed is free software; you can redistribute it and/or modify
under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

shed is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with shed; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

/* includes */
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <getopt.h>
#include <stdint.h>

#include "util.h"

/* ascii control char descs */
char  ascii_short_desc[] = {'0',0,0,0,0,0,0,'a','b','t','n','v','f','r'};
char *ascii_long_desc[]  = {"NUL","SOH","STX","ETX","EOT","ENQ","ACK","BEL",
                            "BS ","HT ","LF ","VT ","FF ","CR ","SO ","SI ",
                            "DLE","DC1","DC2","DC3","DC4","NAK","SYN","ETB",
                            "CAN","EM ","SUB","ESC","FS ","GS ","RS ","US ","SPC"};



/* returns a description of the char if its <32 || >126 */
char *getascii(unsigned char c, char *s, int mode)
{
  if(c>127)
    strncpy(s,"   ",3);
  else if(c>=32 && c<=126) {
    s[0] = ' ';
    s[1] = c;
    s[2] = ' ';
  } else {
    switch(mode) {
      case 1: /* do c-style control chars */
        if(c!=127 && ascii_short_desc[c]) {
          s[0] = ' ';
          s[1] = '\\';
          s[2] = ascii_short_desc[c];
          break;
        }

      case 0: /* no special chars */
        strncpy(s,"   ",3);
        break;

      case 2: /* 3 letter descs */
        strncpy(s,c!=127?ascii_long_desc[c]:"DEL",3);
        break;
    }
  }
  s[3] = 0;
  return s;
}

/* returns string representation of n in base 'base' */
char *getstring(uint64_t n, char *s, int base, int width) {
  uint64_t i,j,c,d = calcwidth(n,base);
  char *p = s;
  if(width && d<width) {
    for(i=d;i<width;i++) {
      *p = '0';
      p++;
    }
  }
  for(i=d;i;i--) {
    j = pow(base,i-1);
    c = 0;
    while(n>=j) {
      n-=j;
      c++;
    }
    *p = c + ((c<10) ? 48 : 55);
    p++;
  }
  *p = 0;
  return s;
}

/* parses 's' as a 'base' base string */
int64_t parsestring(char *s, int base) {

  unsigned int i,j,slen=strlen(s),n=0;
  int64_t r = 0;

  for(i=0;i<slen;i++) {
    if(s[i]>='0' && s[i]<='9')
      n = s[i] - 48;
    else if(s[i]>='A' && s[i]<='F')
      n = s[i] - 55;
    else if(s[i]>='a' && s[i]<='f')
      n = s[i] - 87;
    else
      return -1;
    if(n>=base)
      return -1;
    for(j=1;j<slen-i;j++)
      n*=base;
    r+=n;
  }
  return r;
}

#include <stdio.h>

/* calculates the width needed to represent 'n' in base 'base' */
int calcwidth(uint64_t n, int base) {
  unsigned int i,j;
  uint64_t b;
  if(!base)
    return 0;
  for(i=0;;i++) {
    b = base;
    for(j=0;j<i;j++)
      b *= base;
    if(b>n || b<base)
      return i + 1;
  }
  return 0;
}

/* creates optstring arg (short opts) from for getopt */
char *getopt_makeoptstring(struct option *opt) {
  int n = 0;
  char *optstring,*p;
  struct option *o = opt;

  while(o->name || o->has_arg || o->flag || o->val) {
    n += 1 + o->has_arg;
    if(o->flag)
      return NULL; /* fail because not returning o->val */
    o++;
  }

  p = optstring = malloc(n+1);
  o = opt;

  while(o->name || o->has_arg || o->flag || o->val) {
    *p++ = o->val;
    n = o->has_arg;
    while(n--)
      *p++ = ':';
    o++;
  }
  *p = 0;

  return optstring;
}

