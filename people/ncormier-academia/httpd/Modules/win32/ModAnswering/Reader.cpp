// Mozia - $Id: Reader.cpp 682 2006-04-19 19:17:14Z dak $

#include <iostream>

#include <string>
#include <stdio.h>

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
      this->_handle = NULL;
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
      this->_handle = CreateFile(path.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
      if (!this->_handle || this->_handle == INVALID_HANDLE_VALUE)
      {
        this->_error = READER_ERROR_INTERNAL;
        return ;
      }
      this->_ibuf = new char[this->_chunkSize];
      ZeroMemory(this->_ibuf, this->_chunkSize);
    }

    Reader::~Reader(void)
    {
      if (this->_ibuf)
        delete [] this->_ibuf;
      this->Close();
    }

    Reader::Reader()
    {
      this->_handle = NULL;
      this->_ibuf = NULL;
      this->_mDate[0] = 0;
      this->_fileSize = -1;
    }

    char*	Reader::getMDate()
    {
      struct _stat  s;

      if (this->_mDate[0] && _stat(this->_fileName.c_str(), &s) != -1)
#pragma warning (disable : 4996)
        strftime(this->_mDate, 31, "%a, %d %b %Y %H:%M:%S GMT", gmtime(&s.st_mtime));
      return (this->_mDate);
    }

    int Reader::getChunkSize(void)
    {
      return (this->_chunkSize);
    }

    __int64 Reader::getFileSize(void)
    {
      struct _stat  s;

      if (this->_fileSize != -1 && _stat(this->_fileName.c_str(), &s) != -1)
        this->_fileSize = s.st_size;
      return (this->_fileSize);
    }

    void  Reader::setChunkSize(int size)
    {
      this->_chunkSize = size;
      delete [] this->_ibuf;
      this->_ibuf = new char[size];
      ZeroMemory(this->_ibuf, size);
    }

    int Reader::getError(void)
    {
      return (this->_error);
    }

    int Reader::getFileStat(string path, __int64 &size, char MDate[32])
    {
      struct _stati64  s;

      for (size_t i = path.length() - 1; i >= 0; i--)\
      {
        if (path[i] == '\\' || path[i] == '/')
          path.resize(i);
        else
          break ;
      }
      if (_stati64(path.c_str(), &s) == -1)
        return (READER_ERROR_NOFILE);
#pragma warning (disable : 4996)
      strftime(MDate, 31, "%a, %d %b %Y %H:%M:%S GMT", gmtime(&s.st_mtime));
      size = s.st_size;
      if (!(S_IRUSR & s.st_mode))
        return (READER_ERROR_ACCESS);
      if ((s.st_mode & S_IFMT) == S_IFDIR)
        return (READER_ERROR_ISDIR);
      HANDLE handle =
        CreateFile(path.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
      if (handle == INVALID_HANDLE_VALUE || !handle)
        return (READER_ERROR_ACCESS);
      CloseHandle(handle);
      return (READER_ERROR_NOERROR);
    }

    BOOL    Reader::seekData(unsigned __int64 begin)
    {
      LARGE_INTEGER test;
      test.QuadPart = begin;
      return (SetFilePointerEx(this->_handle, test, NULL, FILE_BEGIN));
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
      if (ReadFile(this->_handle, tmp, this->_chunkSize, (LPDWORD) &r, NULL) == false)
      {
        this->_error = READER_ERROR_INTERNAL;
        return (NULL);
      }
      if (!r)
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
      CloseHandle(this->_handle);
    }

	const string&  Reader::GetType() const
	{
		static  string type = "Reader";
		return (type);
	}

  }
}
