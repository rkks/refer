#ifndef _PCSYNC_SIG_H
#define _PCSYNC_SIG_H

struct pcsync {
	sigset_t old_set;
};

bool pcsync_init(struct pcsync *p);
bool pcsync_wait_for_parent(struct pcsync *p);
bool pcsync_unblock_children(struct pcsync *p, int children,
  pid_t *pid_child);
bool pcsync_end(struct pcsync *p);

#endif /* _PCSYNC_SIG_H */
