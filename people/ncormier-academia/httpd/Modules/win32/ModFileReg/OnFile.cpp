#include "OnFile.h"

using namespace mozia::filereq;

namespace mozia
{
  namespace filereq
  {
    COnFile::COnFile(Reader *reader)
    {
      this->_getRange = 0;
      this->_curRange = -1;
      this->_inRange = 0;
      this->_nbRange = 0;
      this->_reader = reader;
      this->_inRange = 0xFFFFFFFFFFFFFFFF;
    }

    COnFile::~COnFile(void)
    {
      delete this->_reader;
    }

    void  COnFile::SetRanges(unsigned __int64* theRange, unsigned int nb)
    {
      this->_getRange = theRange;
      this->_nbRange = nb;
      this->_inRange = 0xFFFFFFFFFFFFFFFF;
    }

    void  COnFile::SetReader(Reader *reader)
    {
      this->_reader = reader;
    }

    Reader&  COnFile::GetReader(void)
    {
      return (*this->_reader);
    }

    Buffer *COnFile::GetData()
    {
      if (this->_nbRange)
      {
        Buffer  *buff;
        if ((int)this->_curRange >= (int)this->_nbRange)
          return (NULL);
        if (this->_inRange > this->_getRange[this->_curRange * 2 + 1])
        {
          this->_curRange++;
          this->_inRange = this->_getRange[this->_curRange * 2];
          if (this->_curRange >= (int) this->_nbRange)
            return (NULL);
          this->_reader->seekData(this->_inRange);
        }
        buff = this->_reader->getData();
        if (buff)
        {
          this->_inRange += buff->getSize();
          if (this->_inRange > this->_getRange[this->_curRange * 2 + 1])
            buff->rmData((int) (this->_inRange - this->_getRange[this->_curRange * 2 + 1]) - 1);
        }
        return (buff);
      }
      return (this->_reader->getData());
    }

	const string&  COnFile::GetType() const
	{
		static  string type = "COnFile";
		return (type);
	}

  }
}