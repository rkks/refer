#include <assert.h>
#include <sys/un.h>
#include <sys/socket.h>
#include "controlport.h"
#include "utstring.h"
#include "tpl.h"
#include "uthash.h"

typedef struct {  // wraps a command structure 
  cp_cmd_t cmd;
  UT_hash_handle hh;
  void *data;
} cp_cmd_w;

typedef struct {

  struct sockaddr_un addr;

  cp_cmd_w *cmds; // hash table of commands
  int connects;   // number of accepts 
  int fd;         // listener
  int timeout;    // client inactivity timeout in sec TODO
  void *data;     // opaque data pointer passed into commands

  /* per client stuff. we're an iterative server, so one client at a time.
     so we can just keep the client state right here with server state */

  int cl;        // client fd
  cp_arg_t arg;  // current request from client, unpacked
  tpl_node *in;  // current request from client A(B)
  tpl_node *out; // current response to client iA(B)
  tpl_bin bbuf;  // temporary space for request/response
  int want_disconnect;  // callback wants to disconnect client
  int want_shutdown;    // callback wants to shutdown control port thread

} cp_t;

static int quit_cmd(void *cp, cp_arg_t *arg, void *data) {
  cp_disconnect(cp);
  return 0;
}

static int help_cmd(void *_cp, cp_arg_t *arg, void *data) {
  cp_t *cp = (cp_t*)_cp;
  UT_string *t;
  utstring_new(t);
  cp_cmd_w *cw, *tmp;
  HASH_ITER(hh, cp->cmds, cw, tmp) {
    utstring_printf(t, "%-20s ", cw->cmd.name);
    utstring_printf(t, "%s\n",    cw->cmd.help);
  }
  cp_add_reply(cp,utstring_body(t),utstring_len(t));
  utstring_free(t);
  return 0; 
}

static int unknown_cmd(void *cp, cp_arg_t *arg, void *data) {
  char unknown_msg[] = "command not found\n";
  if (arg->argc == 0) return 0;  /* no command; no-op */
  cp_add_reply(cp, unknown_msg, sizeof(unknown_msg)-1);
  return -1;
}
// unknown is not registered in the commands hash table
// so we point to this record if we need to invoke it.
static cp_cmd_w unknown_cmdw = {{"unknown",unknown_cmd}};

static int stop_cmd(void *_cp, cp_arg_t *arg, void *data) {
  cp_t *cp = (cp_t*)_cp;
  cp->want_shutdown=1;
  cp_disconnect(cp);
  return 0;
}


void *cp_init(char *path, cp_cmd_t *cmds, void *data, int timeout) {
  cp_cmd_t *cmd;
  cp_t *cp;
  
  if ( (cp=malloc(sizeof(cp_t))) == NULL) goto done;
  memset(cp, 0, sizeof(*cp));
  cp->timeout = timeout;
  cp->data = data;

  cp->addr.sun_family = AF_UNIX;
  strncpy(cp->addr.sun_path, path, sizeof(cp->addr.sun_path)-1);
  if (*path != '\0') unlink(path);

  if ( (cp->fd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
    perror("socket error"); free(cp); cp=NULL; goto done;
  }

  if (bind(cp->fd, (struct sockaddr*)&cp->addr, sizeof(cp->addr)) == -1) {
    perror("bind error"); close(cp->fd); free(cp); cp=NULL; goto done;
  }

  cp_add_cmd(cp, "help", help_cmd, "this text", NULL);
  cp_add_cmd(cp, "quit", quit_cmd, "close this session", NULL);
  cp_add_cmd(cp, "stop", stop_cmd, "shutdown control port", NULL);
  for(cmd=cmds; cmd && cmd->name; cmd++) {
    cp_add_cmd(cp,cmd->name,cmd->cmdf,cmd->help,data);
  }

 done:
  return cp;
}

void cp_add_cmd(void *_cp, char *name, cp_cmd_f *cmdf, char *help, void *data) {
  cp_t *cp = (cp_t*)_cp;
  cp_cmd_w *cw;

  /* create new command if it isn't in the hash; else update in place */
  HASH_FIND(hh, cp->cmds, name, strlen(name), cw);
  if (cw == NULL) {
    if ( (cw = malloc(sizeof(*cw))) == NULL) exit(-1);
    memset(cw,0,sizeof(*cw));
    cw->cmd.name = strdup(name);
    cw->cmd.help = help ? strdup(help) : strdup("");
    HASH_ADD_KEYPTR(hh, cp->cmds, cw->cmd.name, strlen(cw->cmd.name), cw);
  }
  cw->cmd.cmdf = cmdf;
  cw->data = data;
}

static void handle_client_request(cp_t *cp) {
  int rc, cr, n, i=0;
  cp_cmd_w *cw;
  void *tmp;

  assert (cp->in == NULL);
  assert (cp->out == NULL);
  assert (cp->arg.argc == 0);
  assert (cp->bbuf.addr == NULL);

  cp->in = tpl_map("A(B)", &cp->bbuf);
  rc = tpl_load(cp->in, TPL_FD, cp->cl);
  if (rc == -1) {cp->want_disconnect=1; goto done;} // EOF or input error 

  /* unpack the tpl into argc/argv/lenv for the command callback */
  if ( (n = tpl_Alen(cp->in,1)) == 0) {cw = &unknown_cmdw; goto run;}
  cp->arg.argc = n;
  cp->arg.argv = malloc( n*sizeof(char*));
  cp->arg.lenv = malloc( n*sizeof(size_t));
  if (!cp->arg.argv || !cp->arg.lenv) {cp->want_disconnect=1; goto done;}

  while(tpl_unpack(cp->in,1) > 0) {
    cp->arg.argv[i] = cp->bbuf.addr; 
    cp->arg.lenv[i] = cp->bbuf.sz;
    /* unfortunate baggage to null-terminate the argv */
    tmp = realloc(cp->arg.argv[i], cp->arg.lenv[i]+1);
    if (tmp) cp->arg.argv[i] = tmp;
    else {cp->want_disconnect=1; goto done;}
    cp->arg.argv[i][cp->arg.lenv[i]]='\0';
    i++;
  }

  /* find the command callback */
  HASH_FIND(hh, cp->cmds, cp->arg.argv[0], cp->arg.lenv[0], cw);
  if (!cw) cw = &unknown_cmdw;

  /* prepare the output area, run the callback */
 run:
  cp->bbuf.addr = NULL;
  cp->out = tpl_map("iA(B)", &cr, &cp->bbuf);
  cr = cw->cmd.cmdf(cp, &cp->arg, cw->data);

  /* send reply back to client */
  tpl_pack(cp->out,0);
  if (tpl_dump(cp->out, TPL_FD, cp->cl) == -1) {cp->want_disconnect=1; }

 done:
  if (cp->in) { tpl_free(cp->in); cp->in=NULL; }
  if (cp->out) { tpl_free(cp->out); cp->out=NULL; }
  while(cp->arg.argc) free(cp->arg.argv[--cp->arg.argc]);
  if (cp->arg.argv) { free(cp->arg.argv); cp->arg.argv=NULL; }
  if (cp->arg.lenv) { free(cp->arg.lenv); cp->arg.lenv=NULL; }
}

int cp_run(void *_cp) {
  cp_t *cp = (cp_t*)_cp;
  int rc=-1;

  if (listen(cp->fd, 5) == -1) { perror("listen error"); goto done; }

  while (!cp->want_shutdown) {

    if ( (cp->cl = accept(cp->fd, NULL, NULL)) == -1) {
      perror("accept error"); continue;
    }

    cp->connects++;
    cp->want_disconnect = 0;

    while(cp->cl != -1) {
      handle_client_request(cp);
      if (cp->want_disconnect && (cp->cl != -1)) { close(cp->cl); cp->cl=-1; }
    }
  }

  rc = 0;

 done:
  // free up the cmds hashtable, etc
  if (cp->fd != -1) { close(cp->fd); cp->fd = -1; }
  cp_cmd_w *cw, *tmp;
  HASH_ITER(hh, cp->cmds, cw, tmp) {
    HASH_DEL(cp->cmds, cw);
    free(cw->cmd.name);
    free(cw->cmd.help);
    free(cw);
  }
  free(cp);
  return rc;
}

/* this is intended for usage from another thread.  it is not particularly
 * graceful; an active client is severed, and it has race conditions. a 
 * better shutdown approach for a threaded app is in order */
void cp_shutdown(void *_cp) { 
  int fd;
  cp_t *cp = (cp_t*)_cp;
  if (cp->fd == -1) return;
  cp->want_shutdown=1;
  /* cause the cp_run thread to get an error return from accept() */
  fd = cp->fd; 
  cp->fd = -1;
  if (fd != -1) close(fd);  
  /* if the cp_run thread has a client connection, induce tpl_read failure */
  if (cp->cl != -1) {
    fd = cp->cl;
    cp->cl = -1;
    close(fd);
  }
}

void cp_add_reply(void *_cp, void *buf, size_t len) { 
  cp_t *cp = (cp_t*)_cp;
  assert(cp->bbuf.addr == NULL);
  assert(cp->out);

  cp->bbuf.addr = buf;
  cp->bbuf.sz = len;
  tpl_pack(cp->out, 1);

  cp->bbuf.addr = NULL;
}

void cp_disconnect(void *_cp) {
  cp_t *cp = (cp_t*)_cp;
  cp->want_disconnect=1;
}
