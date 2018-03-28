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
#ifndef _UXDIRSTREAM_HPP_
#define _UXDIRSTREAM_HPP_

#include <dirent.h>

namespace Ux {

/**
	\ingroup Ux
*/
class DirStream : public Base {
protected:
	DIR *dirstream;
	struct dirent *entry;

public:
	DirStream(void)
		: dirstream(NULL), entry(NULL)
		{ }
	struct dirent *get_entry(void)
		{ return entry; }
	const char *get_name(void)
		{ return entry == NULL ? NULL : entry->d_name; }
	void alloc(const char *path = "/");
	void free(void);
	size_t get_max_name(const char *path = "/");

	void open(const char *path);
	void open_alloc(const char *path);
	void close(void);
	bool read(void);
	void rewind(void);
	void seek(long loc);
	long tell(void);
};

} // namespace

#endif // _UXDIRSTREAM_HPP_
