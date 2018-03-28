#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/un.h>
#include <sys/types.h>
#include <fcntl.h>
#include <assert.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "tpl.h"
#include "utarray.h"

char *path = "socket"; 
char prompt[100];

void usage(char *prog) {
  fprintf(stderr, "usage: %s [-v] [-s|-S socket] [-f file]\n", prog);
  fprintf(stderr, "          -v verbose\n");  
  fprintf(stderr, "          -s path to UNIX domain socket\n");  
  fprintf(stderr, "          -S path to socket (abstract namespace)\n");
  fprintf(stderr, "          -f file to read commands from\n");
  exit(-1);
}

int verbose;
char *file;
FILE *filef;
char buf[256]; // max line length
int fd;        // connection to unix domain socket (control port)

/* the control port can became readable when we aren't expecting a response.
 * this probably means the control port is shutting down. check for this.
 * if found, confirm its EOF. any other I/O without EOF here would be a bug. */
int last_gasp(int fd) {
  char buf[100];
  fd_set rfds;
  int sr,rc;

  int flags;
  flags = fcntl(fd, F_GETFL);
  fcntl(fd, F_SETFL, flags | O_NONBLOCK);
  do {
    rc=read(fd,buf,sizeof(buf));
    if (rc > 0) fprintf(stderr,"control port: %.*s\n", rc, buf);
    else if (rc == 0) fprintf(stderr,"control port: closed\n");
  } while(rc > 0);
  fcntl(fd, F_SETFL, flags);

  return (rc==-1) ? 0 : 1;
}

char *next_line() {
  size_t len;
  char *line=NULL,*tmp;

  if (file) line=fgets(buf,sizeof(buf), filef);
  else {
    if (last_gasp(fd)) goto done; // before we block on stdin, see if cp closed
    line=readline(prompt);  // must free it
  }
  if (!line) goto done;

  len = strlen(line);
  
  if (file) { 
    /* fgets keeps trailing newline. null it out. if absent, line got truncated*/
    if (buf[len-1] == '\n') buf[len-1]='\0'; 
    else { fprintf(stderr, "line too long\n"); line=NULL; }
  } else {  
    /* copy the mallocd readline buffer and free it */
    tmp = line;
    if (len+1 < sizeof(buf)) {memcpy(buf, line, len+1); line=buf; }
    else { fprintf(stderr, "line too long\n"); line=NULL; }
    free(tmp); 
  }

 done:
  if (file && !line) fclose(filef);
  return line;
}

/* This little parsing function finds one word at a time from the
 * input line. It supports double quotes to group words together. */
const int ws[256] = {[' ']=1, ['\t']=1};
char *find_word(char *c, char **start, char **end) {
  int in_qot=0;
  while ((*c != '\0') && ws[*c]) c++; // skip leading whitespace
  if (*c == '"') { in_qot=1; c++; }
  *start=c;
  if (in_qot) {
    while ((*c != '\0') && (*c != '"')) c++;
    *end = c;
    if (*c == '"') { 
      in_qot=0; c++; 
      if ((*c != '\0') && !ws[*c]) {
        fprintf(stderr,"text follows quoted text without space\n"); return NULL;
      }
    }
    else {fprintf(stderr,"quote mismatch\n"); return NULL;}
  }
  else {
    while ((*c != '\0') && (*c != ' ')) {
      if (*c == '"') {fprintf(stderr,"start-quote within word\n"); return NULL; }
      c++;
    }
    *end = c;
  }
  return c;
}

char *slurp(char *file, size_t *len) {
  struct stat s;
  char *buf;
  int fd;
  if (stat(file, &s) == -1) {
      fprintf(stderr,"can't stat %s: %s\n", file, strerror(errno));
      return NULL;
  }
  *len = s.st_size;
  if ( (fd = open(file, O_RDONLY)) == -1) {
      fprintf(stderr,"can't open %s: %s\n", file, strerror(errno));
      return NULL;
  }
  buf = malloc(*len);
  if (buf) {
    if (read(fd, buf,*len) != *len) {
       fprintf(stderr,"read failure\n");
       free(buf); buf=NULL;
    }
  }
  close(fd);
  return buf;
}

/* this helper finds <File and >File words in the command.
 * It replaces <File with the contents of file or
 * records the >File so we can store output to it later.*/
int redir(tpl_bin *bbuf, UT_array *of, int *need_free) {
  char *c = (char*)bbuf->addr;
  char *file;
  int rc=1;

  *need_free = 0;
  if (bbuf->sz <= 1) goto done;
  if ((*c != '<') && (*c != '>')) goto done;

  file = strndup(c+1,bbuf->sz-1);
  if (*c == '>') { utarray_push_back(of, &file); rc=0; goto done;}
  if (*c == '<') {
    bbuf->addr = slurp(file, &bbuf->sz);
    *need_free = bbuf->addr ? 1 : 0;
    rc = bbuf->addr ? 1 : -1;
  }
  free(file);

 done:
  return rc;
}

void write_file(tpl_bin *bbuf, char *file) {
  int fd,rc;
  fd = open(file, O_WRONLY|O_TRUNC|O_CREAT, 0644);
  if (fd == -1) {
    fprintf(stderr,"failed to open %s: %s\n", file, strerror(errno));
    return;
  }
  rc = write(fd, bbuf->addr, bbuf->sz);
  if (rc == bbuf->sz) {
    if (verbose) fprintf(stderr,"Wrote %s: %zu bytes\n", file, bbuf->sz);
  } else {
    if (rc >=0) fprintf(stderr,"partial write to %s\n", file);
    else fprintf(stderr,"error writing to %s: %s\n", file, strerror(errno));
  }
  close(fd);
}

int do_rqst(char *line, int fd, int *cr) {
  char *c=line, *start=NULL, *end=NULL, **f, *file;
  tpl_node *tn=NULL,*tr=NULL;
  UT_array *of; /* optional output files for command response channels */
  int i, rc = -1, need_free, rr;
  tpl_bin bbuf;

  utarray_new(of, &ut_str_icd);
  tn = tpl_map("A(B)", &bbuf);
  *cr = 0;

  /* parse the line into argv style words, pack and transmit the request */
  while(*c != '\0') {
    if ( (c = find_word(c,&start,&end)) == NULL) goto done;
    //fprintf(stderr,"[%.*s]\n", (int)(end-start), start);
    assert(start && end);
    bbuf.addr =   start;
    bbuf.sz = end-start;
    if ( (rr = redir(&bbuf,of,&need_free)) > 0) tpl_pack(tn,1);
    else if (rr < 0) goto done; // rr==0 requires no action
    if (need_free) free(bbuf.addr);
    start = end = NULL;
  }
  if (tpl_dump(tn, TPL_FD, fd) == -1) goto done;

  /* get the reply */
  tr = tpl_map("iA(B)", cr, &bbuf);
  if (tpl_load(tr, TPL_FD, fd) == -1) {
    fprintf(stderr,"error receiving server response\n");
    goto done;
  }
  tpl_unpack(tr,0);
  if (*cr) printf("non-zero exit status: %d\n", *cr);
  if ( (i = tpl_Alen(tr,1)) > 1) printf("%u buffers received\n",i);

  /* unpack reply buffers. print the first. put the requested ones into files.*/
  for(i=0; tpl_unpack(tr,1) > 0; i++) {
    if (i==0 || verbose) printf("%.*s", (int)bbuf.sz, (char*)bbuf.addr);
    if ( (f = (char**)utarray_eltptr(of,i))) write_file(&bbuf,*f);
    if (bbuf.addr) free(bbuf.addr);
  }
  
  rc = 0;

 done:
  utarray_free(of);
  if (tn) tpl_free(tn);
  if (tr) tpl_free(tr);
  return rc;
}
 
int main(int argc, char *argv[]) {
  struct sockaddr_un addr;
  int opt,rc,cr;
  tpl_bin bbuf;
  char *line;

  while ( (opt = getopt(argc, argv, "v+f:s:S:")) != -1) {
    switch (opt) {
      case 'v': verbose++; break;
      case 'f': file = strdup(optarg); break;
      case 's': path = strdup(optarg); break;
      case 'S': path = calloc(strlen(optarg)+2,1); strcpy(path+1,optarg); break;
      default: usage(argv[0]); break;
    }
  }
  if (optind < argc) usage(argv[0]);
  snprintf(prompt,sizeof(prompt),"%s> ", path);
  using_history();

  if ( (fd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
    perror("socket error");
    exit(-1);
  }

  memset(&addr, 0, sizeof(addr));
  addr.sun_family = AF_UNIX;
  strncpy(addr.sun_path, path, sizeof(addr.sun_path)-1);

  if (connect(fd, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
    perror("connect error");
    exit(-1);
  }

  if (file && !(filef = fopen(file,"r"))) {
    perror("fopen error"); 
    exit(-1);
  }

  while ( (line=next_line()) != NULL) {
    add_history(line);
    if (do_rqst(line,fd,&cr) == -1) break;
  }

  clear_history();
  return 0;
}

