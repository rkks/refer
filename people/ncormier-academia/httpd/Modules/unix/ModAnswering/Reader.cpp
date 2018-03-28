// Mozia - $Id: Reader.cpp 544 2006-04-11 15:23:05Z nico $


#include <iostream>

#include <string>
#include <stdio.h>
#include <fcntl.h>

#include "Reader.h"
#include "utils/cmpfunc.h"


using namespace std;
using namespace mozia::utils;

namespace mozia
{
  namespace answering
  {
    Reader::Reader(string path)
    {
      this->_handle = 0;
      this->_error = READER_ERROR_NOERROR;
      this->_chunkSize = READER_CHUNK_SIZE;
      this->_ibuf = NULL;
      this->_mDate[0] = 0;
      this->_fileSize = -1;

      for (size_t i = path.length() - 1; i >= 0; i--)
	{
	  if (path[i] == '\\' || path[i] == '/')
	    path.resize(i);
	  else
	    break ;
	}
      this->_fileName = path;
      this->_handle = open(path.c_str(), O_RDONLY);
      if (this->_handle <= 0)
	{
	  this->_error = READER_ERROR_INTERNAL;
	  return ;
	}
      this->_ibuf = new char[this->_chunkSize];
      memset(this->_ibuf, 0, this->_chunkSize);
    }

    Reader::~Reader(void)
    {
      if (this->_ibuf)
        delete [] this->_ibuf;
      this->Close();
    }

    Reader::Reader()
    {
      this->_handle = 0;
      this->_ibuf = NULL;
      this->_mDate[0] = 0;
      this->_fileSize = -1;
    }

    char*	Reader::getMDate()
    {
      struct stat  s;

      if (this->_mDate[0] && stat(this->_fileName.c_str(), &s) != -1)
        strftime(this->_mDate, 31, "%a, %d %b %Y %H:%M:%S GMT", gmtime(&s.st_mtime));
      return (this->_mDate);
    }

    int Reader::getChunkSize(void)
    {
      return (this->_chunkSize);
    }

    long long Reader::getFileSize(void)
    {
      struct stat  s;

      if (this->_fileSize != -1 && stat(this->_fileName.c_str(), &s) != -1)
        this->_fileSize = s.st_size;
      return (this->_fileSize);
    }

    void  Reader::setChunkSize(int size)
    {
      this->_chunkSize = size;
      delete [] this->_ibuf;
      this->_ibuf = new char[size];
      memset(this->_ibuf, 0, size);
    }

    int Reader::getError(void)
    {
      return (this->_error);
    }

    int Reader::getFileStat(string path, long long &size, char MDate[32])
    {
      struct stat  s;

      for (size_t i = path.length() - 1; i >= 0; i--)\
	{
	  if (path[i] == '\\' || path[i] == '/')
	    path.resize(i);
	  else
	    break;
	}
      if (stat(path.c_str(), &s) == -1)
        return (READER_ERROR_NOFILE);
      strftime(MDate, 31, "%a, %d %b %Y %H:%M:%S GMT", gmtime(&s.st_mtime));
      size = s.st_size;
      if (!(S_IRUSR & s.st_mode))
        return (READER_ERROR_ACCESS);
      if ((s.st_mode & S_IFMT) == S_IFDIR)
        return (READER_ERROR_ISDIR);
      int handle = open(path.c_str(), O_RDONLY);
      if (handle <= 0)
	return (READER_ERROR_ACCESS);
      close(handle);
      return (READER_ERROR_NOERROR);
    }

    bool    Reader::seekData(unsigned long long begin)
    {
      if (lseek(this->_handle, begin, SEEK_SET) == -1)
	return (false);
      return (true);
    }

    Buffer  *Reader::getData(void)
    {
      int r;
      char  *tmp;

      if (this->_buf.getRealSize() >= this->_chunkSize)
        tmp = this->_buf.getRawBuf();
      else
        tmp = this->_ibuf;
      if (this->_error != READER_ERROR_NOERROR)
        return (NULL);
      if ((r = read(this->_handle, tmp, this->_chunkSize)) <= 0)
	{
	  this->_error = READER_ERROR_EOF;
	  return (NULL);
	}
      if (tmp == this->_ibuf)
        this->_buf.setData(this->_ibuf, r);
      else
        this->_buf.setSize(r);
      return (&(this->_buf));
    }

    void  Reader::Close(void)
    {
      if (this->_handle != -1)
	close(this->_handle);
    }

    const string&  Reader::GetType() const
    {
      static  string type = "Reader";
      return (type);
    }
  }
}
