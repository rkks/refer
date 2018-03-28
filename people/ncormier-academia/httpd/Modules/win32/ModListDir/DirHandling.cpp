# include <string>
# include <ctime>
# include <sys/types.h>
# include <sys/stat.h>

# include <fstream>
# include <windows.h>
# include <iostream>
# include <map>

# include "ZiaString.h"
# include "FileInfo.h"
# include "DirHandling.h"

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
    }

    DirHandling::~DirHandling()
    {
    }
    
    void  DirHandling::findFirst()
    {
      string path;

      path = this->_path + "\\*";
      this->_hFind = FindFirstFileA(path.c_str(), &(this->_FindFileData));
 		  if (this->_hFind == INVALID_HANDLE_VALUE)
        this->_exists = false;
      else
        this->_exists = true;
      this->_opened = true;
    }

    void  DirHandling::formatResult()
    {
      struct stat   sb;
      static int    mask_tab[2] = { S_IFDIR, S_IFREG };
      int           idx = 0;
      string        path(this->_FindFileData.cFileName);

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
      this->_file = (char*)this->_FindFileData.cFileName;
      this->_adate = sb.st_atime;
      this->_cdate = sb.st_ctime;
      this->_mdate = sb.st_mtime;
    }

    bool  DirHandling::NextFile()
    {
      if (this->_opened == false)
        findFirst();
      else if (!FindNextFileA(this->_hFind, &(this->_FindFileData)))
      {
        FindClose(this->_hFind);
        this->_hFind = NULL;
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

      cur = new FileInfo;
      cur->File = ZiaString::dup((char*) this->_FindFileData.cFileName);
      cur->Size = ((long long) this->_FindFileData.nFileSizeHigh << 32) + this->_FindFileData.nFileSizeLow;
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