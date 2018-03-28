#include <sys/inotify.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <limits.h>
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

/* usage: watch_copy <watch-dir> <dest-dir>
 *
 * whenever a file in watch-dir is closed (if it was open for writing),
 * it is copied to the dest-dir. It does not recurse.
 *
 * This implementation mmaps the source and dest files.
 * 
 */

int map_copy(char *file, char *dest) {
  struct stat s;
  char *src=NULL,*dst=NULL;
  int fd=-1,dd=-1,rc=-1;

  /* source file */
  if ( (fd = open(file, O_RDONLY)) == -1) {
    fprintf(stderr,"can't open %s: %s\n", file, strerror(errno));
    goto done;
  }
  if (fstat(fd, &s) == -1) {
    fprintf(stderr,"can't stat %s: %s\n", file, strerror(errno));
    goto done;
  }
  if (!S_ISREG(s.st_mode)) {
    fprintf(stderr,"not a regular file: %s\n", file);
    goto done;
  }
  src = mmap(0, s.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
  if (src == MAP_FAILED) {
    fprintf(stderr, "mmap %s: %s\n", file, strerror(errno));
    goto done;
  }

  /* dest file */
  if ( (dd = open(dest, O_RDWR|O_TRUNC|O_CREAT, 0644)) == -1) {
    fprintf(stderr,"can't open %s: %s\n", dest, strerror(errno));
    goto done;
  }
  if (ftruncate(dd, s.st_size) == -1) {
    fprintf(stderr,"ftruncate: %s\n", strerror(errno));
    goto done;
  }
  dst = mmap(0, s.st_size, PROT_READ|PROT_WRITE, MAP_SHARED, dd, 0);
  if (dst == MAP_FAILED) {
    fprintf(stderr, "mmap %s: %s\n", dest, strerror(errno));
    goto done;
  }
  memcpy(dst,src,s.st_size);

  rc = 0;

done:
  if (src && (src != MAP_FAILED)) if (munmap(src, s.st_size)) fprintf(stderr,"munmap: %s\n",strerror(errno));
  if (dst && (dst != MAP_FAILED)) if (munmap(dst, s.st_size)) fprintf(stderr,"munmap: %s\n",strerror(errno));
  if (fd != -1) close(fd);
  if (dd != -1) close(dd);
  return rc;
}

int main(int argc, char *argv[]) {
  int fd, wd, mask, rc;
  char *dir, *dest, *name, oldname[PATH_MAX],newname[PATH_MAX];

  if (argc != 3) {
    fprintf(stderr,"usage: %s <watch-dir> <dest-dir>\n", argv[0]);
    exit(-1);
  }

  dir = argv[1];  int olen = strlen(dir);
  dest = argv[2]; int dlen = strlen(dest);
  memcpy(newname, dest, dlen); newname[dlen]='/';
  memcpy(oldname, dir, olen); oldname[olen]='/';

  if ( (fd = inotify_init()) == -1) {
    perror("inotify_init failed");
    exit(-1); 
  }

  mask = IN_CLOSE_WRITE;
  if ( (wd = inotify_add_watch(fd, dir, mask)) == -1) {
    perror("inotify_add_watch failed");
    exit(-1); 
  }

  /* see inotify(7) as inotify_event has a trailing name
   * field allocated beyond the fixed structure; we must
   * allocate enough room for the kernel to populate it */
  struct inotify_event *eb, *ev, *nx;
  size_t eb_sz = sizeof(*eb) + PATH_MAX, sz;
  if ( (eb = malloc(eb_sz)) == NULL) {
    fprintf(stderr, "out of memory\n");
    exit(-1);
  }

  /* one read will produce one or more event structures */
  while ( (rc=read(fd,eb,eb_sz)) > 0) {
    for(ev = eb; rc > 0; ev = nx) {

      sz = sizeof(*ev) + ev->len;
      nx = (struct inotify_event*)((char*)ev + sz);
      rc -= sz;

      name = (ev->len ? ev->name : dir);
      memcpy(&newname[dlen+1],name,strlen(name)+1);
      memcpy(&oldname[olen+1],name,strlen(name)+1);
      fprintf(stderr, "%s --> %s\n", oldname, newname); 

      if (map_copy(oldname, newname)) goto done;
    }
  }

 done:
  close(fd);
}
