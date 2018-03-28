#include <netdb.h>
#include <errno.h>
#include <ifaddrs.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <zmq.h>

#include "c60_internal.h"
#include "utarray.h"


static int is_me(char *uri,UT_array *ips,UT_string *err) {
  char *c,*colon,*host,*ip_str;
  struct hostent *h;
  int rc=-1;

  if (strncmp(uri,"tcp://",6)) {
    utstring_printf(err,"unsupported transport %s\n",uri);
    goto done;
  }
  c = &uri[6];
  colon = strchr(uri,':');
  if (!colon) {
    utstring_printf(err,"no port on transport %s\n",uri);
    goto done;
  }

  host = strndup(uri,colon-c);
  h = gethostbyname(host);
  free(host);
  if (!h) {
    utstring_printf(err,"cannot resolve %s: %s\n",uri,hstrerror(h_errno));
    goto done;
  }
  ip_str = inet_ntoa(*(struct in_addr*)h->h_addr);
  // TODO find ip_str in IPs

 done:
  // TODO clean up?
  return rc;
}

static void get_ips(UT_array *ips) {
  struct ifaddrs *ifaddr, *ifa;
  char host[NI_MAXHOST],*hostp=host,**hp;
  int family,s;

  if (getifaddrs(&ifaddr) == -1) return;
  for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next) {
     family = ifa->ifa_addr->sa_family;
     if (! ((family == AF_INET) || (family == AF_INET6))) continue; 
     s = getnameinfo(ifa->ifa_addr,
                 (family == AF_INET) ? sizeof(struct sockaddr_in) :
                                       sizeof(struct sockaddr_in6),
                 host, NI_MAXHOST, NULL, 0, NI_NUMERICHOST);
     if (s != 0) continue;
     utarray_push_back(ips,&hostp);
  }
#if 1
  hp=NULL;
  while ( (hp=(char**)utarray_next(ips,hp))) {
    hostp = *hp;
    fprintf(stderr,"--> %s\n",hostp);
  }
#endif
}

/* read map from file, bring up PULL sockets on our IP's */
void *c60_server_init_fromfile(char *file, UT_string *err) {
  char line[100], cmd[100], uri[100]; /* TODO safer parse */
  unsigned start, end, i;
  c60_server_t *c60s;
  int rc, lines=0;
  FILE *f;
  void *s;

  UT_array *ips;
  utarray_new(ips,&ut_str_icd);
  get_ips(ips);
  exit(-1);

  if ( (c60s = calloc(1,sizeof(*c60s))) == NULL) goto done;
  if ( (c60s->c60.zcontext = zmq_init(1)) == NULL) goto done;

  if ( (f = fopen(file,"r")) == NULL) {
    utstring_printf(err,"can't open %s: %s\n", file, strerror(errno));
    goto done;
  }
 
  while (fgets(line,sizeof(line),f) != NULL) {
    lines++;
    if (sscanf(line,"%s %u %u %s", cmd, &start, &end, uri) != 4) {
      utstring_printf(err,"%s: syntax error at line %u\n", file, lines);
      goto done;
    }
    if (strcmp(cmd, "assign")) {
      utstring_printf(err,"%s: unknown command at line %u\n", file, lines);
      goto done;
    }
    if (((start < 0) || (start >= C60_NUM_BUCKETS)) || 
        ((end < 0)   || (end >= C60_NUM_BUCKETS)) || 
        (end < start)) {
      utstring_printf(err,"%s: invalid range at line %u\n", file, lines);
      goto done;
    }

    if ( (rc = is_me(uri,ips,err)) == -1) goto done;
    else if (rc == 0) continue;

    // TODO stick s in structure
    if ( (s = zmq_socket(c60s->c60.zcontext, ZMQ_PULL)) == NULL) {
      utstring_printf(err,"%s: can't open socket on %s: %s\n", file, uri,
        zmq_strerror(errno));
      goto done;
    }
    char *xlated_uri = "TODO"; // TODO
    if (zmq_bind(s, xlated_uri) != 0) {
      utstring_printf(err,"%s: can't connect to %s: %s\n", file, xlated_uri,
        zmq_strerror(errno));
      goto done;
    }
  }
  rc = 0; /* success */

 done:
  if (f) fclose(f);
  utarray_free(ips);
  return NULL; /* TODO  */
}

/* establish callback for messages of given prefix (NULL implies any message) */
typedef void (c60_handler)(int a); // TODO
void c60_server_set_handler(void *c60, char *prefix, size_t len, c60_handler *handler) {
}

/* enter run loop, waiting for messages */
int c60_run(void *handle) {
}
