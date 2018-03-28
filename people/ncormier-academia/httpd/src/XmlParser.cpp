/*
**
**	$HeadURL$
**	$Author$
**	$Revision$
**	$Date$
**
*/

#include <iostream>

#include "Exceptions.h"
#include "XmlParser.h"


using namespace std;
using namespace mozia::exceptions;

namespace mozia
{
  namespace xmlconf
  {
    XmlParser::XmlParser(void)
    {
      this->_queue = new list<const char *>;
    }

    XmlParser::~XmlParser(void)
    {
    }

    XmlContainer  *XmlParser::parse(const char *file)
    {
      TiXmlDocument *xmldoc = new TiXmlDocument(file);
      if (xmldoc->LoadFile() == false)
        throw new XmlParserException(xmldoc->ErrorDesc());
      XmlContainer *xmlconf = new XmlContainer;
      for (TiXmlNode *child = xmldoc->RootElement()->FirstChild(); child != 0; child = child->NextSibling())
        this->_walk(child, xmlconf);
      xmldoc->Clear();
      delete xmldoc;
      return (xmlconf);
    }

    XmlContainer  *XmlParser::parse(vector <string *> *av)
    {
      return (this->parse(av->at(0)->c_str()));
    }

    XmlContainer  *XmlParser::append(XmlContainer *xmlconf, char *file)
    {
      TiXmlDocument *xmldoc = new TiXmlDocument(file);
      if (xmldoc->LoadFile() == false)
      {
        cout << "Cannot open xml file " << file << endl;
        cout << "Error : " << xmldoc->ErrorDesc() << endl;
        exit(1);
      }
      for (TiXmlNode *child = xmldoc->RootElement()->FirstChild(); child != 0; child = child->NextSibling())
        this->_walk(child, xmlconf);
      xmldoc->Clear();
      delete xmldoc;
      return (xmlconf);
    }

    void  XmlParser::_walk(TiXmlNode *node, XmlContainer *xmlconf)
    {
      if (!node)
        return;

      TiXmlNode *child;

      if (node->Type() == TiXmlNode::ELEMENT)
      {
        bool flag = false;
        const char *value;
        TiXmlAttribute* pAttrib = ((TiXmlElement *)node)->FirstAttribute();
        for (child = node->FirstChild(); child != 0; child = child->NextSibling())
          if (child->Type() == TiXmlNode::TEXT)
          {
            flag = true;
            value = node->Value();
            if (pAttrib)
              value = pAttrib->Value();
            xmlconf->NewValue(value, child->ToText()->Value());
          }
        if (flag == false)
        {
          value = node->Value();
          if (pAttrib)
            value = pAttrib->Value();
          xmlconf = xmlconf->NewChild(value);
        }
      }

      for (child = node->FirstChild(); child != 0; child = child->NextSibling())
        if (child->Type() == TiXmlNode::ELEMENT)
          this->_walk(child, xmlconf);
    }
  }
}
