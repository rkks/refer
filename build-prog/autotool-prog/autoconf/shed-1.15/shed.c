/*
$Id: shed.c,v 1.51 2009/03/24 13:47:26 alexsisson Exp $

shed 1.15 source

(C) Copyright 2002-2009 Alex Sisson (alexsisson@gmail.com)

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#if SHED_LFS == 1
#define SHED_FSEEK fseeko
#else
#define SHED_FSEEK fseek
#endif

#if SHED_NOFOLLOW == 1
#define _GNU_SOURCE
#endif

/* includes */
#include <stdio.h>      /* fprintf    */
#include <ncurses.h>    /* ncurses    */
#include <signal.h>     /* signal     */
#include <string.h>     /* strcmp etc */
#include <stdlib.h>     /* exit       */
#include <sys/stat.h>   /* stat       */
#include <getopt.h>     /* getopt     */
#include <ctype.h>      /* tolower    */
#include <unistd.h>     /* dup, dup2  */
#include <sys/select.h> /* select     */
#include <fcntl.h>      /* open       */
#include <errno.h>

#include "util.h"

//#define LOG
#ifdef LOG
FILE *SHEDLOG;
void shedloginit() {
  SHEDLOG = fopen("shed.log","w");
}
#endif

/* function prototypes */
void finish(int s);
void ctrlc(int s);
void cursorup(int n);
void cursordown(int n);
void cursorleft();
void cursorright();
void cursoradjust();
int  cursorjump(off_t n);
void clearline(int line, int width);
int  search(char* str);
int  getinput(char *prompt, int state, int acceptemptystr, int hexonly);
int  mainloop();
int  redraw();
int  redraw_key_help(int onlykeyhelp);
int  dump(char *dumpfile);

/* globals */
char   *filename;
FILE   *f                = NULL;
off_t  len               = 0;                               /* file length */
off_t  cursorrow         = 0;                               /* offset of cursor from SOF */
int    cursorcol         = 0;                               /* which column cursor is in */
off_t  viewport          = 0;                               /* offset of current view from SOF */
int    viewsize          = 0;                               /* size of viewport */
int    decmode           = 1;                               /* dec or hex display */
int    ascmode           = 0;                               /* setting for ascii column */
int    readonly          = 0;                               /* readonly flag */
int    preview           = 0;                               /* preview mode on/off */
off_t  startoffset       = 0;                               /* arg of --start stored here */
int    offsetwidth       = 0;                               /* width of left column */
int    colbase[5]        = {0,16,10,8,2};                   /* base of each column */
int    colwidth[5]       = {1,2,3,3,8};                     /* width of columns */
int    coloffset[5]      = {0,6,10,14,18};                  /* offset from left for each column */
char   *coltitle[5]      = {"asc","hex","dec","oct","bin"}; /* name of each column */
char   *searchstr[5]     = {0,0,0,0,0};                     /* previous searches */
char   *reply            = NULL;                            /* where input is returned */
int    fdin              = 0;                               /* for dup()'ed stdin */
fd_set fdset;                                               /* for select()ing fdin */
char   *stdinbuf         = NULL;                            /* buf for stdin */
int    inputstate        = 0;                               /* input state */
int    binmode           = 1;                               /* binary column mode */
int    binmodecursor     = 0;                               /* cursor pos in binary toggle mode */
off_t  lenarg            = 0;                               /* --length parameter */
int    isdevice          = 0;                               /* is file a device? */
char   *filetype         = "";                              /* description of file type */
int    nofollow          = 0;                               /* do not follow symlinks? */
int    searchbackwards   = 0;                               /* are we searching backwards? */
int    searchinsensitive = 0;                               /* are we searching insensitive of case */
char  *message           = NULL;                            /* message to display */
int    calclength        = 0;
int    hlo               = 0;                               /* highlight offset */
char  *hlv               = NULL;                            /* highlight values */
char   hloc              = '*';                             /* highlight offset char */
char   hlvc              = '*';                             /* highlight value char */
int    editsize          = 1;                               /* size of cursor in bytes */
int    bigendianmode     = 1;                               /* are we treating multi-byte values as big endian? */
int    signedmode;

/* pointer to stat function */
int (*STAT)(const char *path, struct stat *buf) = stat;

/* defines */
#define INPUTSTATE_MAIN                      0
#define INPUTSTATE_GETSTR                    1
#define INPUTSTATE_GETSTR_HEX                2
#define INPUTSTATE_GETSTR_SEARCH             3

#define COL_ASC 0
#define COL_HEX 1
#define COL_DEC 2
#define COL_OCT 3
#define COL_BIN 4

#define GETINPUT_EMPTYOK

#define STDINBUFSIZE 0xFFFF

#define errstr (strerror(errno))


/* main */
int main(int argc, char **argv) {

  int i;

  /* getopt long opts */
  struct option opts[] = { {"help",0,0,'h'},
                           {"version",0,0,'v'},
                           {"hex",0,0,'H'},
                           {"length",1,0,'L'},
                           {"readonly",0,0,'r'},
                           {"start",1,0,'s'},
/*
                           {"hlo",1,0,'o'},
                           {"hlv",1,0,'v'},
                           {"hloc",1,0,'o'},
*/
#if SHED_NOFOLLOW == 1
                           {"nofollow",0,0,'n'},
#endif
                           {0,0,0,0} };

#ifdef LOG
  shedloginit();
#endif


  /* hack for getopt's error messages */
  argv[0] = strdup(PACKAGE);

  /* process args */
  while(1) {
    i = getopt_long(argc,argv,getopt_makeoptstring(opts),opts,0);
    if(i<0)
      break;
    switch(i) {
      case 'h':
        printf("usage: %s [OPTIONS] [FILE]\n\n",PACKAGE);
        printf("options:\n");
        printf("  -r / --readonly       open FILE read only\n");
        printf("  -s / --start=OFFSET   position cursor to offset\n");
        printf("  -H / --hex            start with hex offsets\n");
        printf("  -L / --length         set length (for device files)\n");
#if SHED_NOFOLLOW == 1
        printf("  -n / --nofollow       do not follow symlinks\n");
#endif
        printf("  -h / --help           show help and exit\n");
        printf("  -v / --version        show version and exit\n");
        printf("\n");
        return 0;
      case 'v':
        printf("%s %s",PACKAGE,VERSION);
#if SHED_LFS == 1
        printf(" (lfs)");
#endif
#if SHED_CYGFIX == 1
        printf(" (cygfix)");
#endif
#if SHED_NOFOLLOW == 1
        printf(" (nofollow)");
#endif
        printf("\n");
        return 0;
      case 'H':
        decmode = 0;
        break;
      case 'L':
        lenarg = atoll(optarg);
        //printf("%lld\n",lenarg);
        break;
      case 'r':
        readonly = 1;
        break;
      case 's':
        startoffset = atoi(optarg);
        break;
      case 'n':
        nofollow = 1;
        STAT = lstat;
        break;
      case 'o':
        hlo = atoi(optarg);
        break;
      case '?':
        return 1;
    }
  }

  /* open stream */
  switch(argc-optind) {

    case 1:
      /* non-option argument */
      if(argv[optind][0]!='-' || strlen(argv[optind])>1) {
        filename = argv[optind];

        /* open file */
        if(!readonly) {
          i = O_RDWR;
#if SHED_NOFOLLOW == 1
        if(nofollow)
          i |= O_NOFOLLOW;
#endif
          fdin = open(filename,i);
          if(fdin>=0)
            f = fdopen(fdin,"r+");
        }
        if(readonly || fdin<0) {
          i = O_RDONLY;
#if SHED_NOFOLLOW == 1
          if(nofollow)
            i |= O_NOFOLLOW;
#endif
          fdin = open(filename,i);
          if(fdin<0) {
            fprintf(stderr,"%s: could not open %s (%s)\n",PACKAGE,filename,errstr);
            return 1;
          }
          f = fdopen(fdin,"r");
          readonly = 1;
        }
        fdin = 0;

        /* stat file */
        struct stat st;
        if(STAT(filename,&st)<0) {
          fprintf(stderr,"%s: could not stat %s\n",PACKAGE,filename);
          return 1;
        }

        if(S_ISREG(st.st_mode))
          filetype = " (regular file)";
        else if(S_ISDIR(st.st_mode))
          filetype = " (directory)";
        else if(S_ISCHR(st.st_mode))
          filetype = " (character device)";
        else if(S_ISBLK(st.st_mode))
          filetype = " (block device)";
        else if(S_ISFIFO(st.st_mode))
          filetype = " (named pipe)";
        else if(S_ISLNK(st.st_mode))
          filetype = " (symbolic link)";
        else if(S_ISSOCK(st.st_mode))
          filetype = " (socket)";

        len = st.st_size;
        fgetc(f);
        if(!len) {
          if(feof(f)) {
            fprintf(stderr,"%s: %s has zero size\n",PACKAGE,filename);
            return 1;
          }
          /* else some kind of special file (eg device) */
          isdevice = S_ISCHR(st.st_mode) || S_ISBLK(st.st_mode);
          if(lenarg>0) {
            len = lenarg;
          }
          else if(calclength) {
            rewind(f);
            while(1) {
              i = fgetc(f);
              if(i<0)
                break;
              if((len%1000)==0)
                printf("%llu\n",(unsigned long long)len);
              len++;
            }

          }
        }
        break;
      }
      /* else drop through */

    case 0:
      /* reading from stdin */
      if(isatty(STDIN_FILENO)) {
        fprintf(stderr,"%s: input from stdin must be piped/redirected.\n",PACKAGE);
        return 1;
      }
      f = tmpfile();
      if(!f) {
        fprintf(stderr,"%s: tmpfile() failed.\n",PACKAGE);
        return 1;
      }
      readonly = 1;
      /* sort out fd's so we can still press keys */
      fdin = dup(STDIN_FILENO);
      dup2(STDOUT_FILENO,STDIN_FILENO);
      filename = "(stdin)";
      stdinbuf = malloc(STDINBUFSIZE);
      break;

    default:
      fprintf(stderr,"%s: too many files!\n",PACKAGE);
      return 1;
      break;
  }

  /* init ncurses */
  signal(SIGINT,ctrlc);
  initscr();
  keypad(stdscr,TRUE);
  cbreak();
  noecho();
  halfdelay(1);

  /* set size of viewport to LINES - 6 (2 reservered for top + 4 for bottom area) */
  viewsize = LINES - 6;

  /* calculate the width for the offset column and round it */
  if(len) {
    offsetwidth = calcwidth(len,10);
    while(offsetwidth%4!=0)
      offsetwidth++;
  } else
    offsetwidth = 16;

  if(startoffset) {
    if(len && startoffset>len)
      startoffset = len-1;
    cursorjump(startoffset);
  }

  redraw();
  mainloop();
  return 0;
}

int mainloop() {

  int i,key;
  char s[256];

  while(1) {
    switch(inputstate) {

      /* handle user input prompts */
      case INPUTSTATE_GETSTR:
      case INPUTSTATE_GETSTR_HEX: /* allows toggle */
      case INPUTSTATE_GETSTR_SEARCH:
        key = getch();
        if(inputstate==INPUTSTATE_GETSTR_SEARCH) {
          /* search specific keys */
          switch(key) {
            case 2: /* ctrl b */
              searchbackwards = !searchbackwards;
              redraw_key_help(1);
              break;
            case 9: /* ctrl i */
              searchinsensitive = !searchinsensitive;
              redraw_key_help(1);
              continue;
            default:
              break;
          }
        }
        switch(key) {
          case 3:  /* ctrlc */
          case '\n':
            inputstate = INPUTSTATE_MAIN;
            if(message) {
              free(message);
              message = NULL;
            }
            redraw();
            return (key=='\n') ? 0 : -1;
          case KEY_BACKSPACE:
          case 127: /* bkspc */
          case 8:   /* ^H */
            if(strlen(reply)) {
              reply[strlen(reply)-1] = 0;
              mvaddch(stdscr->_cury,stdscr->_curx-1,' ');
              move(stdscr->_cury,stdscr->_curx-1);
            }
            break;
          default:
            if(inputstate==INPUTSTATE_GETSTR_HEX) {
              if(!isxdigit(key)) {
                switch(toupper(key)) {
                  case 'T':
                    decmode = !decmode;
                    break;
                }
              }
              key=0;
            } else if(!isprint(key)) {
              key=0;
            }
            if(key) {
              reply[strlen(reply)] = key;
              addch(key);
              refresh();
            }
            break;
          }
          break;
        break;

      /* normal key handling */
      case INPUTSTATE_MAIN:
        if(fdin) {
          struct timeval tv = {0,0};
          FD_ZERO(&fdset);
          FD_SET(fdin,&fdset);
          if(select(fdin+1,&fdset,0,0,&tv)>0) {
            if(FD_ISSET(fdin,&fdset)) {
              i = read(fdin,stdinbuf,STDINBUFSIZE);
              if(i>0) {
                SHED_FSEEK(f,len,SEEK_SET);
                fwrite(stdinbuf,1,i,f);
                len += i;
              } else
                fdin = 0;
            }
          }
        }
        redraw();
        if(message) {
          free(message);
          message = NULL;
        }
        key = toupper(getch());
        switch(key) {
          case KEY_UP:
            cursorup(1);
            break;
          case KEY_DOWN:
            cursordown(1);
            break;
          case KEY_LEFT:
            cursorleft(1);
            break;
          case KEY_RIGHT:
          case 9: /* tab */
            cursorright(1);
            break;
          case KEY_PPAGE:
          case 25: /* ctrl Y */
            cursorup(16);
            break;
          case KEY_NPAGE:
          case 22: /* ctrl V */
            cursordown(16);
            break;
          case KEY_HOME:
          case 1: /* ctrl A */
            if(cursorcol==COL_BIN && binmode && binmodecursor)
              binmodecursor = 0;
            else
              cursorcol = 0;
            break;
          case KEY_END:
          case 5: /* ctrl E */
            cursorcol = COL_BIN;
            binmodecursor = 7;
            break;

          /* edit */
          case ' ':
          case 'E':
            if(readonly) {
              beep();
              break;
            }
            if(cursorcol==COL_BIN && binmode) {
              /* bit toggle */
              int c,m;
              clearerr(f);
              SHED_FSEEK(f,cursorrow,SEEK_SET);
              c = fgetc(f);
              clearerr(f);
              SHED_FSEEK(f,cursorrow,SEEK_SET);
              m = 0x80 >> binmodecursor;
              fputc(c&m?c&(~m):c|m,f);
            } else {
              if(cursorcol&&editsize>1)
                sprintf(s,"new value [%d bit %s] (%s)",editsize*8,bigendianmode?"b.e.":"l.e.",coltitle[cursorcol]);
              else
                sprintf(s,"new value (%s)",coltitle[cursorcol]);
              getinput(s,0,0,cursorcol);
              if(!reply)
                break; /* input cancelled */
              clearerr(f);
              SHED_FSEEK(f,cursorrow,SEEK_SET);
              if(!cursorcol)
                fputc((int)reply[0],f); /* ascii column */
              else {
                int c;
                int64_t c64 = parsestring(reply,colbase[cursorcol]);
                int64_t m = 1;
                m <<= editsize * 8;
                if(c64<0||c64>m-1)
                  break;
                for(i=1;i<=editsize;i++) {
                  if(bigendianmode)
                    c = 0xff & (c64 >> ((editsize-i)*8));
                  else
                    c = 0xff & (c64 >> ((i-1)*8));
                  fputc(c,f);
                }
                break;
              }
            }
            break;

          /* toggle endianness */
          case '`':
            bigendianmode = !bigendianmode;
            break;

          /* exit */
          case 'X':
          case 24:  /* ^X */
            finish(0);
            break;

          /* jump to */
          case 'J':
            sprintf(s,"jump to (%s)",decmode?"dec":"hex");
            getinput(s,0,0,1);
            if(!reply)
              break;
            if(strequ(reply,"top"))
              lenarg = 0;
            else if(strequ(reply,"end")) {
              if(len)
                lenarg = len - 1;
              else {
                lenarg = cursorrow;
                beep();
              }
            } else {
              i = 1;
              switch(reply[strlen(reply)-1]) {
/*              case '%':
                  break;*/
                case 'G':
                case 'g':
                  i *= 1024;
                case 'M':
                case 'm':
                  i *= 1024;
                case 'K':
                case 'k':
                  i *= 1024;
                  reply[strlen(reply)-1]=0;
                  break;
                default:
                  break;
              }
              lenarg = i * parsestring(reply,decmode?10:16);
              if(lenarg<0)
                break;
              if(len && lenarg>=len)
                lenarg = len-1;
            }
            cursorjump(lenarg);
            break;

          /* repeat search */
          case 267: /* F3 */
          case 'R':
          case 'N':
            if(searchstr[cursorcol]) {
              search("");
              break;
            }
            /* drop thru */
            if(0)

          /* ^B/^F search */
          case 2:
          case 6:
            searchbackwards = key == 2;
            /* drop thru */

          /* Normal search */
          case 'S':
          case 23: /* ^W */
          case 'W':
          case 'F':
          case '/':
            sprintf(s,"search for (%s)",coltitle[cursorcol]);
            if(searchstr[cursorcol])
              sprintf(s+strlen(s),"[%s]",searchstr[cursorcol]);
            getinput(s,INPUTSTATE_GETSTR_SEARCH,1,0);
            search(reply);
            break;

          /* toggle dec/hex */
          case 'T':
            decmode = !decmode;
            break;

          /* ascii mode change */
          case 'A':
            ascmode++;
            if(ascmode>2)
              ascmode=0;
            break;

          /* preview */
          case 'P':
            preview = !preview;
            if(preview)
              editsize = 1;
            break;

          /* dump */
          case 'D':
            sprintf(s,"dump to file");
            getinput(s,0,0,0);
            if(!reply)
              break;
            dump(reply);
            break;

          /* bit edit mode */
          case 'B':
            binmode = !binmode;
            binmodecursor = 0;
            break;

          case 'L':
            if(isdevice) {
              sprintf(s,"length");
              getinput(s,0,0,0);
              if(!reply)
                break;
              len = atoi(reply);
            } else
              beep();
            break;

          /* set edit size */
          case '1':
          case '2':
          case '4':
            editsize = key - '0';
            if(editsize>1)
              preview = 0;
            cursoradjust();
            break;

          /* redraw */
          case 12: /* ^L */
            erase();
            refresh();
            break;

          /* resize */
          case KEY_RESIZE:
            refresh();
            viewsize = LINES - 6;
            if(cursorrow>=viewport+viewsize)
              viewport = cursorrow;
            if(viewport+viewsize>len) {
              while(viewport+viewsize>len)
                cursorup(1);
            }
            cursordown(viewsize);
            for(i=1;i<LINES-1;i++)
              clearline(i,COLS);
            refresh();
            break;

          default:
            break;
        }
        break;
    }
  }
  finish(0);
}



/* functions */

/* ends ncurses and quits */
void finish(int s) {
  endwin();
  printf("\n");
  exit(s);
}

/* handles ctrl c */
void ctrlc(int s) {
  ungetch(3);
}

/* cursor movements functions */

#define calccursorsize() (cursorcol?((cursorcol==COL_BIN&&binmode)?1:editsize):1)

void cursorup(int n) {
  while(n--) {
    if(cursorrow) {
      cursorrow--;
      if(cursorrow<viewport)
        viewport--;
    }
    else
      beep();
  }
}

void cursordown(int n) {
  int cs = calccursorsize();
  while(n--) {
    if(cursorrow<len-cs || !len) {
      cursorrow++;
      while(cursorrow+(cs-1)>=viewport+viewsize)
        viewport++;
    }
    else
      beep();
  }
}

void cursorleft() {
  if(cursorcol==COL_BIN) {
    if(binmode && binmodecursor)
      binmodecursor--;
    else
      cursorcol--;
  }
  else if(cursorcol)
    cursorcol--;
  else {
    beep();
    return;
  }
  cursoradjust();
}

void cursorright() {
  if(cursorcol<COL_BIN)
    cursorcol++;
  else {
    if(binmode && binmodecursor<7)
        binmodecursor++;
    else {
      beep();
      return;
    }
  }
  cursoradjust();
}

void cursoradjust() {
  int cs = calccursorsize();
  while(cursorrow>len-cs)
    cursorup(1);
  while(cursorrow+(cs-1)>=viewport+viewsize)
    viewport++;
}

/* clears a line on the screen */
void clearline(int line, int width) {
#if SHED_CYGFIX == 1
  width--;
#endif
  move(line,0);
  while(width--)
    addch(' ');
}

/* search */
#define search_char_cmp(c1,c2) ((searchinsensitive && isalpha((c1))) ? tolower((c1))==(c2) || toupper((c1))==(c2) : (c1) == (c2))

int search(char *str) {

  int c;
  int i,slen;
  unsigned char *search = NULL;
  long l;

  if(!str)
    return 0;

  if(strlen(str)) {
    /* user entered a string, so make a copy to searchstr for repeat searches. */
    free(searchstr[cursorcol]);
    searchstr[cursorcol] = strdup(str);
  } else if(!searchstr[cursorcol]) {
    /* else user just pressed enter, but no previous search */
    return 0;
  }

  slen = strlen(searchstr[cursorcol]);
  search = malloc(slen+2);
  strcpy((char*)search,searchstr[cursorcol]);

  if(cursorcol) {
    /* parse string */
    char *p = malloc(slen+2);
    strcpy(p,(char*)search);
    p = strtok(p," :,.\0");
    for(i=0;p;i++) {
      l = parsestring(p,colbase[cursorcol]);
      if(l<0 || l>255)
        return 0;
      search[i] = (unsigned char)l;
      p = strtok(NULL," :,.\0");
    }
    free(p);
    search[i] = 0;
    slen = i;
  }

  clearerr(f);

  /* backwards */
  if(searchbackwards) {
#ifdef LOG
    fprintf(SHEDLOG,"search: backwards for '%s'\n",search);
#endif
    cursorup(1);
    SHED_FSEEK(f,cursorrow-1,SEEK_SET);
    for(;cursorrow;cursorup(1)) {
      c = fgetc(f);
#ifdef LOG
      fprintf(SHEDLOG,"search: %d: c=%d(%c)\n",cursorrow,c,isprint(c)?c:'.');
#endif
      if(search_char_cmp((unsigned char)c,search[0])) {
#ifdef LOG
        fprintf(SHEDLOG,"search:   first letter match:\n");
#endif
        for(i=1;i<slen;i++) {
#ifdef LOG
          fprintf(SHEDLOG,"search:     c=%d(%c)\n",c,isprint(c)?c:'.');
#endif
          c = fgetc(f);
          if(c==EOF)
            break;
          if(!search_char_cmp(c,search[i]))
            break;
        }
        if(i==slen) {
          cursorup(i-1);
          cursordown(i-1);
          free(search);
          return 1;
        }
      }
      clearerr(f);
      SHED_FSEEK(f,cursorrow-1,SEEK_SET);
    }

  /* forwards */
  } else {
    cursordown(1);
    SHED_FSEEK(f,cursorrow,SEEK_SET);
    for(;cursorrow<len-1;cursordown(1)) {
      c = fgetc(f);
      if(search_char_cmp((unsigned char)c,search[0])) {
        for(i=1;i<slen;i++) {
          c = fgetc(f);
          if(c==EOF)
            break;
          if(!search_char_cmp(c,search[i]))
            break;
        }
        if(i==slen) {
          cursordown(i-1);
          cursorup(i-1);
          free(search);
          return 1;
        }
        clearerr(f);
        SHED_FSEEK(f,cursorrow+1,SEEK_SET);
      }
    }
  }

  free(search);
  return 0;
}

int cursorjump(off_t n) {

  if(cursorrow>n) {
    if(n<viewport || n>viewport+viewsize) {
      cursorrow = n;
      viewport = cursorrow;
    } else
      cursorup(cursorrow-n);
  } else {
    if(n<viewport || n>viewport+viewsize) {
      cursorrow = n;
      viewport = cursorrow - (viewsize - 1);
    } else
      cursordown(n-cursorrow);
  }
  return 0;
}

int redraw() {

  int i,c,n;
  char str[256];

  /* redraw top */
  attron(A_REVERSE);
  clearline(0,COLS);
  mvprintw(0,0,"%s%s%s",filename,filetype,readonly?" (read only)":"");
  mvprintw(0,COLS-10,"shed %s",VERSION);
  attroff(A_REVERSE);

  /* draw column headers */
  mvprintw(1,0,"%s",(offsetwidth==4)?"offs":"offset");
  move(1,offsetwidth+2);
  if(hlo)
    printw("hlo ");
  printw("asc hex dec oct bin      %s",preview?"preview":"       ");

  /* seek to current part of file and display */
  clearerr(f);
  for(i=0;i<viewsize;i++) {
    SHED_FSEEK(f,viewport+i,SEEK_SET);
    c = fgetc(f);
    if(c==EOF)
      break;
    mvprintw(i+2,0,"%s: ",getstring(viewport+i,str,(decmode)?10:16,offsetwidth));
    if(hlo)
      printw(" %c  ",((viewport+i)%hlo)?' ':hloc);
    printw("%s ",getascii(c,str,ascmode));
    printw("%s  ",getstring(c,str,16,2));
    printw("%s ",getstring(c,str,10,3));
    printw("%s ",getstring(c,str, 8,3));
    printw("%s ",getstring(c,str, 2,8));
    n = offsetwidth + coloffset[COL_BIN] + colwidth[COL_BIN] + 1;
    move(i+2,n);
    for(;n<COLS;n++)
      addch(' ');
  }

  /* draw cursor */
  clearerr(f);
  attron(A_REVERSE);
  SHED_FSEEK(f,cursorrow,SEEK_SET);
  for(i=0;i<editsize;i++) {
    n = (cursorrow-viewport) + 2 + i;
    c = fgetc(f);
    if(!cursorcol) {
      mvaddch(n,offsetwidth+3,isprint(c)?c:' ');
      break;
    } else {
      getstring(c,str,colbase[cursorcol],colwidth[cursorcol]);
      if(cursorcol==COL_BIN && binmode) {
        mvprintw(n,offsetwidth+coloffset[cursorcol]+binmodecursor,"%C",str[binmodecursor]);
        break;
      } else
        mvaddstr(n,offsetwidth+coloffset[cursorcol],str);
    }
  }
  attroff(A_REVERSE);

  /* draw preview */
  if(preview) {
    SHED_FSEEK(f,cursorrow,SEEK_SET);
    n = offsetwidth + coloffset[COL_BIN] + colwidth[COL_BIN] + 1;
    move((cursorrow-viewport)+2,n);
    for(i=0;i<32;i++) {
      c = fgetc(f);
      if(c==EOF)
        break;
      addch(isprint(c)?c:'.');
    }
  }
  else if(inputstate==INPUTSTATE_MAIN) {
    /* draw multi-byte value */
    if(editsize>1 && cursorcol && !(cursorcol==COL_BIN&&binmode)) {
      uint32_t v=0,c32;
      SHED_FSEEK(f,cursorrow,SEEK_SET);
      for(i=1;i<=editsize;i++) {
        c = fgetc(f);
        if(c>=0) {
          c32 = c;
          if(bigendianmode)
            v += c32 << ((editsize-i)*8);
          else
            v += c32 << ((i-1)*8);
        }
      }
      move((cursorrow-viewport)+2,offsetwidth + coloffset[COL_BIN] + colwidth[COL_BIN] + 1);
      n = cursorcol==COL_HEX ? calcwidth((((uint64_t)1)<<(editsize*8))-1,colbase[cursorcol]) : 0;
      getstring(v,str,colbase[cursorcol],n);
      printw("%s (%d bit unsigned %s %s)",str,editsize*8,bigendianmode?"b.e.":"l.e.",coltitle[cursorcol]);
    }
  }

  /* draw status line */
  attron(A_REVERSE);
  clearline(LINES-3,COLS);
  if(message)
    mvaddstr(LINES-3,0,message);
  else if(inputstate==INPUTSTATE_MAIN) {
    /* draw (end) */
    if(cursorrow==len-calccursorsize())
      mvaddstr(LINES-3,0,"(end)");
  }

  c = decmode?10:16;
  mvaddstr(LINES-3,COLS-(calcwidth(cursorrow,c)+calcwidth(len,c)+8),getstring(cursorrow,str,c,0));
  addstr("/");
  addstr(getstring(len,str,c,0));
  addstr((decmode)?" (dec)":" (hex)");
  attroff(A_REVERSE);

  /* draw key help */
  redraw_key_help(0);

  refresh();
  return 0;
}


#define ONOFF(s) (s?"on":"off")

/* factored out of redraw so could be called during INPUTSTATE_GETSTR* */
int redraw_key_help(int onlykeyhelp) {
  if(onlykeyhelp)
    attroff(A_REVERSE);
  clearline(LINES-2,COLS);
  clearline(LINES-1,COLS);
  switch(inputstate) {
    case INPUTSTATE_MAIN:
      mvaddstr(LINES-2,0,"SPACE|E edit  S|W|F search  J jump to   T dec/hex   D dump     1|2|4 cursor");
      mvaddstr(LINES-1,0,"X       exit  R|N   repeat  B bin edit  A ext. asc  P preview  `     endian");
      break;
    case INPUTSTATE_GETSTR:
      mvaddstr(LINES-2,0,"ENTER accept");
      mvaddstr(LINES-1,0,"^C    cancel");
      break;
    case INPUTSTATE_GETSTR_HEX:
      mvaddstr(LINES-2,0,"ENTER accept  T toggle");
      mvaddstr(LINES-1,0,"^C    cancel");
      break;
    case INPUTSTATE_GETSTR_SEARCH:
      mvprintw(LINES-2,0,"ENTER accept  ^B search backwards (%s)",ONOFF(searchbackwards));
      mvprintw(LINES-1,0,"^C    cancel");
      if(!cursorcol)
        printw("  ^I ignore case (%s)",ONOFF(searchinsensitive));
      break;
    default:
      break;
  }
  if(onlykeyhelp) {
    attron(A_REVERSE);
    move(LINES-3,strlen(message)+strlen(reply));
    curs_set(1);
  }
  else {
    mvaddstr(LINES-1,COLS-1," ");
    curs_set(0);
  }
  return 0;
}


/* gets user input */
int getinput(char *prompt, int state, int acceptemptystr, int hexonly) {
  inputstate = state ? state : INPUTSTATE_GETSTR; //hexonly ? 2 : 1;
  message = malloc(strlen(prompt)+5);
  if(!message)
    return -1;
  sprintf(message,"%s: ",prompt);
  redraw();
  move(LINES-3,strlen(message));
  curs_set(1);
  attron(A_REVERSE);
  free(reply);
  reply = malloc(128);
  if(!reply)
    return -1;
  memset(reply,0,128);
  if(!mainloop())
    if(acceptemptystr||strlen(reply)) {
      attroff(A_REVERSE);
      return 0;
    }
  attroff(A_REVERSE);
  free(reply);
  reply = NULL;
  return -1;
}


/* dump shed output to another file */
int dump(char *dumpfile) {

  int i,c;
  FILE *df;
  char str[32];

  df = fopen(reply,"w");
  if(!df)
    return -1;

  attron(A_REVERSE);
  mvaddstr(LINES-3,0,"dumping...");
  mvprintw(LINES-3,11+offsetwidth,"/%s",getstring(len,str,decmode?10:16,offsetwidth));

  fprintf(df,"%s%s",((offsetwidth==4)?"offs":"offset"),"  asc hex dec oct bin\n");
  rewind(f);
  for(i=0;i<len;i++) {
    c = fgetc(f);
    if(c<0)
      break;
    if(!(i%1024)) {
      mvaddstr(LINES-3,11,getstring(i,str,decmode?10:16,offsetwidth));
      refresh();
    }
    fprintf(df,"%s:  ",getstring(viewport+i,str,decmode?10:16,offsetwidth));
    fprintf(df,"%c  ",(char)((c>32&&c<127)?c:' '));
    fprintf(df,"%s  ",getstring(c,str,16,2));
    fprintf(df,"%s ",getstring(c,str,10,3));
    fprintf(df,"%s ",getstring(c,str, 8,3));
    fprintf(df,"%s\n",getstring(c,str, 2,8));
  }
  attroff(A_REVERSE);
  fclose(df);
  return 0;
}
