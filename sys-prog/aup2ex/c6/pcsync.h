#ifndef _PCSYNC_H
#define _PCSYNC_H

struct pcsync {
	int fdpc[2];
	int fdcp[2];
};

bool pcsync_init(struct pcsync *p);
bool pcsync_wait_for_parent(struct pcsync *p);
bool pcsync_unblock_children(struct pcsync *p, int children,
  pid_t *pid_child);
bool pcsync_end(struct pcsync *p);

#endif /* _PCSYNC_H */
