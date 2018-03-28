/*
**
**	$HeadURL$
**	$Author$
**	$Revision$
**	$Date$
**
*/

#ifndef __XMLPARSER_H__
# define __XMLPARSER_H__

# include <list>
# include <string>
# include <vector>

# include <tinyxml.h>
# include "XmlContainer.h"

using namespace std;

namespace mozia
{
  namespace xmlconf
  {
    class XmlParser
    {
    public:
      XmlParser();
      ~XmlParser(void);

      XmlContainer  *parse(const char *file);
      XmlContainer *parse(vector <string *> *);
      XmlContainer *append(XmlContainer *, char *);
    private:
      list<const char *> *_queue;

      void  _walk(TiXmlNode *, XmlContainer *);
    };
  }
}
#endif //__XMLPARSER_H__
