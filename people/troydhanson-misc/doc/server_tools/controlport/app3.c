/* test application for control port library */

#include <time.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <assert.h>
#include <dirent.h>
#include "controlport.h"
#include "utstring.h"

#define SOCKETNAME "./socket"

int put_cmd(void *cp, cp_arg_t *arg, void *data) {
  int fd=-1, rc = -1, n;
  char buf[100], *file, *body;
  size_t blen;

  if (arg->argc != 3) {
    snprintf(buf,sizeof(buf),"usage: %s name <name\n",arg->argv[0]);
    cp_add_reply(cp, buf, strlen(buf));
    goto done;
  } 

  file = arg->argv[1];
  body = arg->argv[2];
  blen = arg->lenv[2];

  fd = open(file, O_WRONLY|O_TRUNC|O_CREAT, 0700);
  if (fd == -1) {
    snprintf(buf,sizeof(buf),"open %s failed: %s\n", file, strerror(errno));
    cp_add_reply(cp, buf, strlen(buf));
    goto done;
  }
  if ( (n=write(fd, body, blen)) != blen) {
    snprintf(buf,sizeof(buf),"error writing to %s\n", file);
    cp_add_reply(cp, buf, strlen(buf));
    goto done;
  }

  snprintf(buf,sizeof(buf),"wrote %zu bytes to %s\n", blen, file);
  cp_add_reply(cp, buf, strlen(buf));
  rc = 0;

 done:
  if (fd != -1) close(fd);
  return rc;
}

int pwd_cmd(void *cp, cp_arg_t *arg, void *data) {
  char buf[100];
  size_t len;
  if (getcwd(buf,sizeof(buf)-1)) {
    len = strlen(buf);
    assert(len+1 < sizeof(buf));
    buf[len]='\n';
    buf[len+1]='\0';
    cp_add_reply(cp, buf, len+1);
  }
  return 0;
}

int dir_cmd(void *cp, cp_arg_t *arg, void *data) {
  struct dirent *dent;
  char buf[100];
  UT_string *s;
  int rc=0;
  DIR *d;

  utstring_new(s);
  char *dir = (arg->argc > 1) ? arg->argv[1] : ".";

  if ( (d = opendir(dir))) {
    while ( (dent = readdir(d))) {
      utstring_printf(s, "%s\n", dent->d_name);
    }
    cp_add_reply(cp, utstring_body(s), utstring_len(s));
  } else {
    snprintf(buf,sizeof(buf),"opendir failed: %s\n", strerror(errno));
    cp_add_reply(cp, buf, strlen(buf));
    rc=-1;
  }

  utstring_free(s);
  return rc;
}

int stat_cmd(void *cp, cp_arg_t *arg, void *data) {
  struct stat s;
  char buf[100];
  char *file;
  int i,rc;

  for(i=1; i < arg->argc; i++) {
    file = arg->argv[i];
    rc = stat(file, &s);
    snprintf(buf,sizeof(buf),"%s: %s : %zu bytes\n", file, 
      ((rc==0) ? "ok": "not found"), (size_t)((rc==0)?s.st_size:0));
    cp_add_reply(cp, buf, strlen(buf));
  }
  return 0;
}

cp_cmd_t cmds[] = {
  {"pwd",  pwd_cmd },
  {"stat", stat_cmd },
  {"dir",  dir_cmd },
  {"put",  put_cmd },
  {NULL, NULL},
};

char *chroot_dir = "/tmp";
int main(int argc, char *argv[]) {
  void *cp;
  if (argc > 1) chroot_dir = argv[1];
  cp = cp_init(SOCKETNAME, cmds, NULL, 0);
  if (chdir(chroot_dir) == -1) {
    fprintf(stderr,"chdir failed: %s\n", strerror(errno));
    return -1;
  }
  if (chroot(chroot_dir) == -1) {
    fprintf(stderr,"failed to chroot: %s\n", strerror(errno));
    return -1;
  }
  cp_run(cp);
}

