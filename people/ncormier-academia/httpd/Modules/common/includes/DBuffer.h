#ifndef _BUFFER_H_
# define _BUFFER_H_

# include <string>
# include "ICData.h"

using namespace std;
using namespace mozia::ticket;

namespace mozia
{
  namespace modules
  {
    namespace utils
    {
      class Buffer : public ICData
      {
      public:
        Buffer(void);
        Buffer(char *data, int size);
        Buffer(const string &d);
        Buffer(const Buffer &b);
        ~Buffer(void);
        char  *getData(void) const;
        int   getDataToInt(void) const;
        const string&  GetType() const;

        void  setData(char *data, int size);
        void  setData(const string &d);
        void  setData(const Buffer &b);
        void  addData(char *data, int size);
        void  addData(const string &d);
        void  addData(const Buffer &b);
        void  preprendData(char *data, int size);
        void  preprendData(const string &d);
        void  preprendData(const Buffer &b);
        void  rmData(int end);
        void  Trunc(int len);
        int getSize(void) const;
        int getRealSize(void) const;
        char *getRawBuf(void) const;
        void setSize(int size);
        void  Clear(void);
        string  toString(void);

        Buffer  &operator =(const Buffer &s);
      private:
        char  *_buf;
        int _size;
        int _rsize;
      };
    }
  }
}

#endif /* !_BUFFER_H_ */
