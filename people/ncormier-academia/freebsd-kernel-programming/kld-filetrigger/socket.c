/*
** socket.c for  in /home/nico/lang/kernel/kld-filetrigger
** 
** Made by nicolas
*/

/* Headers
 */
#ifndef _KERNEL
# include <stdlib.h>
# include <unistd.h>
# include <string.h>
# include <fcntl.h>
# include <sys/ioctl.h>
# include <strings.h>
# include <sys/types.h>
# include <sys/socket.h>
# include <netinet/in.h>
# include <arpa/inet.h>
#else
# include <sys/types.h>
# include <sys/stat.h>
# include <sys/param.h>
# include <sys/systm.h>
# include <sys/kernel.h>
# include <sys/kthread.h>
# include <sys/namei.h>
# include <sys/proc.h>
# include <sys/socket.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <sys/malloc.h>
# include <sys/socketvar.h>
# include <sys/libkern.h>
# include <sys/protosw.h>
# include "kernel-dependant.h"
#endif /* _KERNEL */
#include "socket.h"
#include "die.h"

/* Forward declarations
 */
static int		socket_init_listening_socket_on_port(socket_t* self, unsigned port);
static int		socket_init_and_connect_to(socket_t* self, const char* addr, unsigned port);
static void		socket_destroy(socket_t* self);
static socket_t*	socket_accept(socket_t* self);
static int		socket_has_something_to_read(socket_t* self);
static int		socket_read(socket_t* self, char* buf, unsigned size);
static int		socket_write(socket_t* self, char* buf, unsigned size);

/* Extern functions
 */
socket_t*	alloc_socket(void)
{
  struct pv* pvdata = malloc(sizeof(struct pv));
  if (!pvdata)
    return NULL;
  bzero(pvdata, sizeof(struct pv));
  socket_t* sock = malloc(sizeof(socket_t));
  if (!sock)
    {
      free(pvdata);
      return NULL;
    }
  bzero(sock, sizeof(socket_t));
  /* Init private data
   */
  sock->pvdata = pvdata;
  /* Init methods
   */
  sock->init_listening_socket_on_port = socket_init_listening_socket_on_port;
  sock->init_and_connect_to = socket_init_and_connect_to;
  sock->destroy = socket_destroy;
  sock->accept = socket_accept;
  sock->has_something_to_read = socket_has_something_to_read;
  sock->read = socket_read;
  sock->write = socket_write;
  return sock;
}

/* Local functions
 */
#ifdef _KERNEL
static unsigned int kern_inet_addr(const char *str)
{
  int a,b,c,d;
  char arr[4];
  sscanf(str,"%d.%d.%d.%d",&a,&b,&c,&d);
  arr[0] = a; arr[1] = b; arr[2] = c; arr[3] = d;
  return *(unsigned int*)arr;
}
#endif /* _KERNEL */

static int		socket_init_listening_socket_on_port(socket_t* self, unsigned port)
{
#ifndef _KERNEL
  /* Create socket and bind it
   */
  int s = -1;
  struct sockaddr_in sin;
  bzero(&sin, sizeof(struct sockaddr_in));
  sin.sin_family = AF_INET;
  sin.sin_port = htons(port);
  sin.sin_addr.s_addr = INADDR_ANY;
  s = socket(AF_INET, SOCK_STREAM, 0);
  if (s == -1)
    return -1;
  /* Set SO_REUSEADDR option to yes
   */
  int yes = 1;
  int set_opt = setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes));
  if (set_opt == -1)
    {
      (void) close(s);
      return -1;
    }
  int fcntl_ret = fcntl(s, F_SETFL, O_NONBLOCK); /* Put socket in non-blocking mode */
  if (fcntl_ret == -1)
    {
      (void) close(s);
      return -1;
    }
  int bind_ret = bind(s, (const struct sockaddr*) &sin, sizeof(sin));
  if (bind_ret == -1)
    {
      (void) close(s);
      return -1;
    }
  int listen_ret = listen(s, SOMAXCONN);
  if (listen_ret == -1)
    {
      (void) close(s);
      return -1;
    }
  self->pvdata->sock = s; /* Assign */
#else
  struct thread *td = curthread;
  struct socket* sock;
  int error = socreate(AF_INET, &sock, SOCK_STREAM, 0, td->td_ucred, td);
  if (error)
    return -1;
  //sosetopt(sock, SOL_SOCKET, SO_REUSEADDR, m);
  struct sockaddr_in sin;
  bzero(&sin, sizeof(struct sockaddr_in));
  sin.sin_len = sizeof(sin);
  sin.sin_family = AF_INET;
  sin.sin_port = htons(port);
  sin.sin_addr.s_addr = INADDR_ANY;
  error = sobind(sock, (struct sockaddr*)&sin, td);
  if (error)
    {
      (void) soclose(sock);
      return -1;
    }
  error = solisten(sock, SOMAXCONN, td);
  if (error)
    {
      (void) soclose(sock);
      return -1;
    }
  self->pvdata->sock = sock; /* Assign */
#endif /* _KERNEL */
  return 0;
}

static int		socket_init_and_connect_to(socket_t* self, const char* addr, unsigned port)
{
#ifndef _KERNEL
  /* Create socket and connect
   */
  int sock = socket(PF_INET, SOCK_STREAM, 0);
  if (sock == -1)
    return -1;
  int fcntl_ret = fcntl(sock, F_SETFL, O_NONBLOCK); /* Put socket in non-blocking mode */
  if (fcntl_ret == -1)
    {
      (void) close(sock);
      return -1;
    }
  struct sockaddr_in    srv_info;
  bzero(&srv_info, sizeof(struct sockaddr_in));
  srv_info.sin_family = AF_INET;
  srv_info.sin_port = htons(port);
  srv_info.sin_addr.s_addr = inet_addr(addr);
  if (connect(sock, (struct sockaddr*) &srv_info, sizeof(srv_info)) == -1)
    {
      (void) close(sock);
      return -1;
    }
  self->pvdata->sock = sock; /* Assign */
#else
  struct thread *td = curthread;
  struct socket* sock;
  int error = socreate(AF_INET, &sock, SOCK_STREAM, 0, td->td_ucred, td);
  if (error)
    return -1;
  //sosetopt(sock, SOL_SOCKET, SO_REUSEADDR, m);
  struct sockaddr_in sin;
  bzero(&sin, sizeof(struct sockaddr_in));
  sin.sin_len = sizeof(sin);
  sin.sin_family = AF_INET;
  sin.sin_port = htons(port);
  sin.sin_addr.s_addr = kern_inet_addr(addr);
  error = soconnect(sock, (struct sockaddr*) &sin, td);
  if (error)
    {
      (void) soclose(sock);
      return -1;
    }
  self->pvdata->sock = sock; /* Assign */
#endif /* _KERNEL */
  return 0;
}

static void		socket_destroy(socket_t* self)
{
  /* Cleanup
   */
#ifndef _KERNEL
  (void) close(self->pvdata->sock);
#else
  if (self->pvdata->sock)
    (void) soclose(self->pvdata->sock);
#endif /* _KERNEL */
  if (self->pvdata)
    free(self->pvdata);
  free(self);
}

static socket_t*	socket_accept(socket_t* self)
{
#ifndef _KERNEL
  int sock = accept(self->pvdata->sock, NULL, NULL);
  if (sock == -1)
    return NULL;
  int fcntl_ret = fcntl(sock, F_SETFL, O_NONBLOCK); /* Put socket in non-blocking mode */
  if (fcntl_ret == -1)
    {
      (void) close(sock);
      return NULL;
    }
  socket_t* ret = alloc_socket();
  ret->pvdata->sock = sock;
  return ret;
#else
  if (!self->pvdata->sock) die("socket_accept: socket has not been initialized");
  if (!self->pvdata->sock->so_comp.tqh_first) /* Nothing to accept */
    return NULL;
  /* There is at least one connection ready to be accepted. 
   * Remove it from the queue.
   * http://www.koders.com/c/fid6071BDF712A86FB6ECA997E25D235644D14C8929.aspx?s=soaccept
   */
  struct socket* new_so = self->pvdata->sock->so_comp.tqh_first;
  TAILQ_REMOVE(&self->pvdata->sock->so_comp, new_so, so_list);
  self->pvdata->sock->so_qlen--;
  struct sockaddr* sa = NULL;
  (void) soaccept(new_so, &sa);
  if (sa) rfree(sa, M_SONAME);
  /* Alloc new socket
   */
  socket_t* ret = alloc_socket();
  if (!ret)
    return NULL;
  ret->pvdata->sock = new_so;
  return ret;
#endif /* _KERNEL */
}

static int		socket_has_something_to_read(socket_t* self)
{
#ifndef _KERNEL
  int to_read = 0;
  ioctl(self->pvdata->sock, FIONREAD, &to_read);
  return to_read;
#else
  if (!self->pvdata->sock) die("socket_has_something_to_read: socket has not been initialized");
  /* see /usr/src/sys/kern/sys_socket.c
   */
  return self->pvdata->sock->so_rcv.sb_cc;
#endif /* _KERNEL */
}

static int		socket_read(socket_t* self, char* buf, unsigned size)
{
#ifndef _KERNEL
  return read(self->pvdata->sock, (void*)buf, size);
#else
  if (!self->pvdata->sock) die("socket_read: socket has not been initialized");
  struct socket* so = self->pvdata->sock;
  struct thread *td = curthread;
  struct iovec aiov;
  aiov.iov_base = (caddr_t)buf;
  aiov.iov_len = size;
  struct uio auio;
  auio.uio_iov = &aiov;
  auio.uio_iovcnt = 1;
  auio.uio_offset = -1;
  auio.uio_resid = aiov.iov_len;
  auio.uio_rw = UIO_READ;
  auio.uio_segflg = UIO_SYSSPACE;
  auio.uio_td = td;
  int error = so->so_proto->pr_usrreqs->pru_soreceive(so, NULL, &auio, NULL, NULL, NULL);
  if (error)
    return -1;
  return size - auio.uio_resid;
#endif /* _KERNEL */
}

static int		socket_write(socket_t* self, char* buf, unsigned size)
{
#ifndef _KERNEL
  return write(self->pvdata->sock, buf, size);
#else
  if (!self->pvdata->sock) die("socket_write: socket has not been initialized");
  struct socket* so = self->pvdata->sock;
  struct thread *td = curthread;
  struct iovec aiov;
  aiov.iov_base = (caddr_t)buf;
  aiov.iov_len = size;
  struct uio auio;
  auio.uio_iov = &aiov;
  auio.uio_iovcnt = 1;
  auio.uio_offset = -1;
  auio.uio_resid = aiov.iov_len;
  auio.uio_rw = UIO_WRITE;
  auio.uio_segflg = UIO_SYSSPACE;
  auio.uio_td = td;
  int error = so->so_proto->pr_usrreqs->pru_sosend(so, NULL, &auio, NULL, NULL, 0, td);
  if (error)
    return -1;
  return size - auio.uio_resid;
#endif /* _KERNEL */
}
