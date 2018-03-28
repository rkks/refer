/******************************************************************************
******************************************************************************
* Copyright (c) 2007 MatchPoint, All rights reserved
*
* File    :    ConfigAdapter.cpp
* Desc    :    Config adapter from XML (implementation)
* Author  :    mat
*****************************************************************************/

/*****************************************************************************/
/* Header files                                                              */
/*****************************************************************************/
#include "UserRequestAdapter.h"
#include "MString.h"
#include "Assert.h"
#include "Exception.h"
#include <typeinfo>
/*****************************************************************************/
/* Default namespace                                                         */
/*****************************************************************************/
DNSPACE
/*****************************************************************************/
/* Class implementation                                                      */
/*****************************************************************************/

MutableUserRequest *UserRequestAdapter::Create(const char *path)
{
  UserRequestAdapter adapter;

	XMLParser *xp = XMLParser::Factory(path);

  MutableUserRequest *request = NULL;
  try
  {
    request = adapter.Create(xp);
    delete xp;
  }
  catch (const Exception& ex)
  {
    delete adapter.m_ureq;
    delete xp;
    throw;
  }

  return (request);
}

MutableUserRequest *UserRequestAdapter::Create(const char *buffer, int size)
{
  UserRequestAdapter adapter;

	XMLParser *xp = XMLParser::Factory(buffer, size);

  MutableUserRequest *request = NULL;
  try
  {
    request = adapter.Create(xp);
    delete xp;
  }
  catch (const Exception& ex)
  {
    delete adapter.m_ureq;
    delete xp;
    throw;
  }

  return (request);
}

MutableUserRequest *UserRequestAdapter::Create(XMLParser *xp)
{
	xp->Parse();

	m_ureq = new MutableUserRequest();
	XMLNode *curnode = xp->GetRootNode();

  CheckRoot(curnode);
  GetRequestBaseConf(curnode);
  GetRequestEntities(curnode->GetChild("entities"));
  GetRequestMatchRules(curnode->GetChild("matchrules"));

	return (m_ureq);
}

void UserRequestAdapter::CheckRoot(XMLNode *root)
{
	FASSERT(root != NULL, "root node does not exist");
}

void UserRequestAdapter::GetRequestBaseConf(XMLNode *root)
{
	FASSERT(root->ChildExists("answerlimit") == true, "answer limit is not specified");
	FASSERT(root->ChildExists("minmatchpercentage") == true, "answer limit is not specified");

	const char *ans = root->GetChild("answerlimit")->GetData();
	FASSERT(ans != NULL, "specified answer limit is wrong");

	int nbr = atoi(ans);
	FASSERT(nbr >= 0, "answer limit should be at least 0");
	m_ureq->SetAnswerLimit(nbr);

	const char *min = root->GetChild("minmatchpercentage")->GetData();
	FASSERT(min != NULL, "specified minimum matching percentage is wrong");

	nbr = atoi(min);
	FASSERT(nbr >= 0 && nbr <= 100, "minimum matching percentage should be between 0 and 100");
	m_ureq->SetMinMatchPercentage(nbr);
}

void UserRequestAdapter::GetRequestEntities(XMLNode *entities)
{
	FASSERT(entities != NULL, "entities node does not exist");
	int entitynbr = entities->GetChildrenCount("entity");
	FASSERT(entitynbr > 0, "you need to specify at least 1 entity");

	for (int i = 0; i < entitynbr; i++)
	{
		m_ureq->AddAllocatedObjectIntefaces(CreateObjectInterface(entities->GetChild("entity", i)));
	}
}

MutableObjectInterface *UserRequestAdapter::CreateObjectInterface(XMLNode *entity)
{
	FASSERT(entity != NULL, "wrong element where it should be 'entity'");
	FASSERT(entity->ChildExists("id") == true, "'id' is missing for element 'entity'");
	FASSERT(entity->ChildExists("defaultentityid") == true, "'defaultentityid' is missing for element 'entity'");

	int attrsnbr = entity->GetChildrenCount("attribute");
	FASSERT(attrsnbr > 0, "an entity must contains at least 1 attribute");

	const char *id = entity->GetChild("id")->GetData();
	FASSERT(id != NULL && strlen(id) != 0, "'id' of the entity is empty");

	const char *entityid = entity->GetChild("defaultentityid")->GetData();
	FASSERT(entityid != NULL && strlen(entityid) != 0, "'defaultentityid' of the entity is empty");

	const MapInterface &mapi = ConfigHandler::MainConfig().MapInterfaceById(entityid);
	MutableObjectInterface *obji = new MutableObjectInterface(id);

	for (int i = 0; i < attrsnbr; i++)
	{
		XMLNode *attr = entity->GetChild("attribute", i);
		FASSERT(attr != NULL, "wrong element where it should be 'attribute'");

		if (attr->ChildExists("entityid") == false)
			CreateObjectInterfaceAttribute(attr, obji, mapi, entityid);
		else
			CreateObjectInterfaceAttributeSub(attr, obji, mapi);
	}

	return (obji);
}

void UserRequestAdapter::CreateObjectInterfaceAttributeSub(XMLNode *attr, MutableObjectInterface *obji, const MapInterface &mapi)
{
	const char *entityid = attr->GetChild("entityid")->GetData();
	FASSERT(entityid != NULL && strlen(entityid) != 0, "'entityid' of the attribute is empty");

	CreateObjectInterfaceAttribute(attr, obji, mapi, entityid);
}

void UserRequestAdapter::CreateObjectInterfaceAttribute(XMLNode *attr, MutableObjectInterface *obji, const MapInterface &mapi, const char *entityid)
{
	FASSERT(IS_INVALID_OBJECT_INTERFACE(mapi) == false, CSTR(MSTR("the entityid '") + MSTR(entityid) + MSTR("' for attribute '") + MSTR(attr->GetName()) + MSTR("' does not exist")));

	FASSERT(attr != NULL, "wrong element where it should be 'attribute'");
	FASSERT(attr->ChildExists("id") == true, "'id' is missing for element 'attribute'");

	const char *id = attr->GetChild("id")->GetData();
	FASSERT(id != NULL && strlen(id) != 0, "'id' of the entity is empty");

	bool res = mapi.AddAttributeToObject(entityid, id, GetAttributeType(attr->GetChild("type")), *obji);
	FASSERT(res == true, CSTR(MSTR("error while creating a new attribute for entity '") + MSTR(entityid) + MSTR("' on attribute '") + MSTR(id) + MSTR("'")));
}

const AttributeValueType *UserRequestAdapter::GetAttributeType(XMLNode *type)
{
	if (type == NULL)
	{
		return (AttributeValueType::String());
	}
	const char *typestr = type->GetData();
	if (typestr == NULL)
	{
		return (AttributeValueType::String());
	}

	if (!strcasecmp(typestr, "int"))
	{
		return (AttributeValueType::Int());
	}
	else if (!strcasecmp(typestr, "float"))
	{
		return (AttributeValueType::Float());
	}
	return (AttributeValueType::String());
}

void UserRequestAdapter::GetRequestMatchRules(XMLNode *matchrules)
{
	FASSERT(matchrules != NULL, "no matchrules element found");

	int mrnbr = matchrules->GetChildrenCount("matchrule");
	FASSERT(mrnbr > 0, "you need to specify at least one matchrule");

	MutableMatchRules *mrs = CreateBaseMatchRules(matchrules);

	for (int i = 0; i < mrnbr; i++)
	{
		CreateRequestMatchRule(mrs, matchrules->GetChild("matchrule", i));
	}

	m_ureq->SetAllocatedMatchRules(mrs);
}

MutableMatchRules *UserRequestAdapter::CreateBaseMatchRules(XMLNode *matchrules)
{
	FASSERT(matchrules->ChildExists("name") == true, "you need to specify a 'name' for the matchrule list");
	FASSERT(matchrules->ChildExists("description") == true, "you need to specify a 'description' for the matchrule list");
	FASSERT(matchrules->ChildExists("request_on_entity") == true, "you need to specify the entity to request for the matchrule list");
	FASSERT(matchrules->ChildExists("answer_on_entity") == true, "you need to specify the entity to answer for the matchrule list");

	const char *name = matchrules->GetChild("name")->GetData();
	FASSERT(name != NULL && strlen(name) != 0, "'name' of the matchrule list is empty");

	const char *description = matchrules->GetChild("description")->GetData();
	FASSERT(description != NULL && strlen(description) != 0, "'description' of the matchrule list is empty");

	const char *request_e = matchrules->GetChild("request_on_entity")->GetData();
	FASSERT(request_e != NULL && strlen(request_e) != 0, "'request_on_entity' of the matchrule list is empty");

	const char *answer_e = matchrules->GetChild("answer_on_entity")->GetData();
	FASSERT(answer_e != NULL && strlen(answer_e) != 0, "'answer_on_entity' of the matchrule list is empty");

	MutableMatchRules *mrs = new MutableMatchRules();
	mrs->SetName(name);
	mrs->SetRequestOn(m_ureq->ObjectByID(request_e));
	mrs->SetAnswerOn(m_ureq->ObjectByID(answer_e));

	return (mrs);
}

void UserRequestAdapter::CreateRequestMatchRule(MutableMatchRules *mrs, XMLNode *matchrule)
{
	FASSERT(matchrule != NULL, "no element where it should be 'matchrule'");
	FASSERT(matchrule->ChildExists("profile") == true, "no element profile specified for the matchrule");

	XMLNode *profilenode = matchrule->GetChild("profile");
	FASSERT(profilenode->ChildExists("defaultentityid") == true, "no 'defaultentityid' for the profile");

	const char *entityid = profilenode->GetChild("defaultentityid")->GetData();
	FASSERT(entityid != NULL && strlen(entityid) != 0, "'defaultentityid' of the profile is empty");

	const MapInterface &mapi = ConfigHandler::MainConfig().MapInterfaceById(entityid);

	int attrnbr = profilenode->GetChildrenCount("attribute");
	// for ugo allowance for empty match rules
	//FASSERT(attrnbr > 0, "you need to specify at least one attribute for the profile");

	MutableProfileInterface *profile = new MutableProfileInterface();
	for (int i = 0; i < attrnbr; i++)
	{
		XMLNode *attr = profilenode->GetChild("attribute", i);
		FASSERT(attr != NULL, "wrong element where it should be 'attribute'");

		if (attr->ChildExists("entityid") == false)
			CreateProfileAttribute(profile, mapi, attr);
		else
			CreateProfileAttributeSub(profile, attr);
	}

	mrs->AddAllocatedProfile(profile);
}

void UserRequestAdapter::CreateProfileAttributeSub(MutableProfileInterface *profile, XMLNode *attr)
{
	const char *entityid = attr->GetChild("entityid")->GetData();
	FASSERT(entityid != NULL && strlen(entityid) != 0, "'entityid' of the attribute is empty");

	const MapInterface &mapi = ConfigHandler::MainConfig().MapInterfaceById(entityid);

	CreateProfileAttribute(profile, mapi, attr);
}

void UserRequestAdapter::CreateProfileAttribute(MutableProfileInterface *profile, const MapInterface &mapi, XMLNode *attr)
{
	FASSERT(IS_INVALID_OBJECT_INTERFACE(mapi) == false, "the entityid does not exist");

	FASSERT(attr != NULL, "no element where it should be 'attribute'");
	FASSERT(attr->ChildExists("id"), "no 'id' specified for the attribute");

	const char *id = attr->GetChild("id")->GetData();
	FASSERT(id != NULL && strlen(id) != 0, "'id' of the attribute is empty");

	const SQLAttribute &sqlattr = mapi.LocalAttributeByID(id);
	FASSERT(IS_INVALID_SQL_ATTRIBUTE(sqlattr) == false, "wrong attribute id");

	IProfileAttributeValue *iprofattrval = CreateProfileAttributeValue(sqlattr, attr);
	profile->AddAttribute(&sqlattr, *iprofattrval);
	delete iprofattrval;
}

IProfileAttributeValue *UserRequestAdapter::CreateProfileAttributeValue(const SQLAttribute &sqlattr, XMLNode *attr)
{
	if (attr->ChildExists("value"))
	{
		const char *nbr = attr->GetChild("value")->GetData();
		FASSERT(nbr != NULL && strlen(nbr) > 0, CSTR(MSTR("value for attribute ") + MSTR(attr->GetName()) + MSTR(" cannot be null")));
		return (CreateSingleProfileAttributeValue(sqlattr.Type(), nbr));
	}
	else if (attr->ChildExists("list"))
	{
		return (CreateEnumProfileAttributeValue(sqlattr.Type(), attr->GetChild("list")));
	}
	else if (attr->ChildExists("range"))
	{
		return (CreateRangeProfileAttributeValue(sqlattr.Type(), attr->GetChild("range")));
	}
	FASSERT(false, CSTR(MSTR("wrong value for the attribute : ") + MSTR(attr->GetName())));
	return (NULL);
}

IProfileAttributeValue *UserRequestAdapter::CreateSingleProfileAttributeValue(const AttributeValueType &avtype, const char *value)
{
	IAttributeValue *val = CreateAttributeValue(avtype, value);
	IProfileAttributeValue *ret = new SingleProfileAttributeValue(*val);
	delete val;
	return (ret);
}

IProfileAttributeValue *UserRequestAdapter::CreateEnumProfileAttributeValue(const AttributeValueType &avtype, XMLNode *enumnode)
{
	int elemnbr = enumnode->GetChildrenCount("elem");
	FASSERT(elemnbr > 0, "you need to specify at least 1 element for your list");

	MList<const IAttributeValue *> attrlist;
	for (int i = 0; i < elemnbr; i++)
	{
		const char *elem = enumnode->GetChild("elem", i)->GetData();
		IAttributeValue *val = CreateAttributeValue(avtype, elem);
		attrlist.PushBack(val);
	}

	IProfileAttributeValue *ret = new EnumProfileAttributeValue(attrlist);

	unsigned int max = attrlist.Count();
	for (unsigned int i = 0; i < max; i++)
		delete attrlist.ElemAtIndex(i);

	return (ret);
}

IProfileAttributeValue *UserRequestAdapter::CreateRangeProfileAttributeValue(const AttributeValueType &avtype, XMLNode *rangenode)
{
	FASSERT(rangenode->ChildExists("low") == true, "you need to specify a low bound for your range");
	FASSERT(rangenode->ChildExists("high") == true, "you need to specify a high bound for your range");
	const char *low = rangenode->GetChild("low")->GetData();
	const char *high = rangenode->GetChild("high")->GetData();
	IAttributeValue *lval = CreateAttributeValue(avtype, low);
	IAttributeValue *hval = CreateAttributeValue(avtype, high);
	IProfileAttributeValue *ret = new RangeProfileAttributeValue(*lval, *hval);
	delete lval;
	delete hval;
	return (ret);
}

IAttributeValue *UserRequestAdapter::CreateAttributeValue(const AttributeValueType &avtype, const char *value)
{
	switch (avtype.TypeOf())
	{
	case (AttributeValueType::INTEGER) :
		{
			FASSERT(value != NULL && strlen(value) > 0, "you specified a wrong or null integer value");
			AttributeValue<int> *val = new AttributeValue<int>;// NICO PLANTAGE SI JALLOC PAS
			val->SetValue(atoi(value));
			return (val);
		}
	case (AttributeValueType::FLOAT) :
		{
			FASSERT(value != NULL && strlen(value) > 0, "you specified a wrong or null float value");
			AttributeValue<float> *val = new AttributeValue<float>;// NICO PLANTAGE SI JALLOC PAS
			val->SetValue((float) atof(value));
			return (val);
		}
	case (AttributeValueType::STRING) :
		{
			FASSERT(value != NULL && strlen(value) > 0, "you specified a wrong or null string value");
			AttributeValue<MString> *vals = new AttributeValue<MString>;
			vals->SetValue(MString(value));
			return (vals);
		}
	default:
		{
			break;
		}
	}
	FASSERT(false, CSTR(MSTR("wrong attribute type : ") + MSTR(value)));
	return (NULL);
}
