/******************************************************************************
 ******************************************************************************
 * Copyright (c) 2007 MatchPoint, All rights reserved
 *
 * File    :   
 * Desc    :    
 * Author  :    mat
 *****************************************************************************/

#include <sstream>
#include <iostream>
#include <vector>
#include <string>

#include "SQLGenerator.h"
#include "SQLExecutor.h"
#include "MatchTask.h"
#include "UserRequest.h"
#include "Attributes.h"
#include "Interfaces.h"
#include "Instances.h"
#include "Selectors.h"
#include "MString.h"
#include "MatchRules.h"
#include "DataAnswer.h"

DNSPACE

void  SQLGenerator::FillAnswer(MatchTask& tsk)
{
  SQLGenerator generator;
  if (!generator.GenerateAndExecute(tsk))
  {
    FASSERT(false, "Problem while generating and executing\n");
  }
}

SQLGenerator::SQLGenerator(void)
{
}

SQLGenerator::~SQLGenerator(void)
{
}

const char *SQLGenerator::EscapeString(const char *p_str)
{
  char  *p_res = new char[2 * ( strlen( p_str ) * sizeof( *p_str ) ) + 1];

  unsigned j = 0;
  for( unsigned i = 0, n = (unsigned int)strlen( p_str ); i < n; i++ )
    {
      char escape = 0;

      switch (p_str[i])
	{
	case '\n':
	  escape = 'n';
	  break;

	case '\r':
	  escape = 'r';
	  break;

	case '\\':
	  escape = '\\';
	  break;

	case '\'':
	  escape = '\'';
	  break;

	case '"':
	  escape = '\"';
	  break;

	case '\032':
	  escape = 'Z';
	  break;
	}

      if (escape != 0)
	{
	  p_res[j++] = '\\';
	  p_res[j++] = escape;
	}
      else
	p_res[j++] = p_str[i];
    }

  p_res[j] = 0;

  return p_res;
}

bool SQLGenerator::GenerateAndExecute(MatchTask& tsk)
{
  SQLExecutor sql;
  MString str;
  GenerateSelect(tsk.Request().Rules().RequestOn(), tsk.Request().Rules().AnswerOn(), tsk.Request().Rules(), str);

  unsigned int answerLimit = tsk.m_mutableUserRequest->AnswerLimit();
  if (answerLimit > 0)
    {
      str += " LIMIT ";

      std::ostringstream o;
      o << answerLimit;

      str += o.str().c_str();
    }

  bool ret = sql.Execute(str, tsk.Request().Rules().RequestOn(), tsk.Request().Rules().AnswerOn());
  char buf[1024];

#ifndef _WIN32
  sprintf(buf, "SQL Execution failed : %s", sql.GetLastError());
#else
  sprintf_s(buf, sizeof(buf), "SQL Execution failed : %s", sql.GetLastError());
#endif
  FASSERT(ret == true, buf);
  tsk.m_mutableDataAnswer = new MutableDataAnswer(); /* TEMPORARY */
  sql.FillResult(str, *tsk.m_mutableDataAnswer);
  return (true);
}

void  SQLGenerator::GenerateSelect(const ObjectInterface &obj, const ObjectInterface &answerOn, const MatchRules &matchRules, MString& sqlRequest)
{
  std::string                    request;
  std::vector<const ISelector *> selectors;
  std::vector<std::string>       tables;

  request = "SELECT ";

  for (unsigned i = 0, n = obj.AttributeCount(); i < n; i++)
  {
    const ObjectAttribute& curAttr = obj.AttributeAtIndex(i);
    FASSERT(!(IS_INVALID_OBJECT_ATTRIBUTE(curAttr)), "Invalid object attribute");

    request += std::string("`") +
      curAttr.Attribute().ParentInterface().TableName().CharArray() +
      std::string("`");

    request += ".";

    request += std::string("`") +
      curAttr.Attribute().ColumnName().CharArray() +
      std::string("`");

    request += ",";

    if (curAttr.Selector().GetType() != ISelector::NONE)
      {
	bool found = false;
	for (unsigned j = 0, m = (unsigned int)selectors.size(); j < m; j++ )
	  {
	    if (selectors[j] == &(curAttr.Selector()))
	      {
		found = true;
		break;
	      }
	  }
	if (found == false)
	  selectors.push_back(&(curAttr.Selector()));
      }

    bool found = false;
    for (unsigned j = 0; j < tables.size(); j++)
      if (tables[j] == curAttr.Attribute().ParentInterface().TableName().CharArray())
	{
	  found = true;
	  break;
	}
    if (found == false)
      tables.push_back(curAttr.Attribute().ParentInterface().TableName().CharArray());
  }

  // tester si le answer_on est pas vide
  if (answerOn.AttributeCount() == 0)
    {
      // remove the ','
      request.erase(request.length() - 1);
    }

  for (unsigned i = 0, n = answerOn.AttributeCount(); i < n; i++)
  {
    const ObjectAttribute& curAttr = answerOn.AttributeAtIndex(i);
    FASSERT(!(IS_INVALID_OBJECT_ATTRIBUTE(curAttr)), "Invalid object attribute");

    // Tester si le champ est pas deja dans le request_on
    for (unsigned j = 0, m = obj.AttributeCount(); j < m; j++)
      {
	const ObjectAttribute &reqattr = obj.AttributeAtIndex(j);
	if ((reqattr.Attribute().ParentInterface().TableName() == curAttr.Attribute().ParentInterface().TableName()) && (reqattr.Attribute().AttributeID() == curAttr.Attribute().AttributeID()))
	  {
	    goto skip;
	  }
      }
    {
      bool found = false;
      for (unsigned j = 0; j < tables.size(); j++)
	if (tables[j] == curAttr.Attribute().ParentInterface().TableName().CharArray())
	  {
	    found = true;
	    break;
	  }
      if (found == false)
	tables.push_back(curAttr.Attribute().ParentInterface().TableName().CharArray());
    }

    request += std::string("`") +
      curAttr.Attribute().ParentInterface().TableName().CharArray() +
      std::string("`");

    request += ".";

    request += std::string("`") +
      curAttr.Attribute().ColumnName().CharArray() +
      std::string("`");

    if (i != (n - 1))
      request += ",";
    else
      request += " ";
    ;
  skip:
    ;
  }

  // Maybe a ','
  if (request[request.length() - 1] == ',')
    request[request.length() - 1] = ' ';

  request += "FROM ";

  /* ALL NONE ? */
  bool a = true;
  for (unsigned j = 0, m = (unsigned int)selectors.size(); j < m; j++)
  {
    if (selectors[j]->GetType() != ISelector::NONE)
    {
      a = false;
      break;
    }
  }
  if (a == true)
  {
    /* ALL NONE ! */
    for (unsigned j = 0, m = (unsigned int)tables.size(); j < m; j++)
    {
      request += "`" + tables[j] + "`";
      if (j != (m - 1))
        request += ",";
    }
    goto where;
  }

  for (unsigned i = 0, n = (unsigned int)selectors.size(); i < n; i++)
  {
    switch (selectors[i]->GetType())
    {
    case (ISelector::BELONGS_TO):
      {
        const BelongsToSelector &btSel =
          static_cast<const BelongsToSelector&>(*selectors[i]);

        if (i == 0)
          request += std::string("`") +
          btSel.GetLocal().ParentInterface().TableName().CharArray() +
          std::string("`");

        request += std::string(" LEFT JOIN `" ) +
          btSel.GetTarget().ParentInterface().TableName().CharArray() +
          std::string("` ON (`") +
          btSel.GetLocal().ParentInterface().TableName().CharArray() +
          std::string("`.`") +
          btSel.GetLocal().ColumnName().CharArray() +
          std::string("` = `") +
          btSel.GetTarget().ParentInterface().TableName().CharArray() +
          std::string("`.`") +
          btSel.GetTarget().ColumnName().CharArray() + std::string("`)");

        if ((n - 1) != i)
          request += " ";
      }
      break;

    case (ISelector::HAS_MANY):
      {
        const HasManySelector &hmSel =
          static_cast<const HasManySelector&>(*selectors[i]);

        if (i == 0)
          request += std::string("`") +
          hmSel.GetLocal().ParentInterface().TableName().CharArray() +
          std::string("`");

        request += std::string(" LEFT JOIN `" ) +
          hmSel.GetTarget().ParentInterface().TableName().CharArray() +
          std::string("` ON (`") +
          hmSel.GetLocal().ParentInterface().TableName().CharArray() +
          std::string("`.`") +
          hmSel.GetLocal().ColumnName().CharArray() +
          std::string("` = `") +
          hmSel.GetTarget().ParentInterface().TableName().CharArray() +
          std::string("`.`") +
          hmSel.GetTarget().ColumnName().CharArray() + std::string("`)");

        if ((n - 1) != i)
          request += " ";
      }
      break;

    case (ISelector::HAS_AND_BELONGS_TO_MANY):
      {
        const HABTMSelector &habtmSel =
          static_cast<const HABTMSelector&>(*selectors[i]);

        if (i == 0)
          request += std::string("`") +
          habtmSel.GetLocal().ParentInterface().TableName().CharArray() +
          std::string("`");

        request += std::string(" JOIN `" ) +
          habtmSel.GetTarget().ParentInterface().TableName().CharArray() +
          std::string("` ON (`") +
          habtmSel.GetLocal().ParentInterface().TableName().CharArray() +
          std::string("`.`") +
          habtmSel.GetLocal().ColumnName().CharArray() +
          std::string("` = `") +
          habtmSel.GetLocalToInter().ParentInterface().TableName().CharArray() +
          std::string("`.`") +
          habtmSel.GetLocalToInter().ColumnName().CharArray() +
          std::string("` AND `") +
          habtmSel.GetInterToTarget().ParentInterface().TableName().CharArray() +
          std::string("`.`") +
          habtmSel.GetInterToTarget().ColumnName().CharArray() +
          std::string("` = `") +
          habtmSel.GetTarget().ParentInterface().TableName().CharArray() +
          std::string("`.`") +
          habtmSel.GetTarget().ColumnName().CharArray() +
          std::string("`)");

        if ((n - 1) != i)
          request += " ";
      }
      break;

    default:
      FASSERT(false, "Unhandled Selector ..");
    }
  }

  where:
  request += " WHERE ";

  for (unsigned w = 0, z = matchRules.ProfileCount(); w < z; w++ )
    {
      const ProfileInterface &profile = matchRules.ProfileAtIndex(w);
      request += "(";
      if (profile.AttributeCount() > 0 )
	{
	  for (unsigned i = 0, n = profile.AttributeCount(); i < n; i++)
	    {
	      const ProfileInterfaceAttribute &profileAttr = profile.AttributeAtIndex(i);
	      FASSERT(IS_INVALID_PROFILE_INTERFACE_ATTRIBUTE(profileAttr) == false, "Invalid profile interface attribute");
	      switch (profileAttr.Value().TypeOf())
		{
		case (ProfileAttributeValueType::SINGLE):
		  {
		    const SingleProfileAttributeValue &singleValue = static_cast<const SingleProfileAttributeValue &> (profileAttr.Value());

		    std::string value;

		    switch (singleValue.Value().TypeOf())
		      {
		      case AttributeValueType::INTEGER:
			{
			  const AttributeValue<int> &intValue = static_cast<const AttributeValue<int> &> (singleValue.Value());

			  std::ostringstream o;
			  o << intValue.Value();
			  value = o.str();
			}
			break;

		      case AttributeValueType::FLOAT:
			{
			  const AttributeValue<float> &floatValue = static_cast<const AttributeValue<float> &> (singleValue.Value());

			  std::ostringstream o;
			  o << floatValue.Value();
			  value = o.str();
			}
			break;

		      case AttributeValueType::STRING:
			{
			  const AttributeValue<MString> &stringValue = static_cast<const AttributeValue<MString> &> (singleValue.Value());
			  value = stringValue.Value().CharArray();
			  const char *p_tmp = EscapeString(value.c_str());
			  value = p_tmp;
			  delete [] p_tmp;
			}
			break;

		      default:
		      case AttributeValueType::LIST:
			FASSERT( false, "lists not supported for SingleProfileAttribute" );
			break;

		      }

		    request += std::string("(`") + profileAttr.Attribute().ParentInterface().TableName().CharArray() + std::string("`.`") + profileAttr.Attribute().ColumnName().CharArray() + std::string("` = '") + value + std::string("')");
		  }
		  break;

		case (ProfileAttributeValueType::RANGE):
		  {
		    const RangeProfileAttributeValue &rangeValue = static_cast<const RangeProfileAttributeValue &> (profileAttr.Value());

		    std::string lowValue;
		    std::string highValue;

		    /* on suppose que LowValue et HighValue auront le meme type ! */
		    switch (rangeValue.LowValue().TypeOf())
		      {
		      case AttributeValueType::INTEGER:
			{
			  const AttributeValue<int> &lowIntValue = static_cast<const AttributeValue<int> &> (rangeValue.LowValue());

			  std::ostringstream o;
			  o << lowIntValue.Value();
			  lowValue = o.str();

			  const AttributeValue<int> &highIntValue = static_cast<const AttributeValue<int> &> (rangeValue.HighValue());
			  o.str("");
			  o << highIntValue.Value();
			  highValue = o.str();
			}
			break;

		      case AttributeValueType::FLOAT:
			{
			  const AttributeValue<float> &lowFloatValue = static_cast<const AttributeValue<float> &> (rangeValue.LowValue());

			  std::ostringstream o;
			  o << lowFloatValue.Value();
			  lowValue = o.str();

			  const AttributeValue<float> &highFloatValue = static_cast<const AttributeValue<float> &> (rangeValue.HighValue());
			  o << highFloatValue.Value();
			  highValue = o.str();
			}
			break;

		      default:
		      case AttributeValueType::STRING:
		      case AttributeValueType::LIST:
			FASSERT( false, "lists and string not supported for RangeProfileAttribute" );
			break;
		      }

		    request += std::string("(`") + profileAttr.Attribute().ParentInterface().TableName().CharArray() + std::string("`.`") + profileAttr.Attribute().ColumnName().CharArray() + std::string("` >= '") + lowValue + std::string("' AND `") + profileAttr.Attribute().ParentInterface().TableName().CharArray() + std::string("`.`") + profileAttr.Attribute().ColumnName().CharArray() + std::string("` <= '") + highValue + std::string("')");
		  }
		  break;

		case (ProfileAttributeValueType::ENUM):
		  {
		    const EnumProfileAttributeValue &enumValue = static_cast<const EnumProfileAttributeValue &> (profileAttr.Value());

		    if (enumValue.Count() == 0 )
		      {
			FASSERT( false, "empty enum!" );
		      }

		    request += std::string("`") + profileAttr.Attribute().ParentInterface().TableName().CharArray() + std::string("`.`") + profileAttr.Attribute().ColumnName().CharArray() + std::string("` IN (");
		    std::string value;

		    /* on suppose que toutes les valeurs du range auront le meme type ! */
		    for (unsigned i = 0, n = enumValue.Count(); i < n; i++)
		      {
			const IAttributeValue &enumElem = enumValue.ValueAtIndex(i);

			switch (enumElem.TypeOf())
			  {
			  case AttributeValueType::INTEGER:
			    {
			      const AttributeValue<int> &intValue = static_cast<const AttributeValue<int> &> (enumElem);

			      std::ostringstream o;
			      o << intValue.Value();
			      value = o.str();
			    }
			    break;

			  case AttributeValueType::FLOAT:
			    {
			      const AttributeValue<float> &floatValue = static_cast<const AttributeValue<float> &> (enumElem);

			      std::ostringstream o;
			      o << floatValue.Value();
			      value = o.str();
			    }
			    break;

			  case AttributeValueType::STRING:
			    {
			      const AttributeValue<MString> &stringValue = static_cast<const AttributeValue<MString> &> (enumElem);

			      value = stringValue.Value().CharArray();
			      const char *p_tmp = EscapeString(value.c_str());
			      value = p_tmp;
			      delete [] p_tmp;
			    }
			    break;

			  default:
			  case AttributeValueType::LIST:
			    FASSERT( false, "lists not supported for EnumProfileAttribute" );
			    break;
			  }

			request += std::string("'") + value + std::string("'");
			if (i != (n - 1))
			  request += std::string(", ");
			else
			  request += std::string(")");
		      }
		  }
		  break;

		case (ProfileAttributeValueType::UNDEFINED):
		  FASSERT( false, "Undefined ProfileAttributeValueType" );
		  break;
		}

	      if ( i != ( n - 1 ) )
		request += " AND ";
	    }
	}
      else
	request += "(1 = 1)";

      request += ")";

      if ( w != ( z - 1 ) )
	request += " OR ";
    }
  sqlRequest = request.c_str();
}
