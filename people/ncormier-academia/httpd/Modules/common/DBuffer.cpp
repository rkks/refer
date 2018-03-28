// Mozia - $Id: DBuffer.cpp 503 2006-03-19 22:38:31Z nico $

#include <iostream>

#ifdef _WIN32
# include <windows.h>
#endif

#include "DBuffer.h"

using namespace std;

namespace mozia
{
  namespace modules
  {
    namespace utils
    {
      Buffer::Buffer(char *data, int size)
      {
        this->_size = 0;
        this->_rsize = 4096;
        this->_buf = new char[4096];
        this->setData(data, size);
      }

      Buffer::Buffer(const string &d)
      {
        this->_size = 0;
        this->_rsize = 4096;
        this->_buf = new char[4096];
        this->setData(d);
      }

      Buffer::Buffer(const Buffer &b)
      {
        this->_size = 0;
        this->_rsize = 4096;
        this->_buf = new char[4096];
        this->setData(b);
      }

      Buffer::Buffer(void)
      {
        this->_buf = NULL;
        this->_size = 0;
        this->_rsize = 4096;
        this->_buf = new char[4096];
      }

      Buffer::~Buffer(void)
      {
        this->Clear();
      }

      const string&  Buffer::GetType() const
      {
        static  string type = "Buffer";
        return (type);
      }

      char  *Buffer::getData(void) const
      {
        return (this->_buf);
      }

      int Buffer::getDataToInt(void) const
      {
        return (atoi(this->_buf));
      }

      void  Buffer::setData(char *data, int size)
      {
        if (size <= 0)
          return ;
        if (this->_rsize < size)
        {
          delete [] this->_buf;
          this->_buf = new char[size];
          this->_rsize = size;
        }
        memcpy(this->_buf, data, size);
        this->_size = size;
      }

      void  Buffer::setData(const string &d)
      {
        this->setData((char *) d.c_str(), (int) d.size());
      }

      void  Buffer::setData(const Buffer &b)
      {
        this->setData(b.getData(), b.getSize());
      }

      void  Buffer::addData(char *data, int size)
      {
        char  *_temp;

        if (size <= 0)
          return ;
        if (this->_size + size > this->_rsize)
        {
          _temp = new char[size + this->_size];
          memcpy(_temp, this->_buf, this->_size);
          memcpy((char *) _temp + this->_size, data, size);
          delete [] this->_buf;
          this->_buf = _temp;
          this->_rsize = this->_size + size;
        }
        else
          memcpy(this->_buf + this->_size, data, size);
        this->_size += size;
      }

      void  Buffer::addData(const string &d)
      {
        this->addData((char *) d.c_str(), (int) d.size());
      }

      void  Buffer::addData(const Buffer &b)
      {
        this->addData(b.getData(), b.getSize());
      }

      void  Buffer::preprendData(char *data, int size)
      {
        if (size <= 0)
          return ;
        char  *temp;

        if (this->_size + size > this->_rsize)
        {
          temp = new char[this->_size + size];
          memcpy(temp, data, size);
          memcpy((char *) temp + size, this->_buf, this->_size);
          delete [] this->_buf;
          this->_buf = temp;
          this->_rsize = this->_size + size;
        }
        else
        {
          memmove(this->_buf + size, this->_buf, this->_size);
          memcpy(this->_buf, data, size);
        }
        this->_size += size;
      }

      void  Buffer::preprendData(const string &d)
      {
        this->preprendData((char *) d.c_str(), (int) d.size());
      }

      void  Buffer::preprendData(const Buffer &b)
      {
        this->preprendData(b.getData(), b.getSize());
      }

      int Buffer::getSize(void) const
      {
        return (this->_size);
      }

      void Buffer::setSize(int size)
      {
        this->_size = size;
      }

      int Buffer::getRealSize(void) const
      {
        return (this->_rsize);
      }

      char *Buffer::getRawBuf(void) const
      {
        return (this->_buf);
      }

      void  Buffer::Clear(void)
      {
        if (this->_buf)
          delete [] this->_buf;
        this->_buf = NULL;
        this->_size = 0;
        this->_rsize = 0;
      }

      void  Buffer::Trunc(int len)
      {
        if (len <= 0)
          return ;
        if (len > this->_size)
          len = this->_size;
        this->_size -= len;
        memmove(this->_buf, this->_buf + len, this->_size);
      }

      Buffer  &Buffer::operator =(const Buffer &s)
      {
        if (this != &s)
        {
          if (this->getSize() != 0)
            this->Clear();
          this->setData(s);
        }
        return (*this);
      }

      string  Buffer::toString(void)
      {
        char  *str = new char[this->_size + 1];
        string  s;

        str[this->_size] = 0;
        memcpy(str, this->_buf, this->_size);
        s = str;
        delete [] str;
        return (s);
      }

      void  Buffer::rmData(int rm)
      {
        this->_size -= rm;
      }
    }
  }
}
