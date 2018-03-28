#include "FileInfo.h"
#include "DirCache.h"

namespace mozia
{
  namespace listdir
  {
    DirCache::DirCache(const string& host, const string& url)
    {
      this->_host = host;
      this->_url = url;
      this->_maxSize = COLUMN_SIZE;
      this->_maxFile = COLUMN_SIZE;
    }

    DirCache::~DirCache()
    {
    }

    int DirCache::GetMDate()
    {
      return (this->_cachedMDate);
    }

    void  DirCache::Add(FileInfo* elem)
    {
      if (strcmp(elem->File, ".") == 0)
        this->_cachedMDate = (int) elem->Mdate;
      if (this->_maxSize < (int) elem->GetSize().length())
        this->_maxSize = elem->GetSize().length() + 1;
      if (this->_maxFile < (int) strlen(elem->File))
        this->_maxFile = strlen(elem->File);
      this->Files.push_front(elem);
    }

    void  DirCache::Sort(const char* arg)
    {
      if (strcmp(arg, "name") == 0)
        this->SortByName();
      else if (strcmp(arg, "date") == 0)
        this->SortByDate();
      else if (strcmp(arg, "size") == 0)
        this->SortBySize();
    }

    void DirCache::SortByName()
    {
      FileInfo*   tmp;
      static bool invert = true;
      
      for (list<FileInfo*>::iterator ite = this->Files.begin(); ite != this->Files.end(); ite++)
        for (list<FileInfo*>::iterator ptr = ite; ptr != this->Files.end(); ptr++)
          if ((strcmp((*ptr)->File, (*ite)->File) < 0) == invert)
          {
            tmp = *ptr;
            *ptr = *ite;
            *ite = tmp;
          }
      invert = !invert;
    }

    void DirCache::SortByDate()
    {
      FileInfo*   tmp;
      static bool invert = true;
      
      for (list<FileInfo*>::iterator ite = this->Files.begin(); ite != this->Files.end(); ite++)
        for (list<FileInfo*>::iterator ptr = ite; ptr != this->Files.end(); ptr++)
          if (((*ptr)->Mdate < (*ite)->Mdate) == invert)
          {
            tmp = *ptr;
            *ptr = *ite;
            *ite = tmp;
          }
      invert = !invert;
    }

    void DirCache::SortBySize()
    {
      FileInfo*   tmp;
      static bool invert = true;
      
      for (list<FileInfo*>::iterator ite = this->Files.begin(); ite != this->Files.end(); ite++)
        for (list<FileInfo*>::iterator ptr = ite; ptr != this->Files.end(); ptr++)
          if (((*ptr)->Size < (*ite)->Size) == invert)
          {
            tmp = *ptr;
            *ptr = *ite;
            *ite = tmp;
          }
      invert = !invert;
    }

    string  DirCache::Host()
    {
      return (this->_host);
    }

    string  DirCache::Url()
    {
      return (this->_url);
    }

    int   DirCache::MaxSizeLength()
    {
      return (this->_maxSize);
    }

    int   DirCache::MaxFileLength()
    {
      return (this->_maxFile);
    }
  }
}