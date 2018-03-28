/******************************************************************************
 ******************************************************************************
 * Copyright (c) 2007 MatchPoint, All rights reserved
 *
 * File    :   
 * Desc    :    
 * Author  :    mat
 *****************************************************************************/

#include "AnswerSerializer.h"
#include "XMLWriter.h"
#include "Attributes.h"
#include "MString.h"
#include "Instances.h"
#include "MatchAnswer.h"
#include "Interfaces.h"

DNSPACE

AnswerSerializer::AnswerSerializer(MString& output)
{
  m_xml = new XMLWriter(output);
}

AnswerSerializer::~AnswerSerializer(void)
{
  delete m_xml;
}


// <matchpointresult>
//  <matchedentitylist>
//    <matchedentity>
//       <attribute>
//          <id>age</id>
//          <value>27</value>
//        </attribute>
//    </matchedentity>
//  </matchedentitylist>
// </matchpointresult>

bool AnswerSerializer::Serialize(const MatchAnswer& matchAnswer, const ObjectInterface &answerOn)
{
  m_xml->OpenNode("matchpointresult");
  m_xml->OpenNode("matchedentitylist");

  for (unsigned i = 0, n = matchAnswer.ObjectsCount(); i < n; i++)
  {
    const MatchedObjectInstance& matchedObj = matchAnswer.ObjectAtIndex(i);
    m_xml->OpenNode("matchedentity");
    for (unsigned j = 0, m = matchedObj.AttributeCount(); j < m; j++)
    {
      const MatchedObjectInstanceAttribute& matchedAttr = matchedObj.AttributeAtIndex(j);
      const SQLAttribute &attr = matchedAttr.Attribute();
      const IAttributeValue &value = matchedAttr.Value();

      // this attribute is in the answer_on ?
      bool found = false;
      for (unsigned k = 0, l = answerOn.AttributeCount(); k < l; k++)
	{
	  const ObjectAttribute &ansattr = answerOn.AttributeAtIndex(k);
	  if ((ansattr.Attribute().ParentInterface().TableName() == matchedAttr.Attribute().ParentInterface().TableName()) &&
	      (ansattr.Attribute().AttributeID() == attr.AttributeID()))
	  {
	    found = true;
	    break;
	  }
	}

      if (found == false)
	continue;

      m_xml->OpenNode("attribute");
      m_xml->OpenNodeWithCDATA("id");
      m_xml->AddCData(attr.AttributeID());
      m_xml->CloseNodeWithCDATA("id");
      MString type;
      m_xml->OpenNodeWithCDATA("value");
      switch (value.TypeOf())
      {
      case (AttributeValueType::INTEGER):
        type = "integer";
        {
          const AttributeValue<int> &val = dynamic_cast<const AttributeValue<int> &>(value);
          char buf[256];
          ::sprintf(buf, "%i", val.Value());
          m_xml->AddCData(buf);
        }
        break;

      case (AttributeValueType::FLOAT):
        type = "float";
        {
          const AttributeValue<float> &val = dynamic_cast<const AttributeValue<float> &>(value);
          char buf[256];
          ::sprintf(buf, "%f", val.Value());
          m_xml->AddCData(buf);
        }
        break;

      case (AttributeValueType::STRING):
        type = "string";
        {
          const AttributeValue<MString> &val = dynamic_cast<const AttributeValue<MString> &>(value);
          m_xml->AddCData(val.Value());
        }
        break;

      case (AttributeValueType::LIST):
        type = "list";
        {
          const ListAttributeValue &val = dynamic_cast<const ListAttributeValue &>(value);
          // 		    for (unsigned i = 0, n = val.Count(); i < n; i++)
          // 		      {
          // 		      }
        }
        break;

      case (AttributeValueType::UNDEFINED):
      default:
        // XXX
        break;
      }
      m_xml->CloseNodeWithCDATA("value");

      m_xml->OpenNodeWithCDATA("type");
      m_xml->AddCData(type);
      m_xml->CloseNodeWithCDATA("type");
      m_xml->CloseNode("attribute");
    }
    m_xml->CloseNode("matchedentity");
  }
  m_xml->CloseNode("matchedentitylist");
  m_xml->CloseNode("matchpointresult");
  return (true);
}
