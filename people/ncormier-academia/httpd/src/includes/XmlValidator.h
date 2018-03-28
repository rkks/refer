/*
**
**	$HeadURL$
**	$Author$
**	$Revision$
**	$Date$
**
*/

#ifndef __XMLVALIDATOR_H__
# define __XMLVALIDATOR_H__

# include "XmlContainer.h"

namespace mozia
{
  namespace xmlconf
  {
    class XmlValidator
    {
    public:
      XmlValidator();
      bool  Check(XmlContainer *);
    };
  }
}
#endif // !__XMLVALIDATOR_H__
