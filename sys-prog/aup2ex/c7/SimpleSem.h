/*
	Simple Semaphore Interface
	AUP2, Sec. 7.09.2, 7.10.1

	Copyright 2003 by Marc J. Rochkind. All rights reserved.
	May be copied only for purposes and under conditions described
	on the Web page www.basepath.com/aup/copyright.htm.

	The Example Files are provided "as is," without any warranty;
	without even the implied warranty of merchantability or fitness
	for a particular purpose. The author and his publisher are not
	responsible for any damages, direct or incidental, resulting
	from the use or non-use of these Example Files.

	The Example Files may contain defects, and some contain deliberate
	coding mistakes that were included for educational reasons.
	You are responsible for determining if and how the Example Files
	are to be used.

*/
#ifndef _SIMPLESEM_H_
#define _SIMPLESEM_H_

struct SimpleSem {
	union {
		int sm_semid;
		void *sm_sem;
	} sm;
};

struct SimpleSem *SimpleSemOpen(const char *name);
bool SimpleSemClose(struct SimpleSem *sem);
bool SimpleSemPost(struct SimpleSem *sem);
bool SimpleSemWait(struct SimpleSem *sem);
bool SimpleSemRemove(const char *name);

#endif /* _SIMPLESEM_H_ */
