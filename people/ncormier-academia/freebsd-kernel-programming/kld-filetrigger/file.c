/*
** file.c for  in /home/nico/lang/kernel/kld-filetrigger
** 
** Made by nicolas
*/

/* Headers
 */
#ifndef _KERNEL
# include <stdlib.h>
# include <strings.h>
# include <unistd.h>
# include <fcntl.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <dirent.h>
# include <errno.h>
#else
# include <sys/types.h>
# include <sys/stat.h>
# include <sys/param.h>
# include <sys/systm.h>
# include <sys/kernel.h>
# include <sys/kthread.h>
# include <sys/namei.h>
# include <sys/proc.h>
# include <sys/filedesc.h>
# include <sys/vnode.h>
# include <sys/malloc.h>
# include <sys/unistd.h>
# include <sys/fcntl.h>
# include <sys/libkern.h>
# include <sys/dirent.h>
# include "kernel-dependant.h"
#endif /* _KERNEL */
#include "file.h"
#include "die.h"
#include "server.h"
#include "host.h"
#include "elem.h"

/* Forward declarations
 */
static int	file_init_with_path(file_t* self, const char* path, int flags, int cmode);
static void	file_destroy(file_t* self);
static int	file_read(file_t* self, unsigned offset, char* buf, unsigned size);
static int	file_write(file_t* self, unsigned offset, char* buf, unsigned size, void* pv);
static unsigned	file_size(file_t* self);
static int	file_truncate(file_t* self, unsigned new_size);
static int	dir_read(file_t* self, unsigned offset, char* buf, unsigned size);
static int	dir_write(file_t* self, unsigned offset, char* buf, unsigned size, void* pv);
static unsigned	dir_size(file_t* self);
static int	dir_truncate(file_t* self, unsigned new_size);

/* Extern functions
 */
file_t*	alloc_file(void)
{
  struct pv* pvdata = malloc(sizeof(struct pv));
  if (!pvdata)
    return NULL;
  bzero(pvdata, sizeof(struct pv));
  file_t* file = malloc(sizeof(file_t));
  if (!file)
    {
      free(pvdata);
      return NULL;
    }
  bzero(file, sizeof(file_t));
  /* Init private data
   */
  file->pvdata = pvdata;
  /* Init methods
   */
  file->init_with_path = file_init_with_path;
  file->destroy = file_destroy;
  file->read = file_read;
  file->write = file_write;
  file->size = file_size;
  file->truncate = file_truncate;
  return file;
}

/* Local functions
 */
static int	file_init_with_path(file_t* self, const char* path, int flags, int cmode)
{
  /* Open file
   */
  unsigned is_directory = 0;
#ifndef _KERNEL
  int fd = open(path, flags, cmode);
  DIR* dir = NULL;
  if (fd == -1)
    {
      dir = opendir(path);
      if (!dir)
	return -1;
      is_directory = 1;
    }
  char* dup_path = strdup(path);
  if (!dup_path)
    {
      if (dir)
	(void) closedir(dir);
      else
	(void) close(fd);
    }
  self->path = dup_path;
  if (dir)
    {
      self->pvdata->dir = dir;
      self->pvdata->fd = dir->dd_fd;
    }
  else
    self->pvdata->fd = fd;
#else
  char* dup_path = strdup(path);
  if (!dup_path)
    return -1;
  struct thread *td = curthread;
  struct nameidata nd;
  int error;
  if (td->td_proc->p_fd->fd_rdir == NULL)
    td->td_proc->p_fd->fd_rdir = rootvnode;
  if (td->td_proc->p_fd->fd_cdir == NULL)
    td->td_proc->p_fd->fd_cdir = rootvnode;
  flags = FFLAGS(flags);
  NDINIT(&nd, LOOKUP, NOFOLLOW, UIO_SYSSPACE, path, td);
  error = vn_open_cred(&nd, &flags, cmode, td->td_ucred, -1);
  NDFREE(&nd, NDF_ONLY_PNBUF);
  if (error)
    {
      free(dup_path);
      return -1;
    }
  /* We just unlock so we hold a reference.
   */
  VOP_UNLOCK(nd.ni_vp, 0, td);
  self->pvdata->vp = nd.ni_vp;
  if (self->pvdata->vp->v_type == VDIR) /* Check if file is a directory */
    is_directory = 1;
  self->path = dup_path;
#endif /* _KERNEL */
  if (is_directory)
    {
      /* Directory is a special file
       * Override methods with directory's methods
       */
      self->read = dir_read;
      self->write = dir_write;
      self->size = dir_size;
      self->truncate = dir_truncate;
    }
  return 0;
}

static void	file_destroy(file_t* self)
{
  /* Cleanup
   */
#ifndef _KERNEL
  if (self->pvdata->dir)
    (void) closedir(self->pvdata->dir);
  else
    (void) close(self->pvdata->fd);
#else
  if (self->pvdata->vp)
    {
      struct thread *td = curthread;
      (void) vn_close(self->pvdata->vp, FWRITE, td->td_ucred, td); /* TODO: juste FWRITE ?? */
    }
#endif /* _KERNEL */
  free(self->pvdata);
  if (self->path)
    free(self->path);
  free(self);
}

static int	file_read(file_t* self, unsigned offset, char* buf, unsigned size)
{
#ifndef _KERNEL
  if (lseek(self->pvdata->fd, offset, SEEK_SET) == -1)
    return -1;
  return read(self->pvdata->fd, (void*)buf, size);
#else
  if (!self->pvdata->vp) die("file_read: vnode has not been initialized");
  struct thread *td = curthread;
  struct uio auio;
  struct iovec aiov;
  /* Init structs
   */
  bzero(&aiov, sizeof(aiov));
  bzero(&auio, sizeof(auio));
  aiov.iov_base = (char*)buf;
  aiov.iov_len = size;
  auio.uio_iov = &aiov;
  auio.uio_offset = offset;
  auio.uio_segflg = UIO_SYSSPACE;
  auio.uio_rw = UIO_READ;
  auio.uio_iovcnt = 1;
  auio.uio_resid = size;
  auio.uio_td = td;
  /* Do all of the junk required to write now.
   */
  vn_lock(self->pvdata->vp, LK_EXCLUSIVE | LK_RETRY, td);
  VOP_LEASE(self->pvdata->vp, td, td->td_ucred, LEASE_READ);
  VOP_READ(self->pvdata->vp, &auio, IO_UNIT | IO_SYNC, td->td_ucred);
  VOP_UNLOCK(self->pvdata->vp, 0, td);
  return size; /* auio.uio_resid */
#endif /* _KERNEL */
}

static int	file_write(file_t* self, unsigned offset, char* buf, unsigned size, void* pv)
{
#ifndef _KERNEL
  if (lseek(self->pvdata->fd, offset, SEEK_SET) == -1)
    return -1;
  return write(self->pvdata->fd, buf, size);
#else
  if (!self->pvdata->vp) die("file_write: vnode has not been initialized");
  struct thread *td = curthread;
  struct mount *mp;
  struct uio auio;
  struct iovec aiov;
  /* Init structs
   */
  bzero(&aiov, sizeof(aiov));
  bzero(&auio, sizeof(auio));
  aiov.iov_base = buf;
  aiov.iov_len = size;
  auio.uio_iov = &aiov;
  auio.uio_offset = offset;
  auio.uio_segflg = UIO_SYSSPACE;
  auio.uio_rw = UIO_WRITE;
  auio.uio_iovcnt = 1;
  auio.uio_resid = size;
  auio.uio_td = td;
  /* Do all of the junk required to write now.
   */
  vn_start_write(self->pvdata->vp, &mp, V_WAIT);
  vn_lock(self->pvdata->vp, LK_EXCLUSIVE | LK_RETRY, td);
  VOP_LEASE(self->pvdata->vp, td, td->td_ucred, LEASE_WRITE);
  VOP_WRITE(self->pvdata->vp, &auio, IO_UNIT | IO_SYNC, td->td_ucred);
  VOP_UNLOCK(self->pvdata->vp, 0, td);
  vn_finished_write(mp);
  return size; /* auio.uio_resid */
#endif /* _KERNEL */
}

static unsigned	file_size(file_t* self)
{
#ifndef _KERNEL
  struct stat sb;
  if (fstat(self->pvdata->fd, &sb) == -1)
    return (unsigned)-1;
  return sb.st_size;
#else
  if (!self->pvdata->vp) die("file_size: vnode has not been initialized");
  struct stat sb;
  struct thread *td = curthread;
  vn_lock(self->pvdata->vp, LK_EXCLUSIVE | LK_RETRY, td); /* Needed ? */
  int error = vn_stat(self->pvdata->vp, &sb, td->td_ucred, NOCRED, td);
  VOP_UNLOCK(self->pvdata->vp, 0, td); /* Needed ? */
  if (error)
    return (unsigned)-1;
  return sb.st_size;
#endif /* _KERNEL */
}

static int	file_truncate(file_t* self, unsigned new_size)
{
#ifndef _KERNEL
  return ftruncate(self->pvdata->fd, new_size);
#else
  if (!self->pvdata->vp) die("file_truncate: vnode has not been initialized");
  struct thread *td = curthread;
  struct mount *mp;
  vn_start_write(self->pvdata->vp, &mp, V_WAIT);
  vn_lock(self->pvdata->vp, LK_EXCLUSIVE | LK_RETRY, td);
  VOP_LEASE(self->pvdata->vp, td, td->td_ucred, LEASE_WRITE);
  struct vattr vattr;
  VATTR_NULL(&vattr);
  vattr.va_size = new_size;
  int error = VOP_SETATTR(self->pvdata->vp, &vattr, td->td_ucred, td);
  VOP_UNLOCK(self->pvdata->vp, 0, td);
  vn_finished_write(mp);
  if (error)
    return -1;
  return 0;
#endif /* _KERNEL */
}

static int	mgetdents(file_t* self, char* buf, unsigned size, unsigned offset)
{
#ifndef _KERNEL
  if (lseek(self->pvdata->fd, offset, SEEK_SET) == -1) die("generate_dir_serialization: lseek failed");
  return getdents(self->pvdata->fd, buf, size);
#else
  if (!self->pvdata->vp) die("file_read: vnode has not been initialized");
  struct thread *td = curthread;
  struct uio auio;
  struct iovec aiov;
  /* Init structs
   */
  bzero(&aiov, sizeof(aiov));
  bzero(&auio, sizeof(auio));
  aiov.iov_base = (char*)buf;
  aiov.iov_len = size;
  auio.uio_iov = &aiov;
  auio.uio_offset = offset;
  auio.uio_segflg = UIO_SYSSPACE;
  auio.uio_rw = UIO_READ;
  auio.uio_iovcnt = 1;
  auio.uio_resid = size;
  auio.uio_td = td;
  /* Do all of the junk required to write now.
   */
  vn_lock(self->pvdata->vp, LK_EXCLUSIVE | LK_RETRY, td);
  VOP_LEASE(self->pvdata->vp, td, td->td_ucred, LEASE_READ);
  int error = VOP_READDIR(self->pvdata->vp, &auio, td->td_ucred, NULL, NULL, NULL);
  VOP_UNLOCK(self->pvdata->vp, 0, td);
  if (error)
    return -1;
  return size;
#endif /* _KERNEL */
}

static int	generate_dir_serialization(file_t* self, char* buf, unsigned size)
{
  unsigned buf_off = 0;
  char dirents_buf[512];
  unsigned getdents_off = 0;
  /* Loop till directory's eof
   */
  while (1)
    {
      int getdents_ret = mgetdents(self, dirents_buf, sizeof(dirents_buf), getdents_off);
      if (getdents_ret == -1) die("generate_dir_serialization: getdents failed");
      if (!getdents_ret)
	break;
      getdents_off += getdents_ret;
      /* Foreach entry read
       */
      char* cur = dirents_buf;
      while (cur < dirents_buf + getdents_ret)
	{
	  struct dirent* entry = (struct dirent*) cur;
	  if (strcmp(entry->d_name, ".") && strcmp(entry->d_name, ".."))
	    {
	      /* Check if current entry is shared
	       */
	      char path[1024] = "";
	      unsigned concat_off = 0;
	      unsigned parent_len = strlen(self->path);
	      (void) memcpy(path, self->path, parent_len);
	      concat_off += parent_len;
	      if (path[parent_len - 1] != '/')
		(void) memcpy(path + concat_off++, "/", 1);
	      (void) memcpy(path + concat_off, entry->d_name, entry->d_namlen);
	      concat_off += entry->d_namlen;
	      path[concat_off] = '\0';
	      elem_t* elem = server_get_elem_by_path(path);
	      if (elem) /* Entry is shared */
		{
		  /* Put just the entry name in path buffer
		   */
		  concat_off = 0;
		  (void) memcpy(path + concat_off, entry->d_name, entry->d_namlen);
		  concat_off += entry->d_namlen;
		  path[concat_off] = '\0';
		  unsigned path_len = strlen(path) + 1;
		  if (size - buf_off < 2 * sizeof(int) + path_len)
		    {
		      /* TODO: Max entry size == size
		       */
		      die("generate_dir_serialization: too small input buffer"); 
		    }
		  /* Elem has been registered
		   * Add entry to input buffer
		   */
		  (void) memcpy(buf + buf_off, (char*)&elem->id, sizeof(int));
		  buf_off += sizeof(int);
		  (void) memcpy(buf + buf_off, (char*)&path_len, sizeof(int));
		  buf_off += sizeof(int);
		  (void) memcpy(buf + buf_off, path, path_len);
		  buf_off += path_len;
		}
	    }
	  cur += entry->d_reclen; /* Next entry */
	}
      break; /* TODO: BUG readdir */
    }
  int eof = -1;
  (void) memcpy(buf + buf_off, (char*)&eof, sizeof(int));
  buf_off += sizeof(int);
  return buf_off;
}

static int	dir_read(file_t* self, unsigned offset, char* buf, unsigned size)
{
  char	dir_contents[2048];
  unsigned maxsize = generate_dir_serialization(self, dir_contents, sizeof(dir_contents));
  if (offset > maxsize)
    return -1;
  if (size > maxsize - offset)
    size = maxsize - offset;
  (void) memcpy(buf, dir_contents, size);
  return size;
}

static int	dir_write(file_t* self, unsigned offset, char* buf, unsigned size, void* pv)
{
  char* cur = buf;
  while (1)
    {
      unsigned id = *((unsigned*)cur);
      cur += sizeof(unsigned);
      if (id == -1)
	break;
      unsigned size = *((unsigned*)cur);
      cur += sizeof(unsigned);
      char path[1024] = "";
      unsigned concat_off = 0;
      unsigned parent_len = strlen(self->path);
      (void) memcpy(path, self->path, parent_len);
      concat_off += parent_len;
      if (path[parent_len - 1] != '/')
	(void) memcpy(path + concat_off++, "/", 1);
      (void) memcpy(path + concat_off, cur, size);
      cur += size;
      if (!server_get_dist_elem_by_id(id))
	{
	  /* New elem in directory !
	   */
	  server_t* serv = get_server();
	  host_t* host = (host_t*) pv;
	  if (host->addr)
	    serv->synchronize(serv, host->addr, host->port, id, path);
	}
    }
  return size;
}

static unsigned	dir_size(file_t* self)
{
  char	dir_contents[2048];
  unsigned maxsize = generate_dir_serialization(self, dir_contents, sizeof(dir_contents));
  return maxsize;
}

static int	dir_truncate(file_t* self, unsigned new_size)
{
  /* Nothing to do
   */
  return 0;
}
