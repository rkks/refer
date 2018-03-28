/*
**
**	$HeadURL: svn://epizia.homeip.net/repo/zia/src_common/includes/XmlContainer.h $
**	$Author: nico $
**	$Revision: 512 $
**	$Date: 2006-03-20 18:52:32 +0100 (Mon, 20 Mar 2006) $
**
*/

#ifndef __XML_CONTAINER_H__
# define __XML_CONTAINER_H__

# include <map>
# include <string>
# include <vector>

# include "utils/cmpfunc.h"

using namespace std;

# define vector_value vector<string *>
# define map_next     map<const char *, XmlContainer *, mozia::utils::eqstr>
# define map_values   map<const char *, vector_value *, mozia::utils::eqstr>

namespace mozia
{
  namespace xmlconf
  {
    class XmlContainer
    {
      friend class XmlParser;

    public:
      XmlContainer();
      ~XmlContainer();

      const string          *Get(const char *) const;
      const string          *Get(const char *, unsigned int) const;
      const vector_value    *GetVector(const char *) const;
      map_next::iterator    ChildBegin(void) const;
      map_next::iterator    ChildEnd(void) const;
      map_values::iterator  ValueBegin(void) const;
      map_values::iterator  ValueEnd(void) const;
      bool                  Exists(const char *key) const;
      XmlContainer          &operator[](const char *key) const;
      XmlContainer          *operator [](int index) const;
      void                  Clear(void);

    private:
      map_next    *_next;
      map_values  *_values;

      XmlContainer  *NewChild(const char *key);
      void          NewValue(const char *key, const char *string);
    };
  }
}

#endif // !__XML_CONTAINER_H__
