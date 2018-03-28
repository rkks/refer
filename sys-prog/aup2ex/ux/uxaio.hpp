/*
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
#ifndef _UXAIO_HPP_
#define _UXAIO_HPP_

#include <aio.h>

namespace Ux {

/**
	\ingroup Ux
*/
class Aio : public aiocb, public Base {
protected:

public:
	Aio(void)
		{
			memset(this, 0, sizeof(struct aiocb));
		}
	int cancel(int fd);
	int error(void); // does not throw
	void sync(int op = O_SYNC);
	void read(void);
	ssize_t return_status(void); // can't call it return
	static void suspend(const struct aiocb *const list[], int cbcnt,
	  const struct timespec *timeout = NULL);
	void write(void);
	static void listio(int mode, struct aiocb *const list[],
	  int cbcnt, struct sigevent *sig = NULL);
};

} // namespace

#endif // _UXAIO_HPP_
