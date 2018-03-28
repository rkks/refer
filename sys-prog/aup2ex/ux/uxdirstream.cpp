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
#include "ux.hpp"

using namespace Ux;

/**
	Calls malloc to allocate space for dirent.
*/
void DirStream::alloc(const char *path)
{
	// use malloc instead of new so errno will be set
	if ((entry = (struct dirent *)::malloc(offsetof(struct dirent, d_name) +
	  get_max_name(path) + 1)) == NULL)
		throw Error(errno);
}

/**
	Calls free to free directory entry.
*/
void DirStream::free(void)
{
	::free(entry);
	entry = NULL;
}

/**
	Calls pathconf to get _PC_NAME_MAX.
*/
size_t DirStream::get_max_name(const char *path)
{
	return (size_t)File(path).pathconf(_PC_NAME_MAX, 512);
}

////////////////////////////////////////////////

/**
	Calls opendir.
*/
void DirStream::open(const char *path)
{
	if ((dirstream = ::opendir(path)) == NULL)
		throw Error(errno);
}

/**
	Calls open and allocates space for directory entry.
*/
void DirStream::open_alloc(const char *path)
{
	open(path);
	alloc(path);
}

/**
	Calls closedir.
*/
void DirStream::close(void)
{
	if (::closedir(dirstream) == -1)
		throw Error(errno);
}

/**
	Calls readdir_r.
*/
bool DirStream::read(void)
{
	struct dirent *result;
	int r;

	if ((r = ::readdir_r(dirstream, entry, &result)) != 0)
		throw(r);
	return result != NULL;
}

/**
	Calls rewinddir.
*/
void DirStream::rewind(void)
{
	::rewinddir(dirstream);
}

/**
	Calls seekdir.
*/
void DirStream::seek(long loc)
{
	::seekdir(dirstream, loc);
}

/**
	Calls telldir.
*/
long DirStream::tell(void)
{
	return ::telldir(dirstream);
}
