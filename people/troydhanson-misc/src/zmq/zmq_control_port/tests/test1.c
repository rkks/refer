#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <zmq.h>
#include "zcontrol.h"

#define ZCON_DEF_ENDPOINT "tcp://127.0.0.1:3333"
struct _CF {
  /* application state */
  int verbose;
  int request_exit;
  /* zmq related */
  void *zmq_context;
  void *zcontrol_socket;
  char *zcontrol_endpoint;
  void *zcontrol;
} CF = {
  .zcontrol_endpoint = ZCON_DEF_ENDPOINT,
};

int version_cmd(void *cp, cp_arg_t *arg, void *data) {
  int major, minor, patch;
  zmq_version (&major, &minor, &patch);
  cp_printf(cp,"Current 0MQ version is %d.%d.%d\n", major, minor, patch);
  return 0;
}

int shutdown_cmd(void *cp, cp_arg_t *arg, void *data) {
  cp_printf(cp,"Shutting down\n");
  CF.request_exit=1;
  return 0;
}

cp_cmd_t cmds[] = { 
  {"version",      version_cmd,      "version info"},
  {"shutdown",     shutdown_cmd,     "shutdown server"},
  {NULL,           NULL,             NULL},
};

void usage(char *prog) {
  fprintf(stderr, "usage: %s [-v] [-e endpoint]\n", prog);
  exit(-1);
}

int setup_zmq() {
  int zero=0;
  if (( (CF.zmq_context = zmq_init(1)) == NULL)                               ||
      ( (CF.zcontrol_socket  = zmq_socket(CF.zmq_context, ZMQ_REP )) == NULL) ||
      ( zmq_setsockopt(CF.zcontrol_socket , ZMQ_LINGER, &zero, sizeof(zero))) ||
      ( zmq_bind(CF.zcontrol_socket, CF.zcontrol_endpoint ) != 0)) {
    fprintf(stderr,"zeromq setup failed: %s\n", zmq_strerror(errno));
    return -1;
  }
  return 0;
}
 
#define adim(x) (sizeof(x)/sizeof(*x))

void msg_loop() {

  zmq_pollitem_t items [] = {
    { CF.zcontrol_socket, 0, ZMQ_POLLIN, 0 },
    /* add your other zmq sockets here */
  };

  while (CF.request_exit==0) {
    zmq_poll (items, adim(items), -1);

    if (items[0].revents & ZMQ_POLLIN) {
      cp_exec(CF.zcontrol, CF.zcontrol_socket);
    }

    /* handle other socket types here ... */
  }
}

int main(int argc, char *argv[]) {
  int opt;

  while ( (opt = getopt(argc, argv, "v+e:")) != -1) {
    switch (opt) {
      case 'v': CF.verbose++; break;
      case 'e': CF.zcontrol_endpoint=strdup(optarg); break;
      default: usage(argv[0]); break;
    }
  }

  if (setup_zmq() == -1) goto program_exit;
  CF.zcontrol = cp_init(cmds,NULL);

  msg_loop();

 program_exit:
  if (CF.zcontrol_socket) zmq_close(CF.zcontrol_socket);
  if (CF.zmq_context) zmq_term(CF.zmq_context);
  cp_free(CF.zcontrol);
  return 0;
}

