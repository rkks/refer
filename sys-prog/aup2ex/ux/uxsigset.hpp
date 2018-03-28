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
#ifndef _UXSIGSET_HPP_
#define _UXSIGSET_HPP_

namespace Ux {

/**
	\ingroup Ux
*/
class Sigset : public sigset_t, public Base {
protected:

public:
	/**
		Initializes set to empty.
	*/
	Sigset(void)
		{
			empty();
		}
	/**
		Calls ::sigaddset.
	*/
	void	add(int signum)
				{
					if (::sigaddset(this, signum))
						throw Error(errno);
				}
	/**
		Calls ::sigdelset.
	*/
	void	del(int signum)
				{
					if (::sigdelset(this, signum))
						throw Error(errno);
				}
	/**
		Calls ::sigemptyset.
	*/
	void	empty(void)
				{
					if (::sigemptyset(this))
						throw Error(errno);
				}
	/**
		Calls ::sigfillset.
	*/
	void	fill(void)
				{
					if (::sigfillset(this))
						throw Error(errno);
				}
	/**
		Calls ::sigismember.
	*/
	bool	ismember(int signum) const
				{
					int r;

					if ((r = ::sigismember(this, signum)) == -1)
						throw Error(errno);
					return r == 1;
				}
	/**
		Calls ::sigpending. (Probably should be in Process instead of here.)
	*/
	void	pending(void)
				{
					if (::sigpending(this) == -1)
						throw Error(errno);
				}
	};
} // namespace

#endif // _UXSIGSET_HPP_
