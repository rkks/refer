
# include <string>
# include <ctime>
# include <sys/types.h>
# include <sys/stat.h>

# include <fstream>
# include <iostream>
# include <map>

# include "ZiaString.h"
# include "FileInfo.h"
# include "DirHandling.h"


#include <errno.h>

using namespace mozia;
using namespace std;

namespace mozia
{
  namespace listdir
  {
    DirHandling::DirHandling(string root)
    {
      this->_root = root;
      this->_opened = false;
      this->_exists = false;
      this->End = false;
      this->_handle = NULL;
      this->_ent = NULL;
    }

    DirHandling::~DirHandling()
    {
    }

    void  DirHandling::findFirst()
    {
      string path;

      path = this->_path;
      _handle = opendir(path.c_str());
      if (_handle == NULL)
	this->_exists = false;
      else
	this->_exists = true;
      this->_opened = true;
    }

    void  DirHandling::formatResult(void)
    {
      struct stat   sb;
      static int    mask_tab[2] = { S_IFDIR, S_IFREG };
      int           idx = 0;
      string	path(this->_ent->d_name);

      path = this->GetPath() + "/" + path;
      if (stat(path.c_str(), &sb) == -1)
	cout << "LISTDIR: Erreur dans le stat !" << endl;

      for (idx = 0; idx < 2; idx++)
        if ((S_IFMT & sb.st_mode) == mask_tab[idx])
	  break;
      if (idx == 2)
        this->_type = 1;
      else
        this->_type = idx;
      this->_size = sb.st_size;
      this->_file = this->_ent->d_name;
      this->_adate = sb.st_atime;
      this->_cdate = sb.st_ctime;
      this->_mdate = sb.st_mtime;
    }

    bool  DirHandling::NextFile()
    {
      if (this->_opened == false)
        findFirst();
      if ((this->_ent = readdir(this->_handle)) == NULL)
	{
	  closedir(this->_handle);
	  this->_handle = NULL;
	  this->End = true;
	  return (false);
	}
      if (!this->_exists)
        return (false);
      this->formatResult();
      return (true);
    }

    FileInfo*     DirHandling::GetFileInfo()
    {
      long long int  toto = 0;
      FileInfo*   cur;
      struct stat sb;

      cur = new FileInfo();
      cur->File = ZiaString::dup(this->_ent->d_name);
      string path(this->GetPath() + "/" + cur->File);

      if (stat(path.c_str(), &sb) == -1)
	throw "Ca devrait pas merder ...";
      cur->Size = sb.st_size;
      cur->Type = this->_type;
      cur->Adate = this->_adate;
      cur->Cdate = this->_cdate;
      cur->Mdate = this->_mdate;
      return (cur);
    }

    const string&  DirHandling::GetType() const
    {
      static  string type = "DirHandling";
      return (type);
    }
  }
}
