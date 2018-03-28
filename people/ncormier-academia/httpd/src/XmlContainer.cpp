/*
**
**	$HeadURL: svn://epizia.homeip.net/repo/zia/src_common/XmlContainer.cpp $
**	$Author: nico $
**	$Revision: 512 $
**	$Date: 2006-03-20 18:52:32 +0100 (Mon, 20 Mar 2006) $
**
*/

#include <map>
#include <iostream>
#include "XmlContainer.h"

using namespace std;
using namespace mozia;

namespace mozia
{
	namespace xmlconf
	{
		XmlContainer::XmlContainer()
		{
			this->_next = new map_next;
			this->_values = new map_values;

		}

		XmlContainer::~XmlContainer()
		{
			this->Clear();
		}

		XmlContainer *XmlContainer::NewChild(const char *key)
		{
			XmlContainer *child = new XmlContainer;
			(*this->_next)[ZiaString::dup(key)] = child;
			return (child);
		}

		void XmlContainer::NewValue(const char *key, const char *value)
		{
			if ((*this->_values).find(key) == (*this->_values).end())
				(*this->_values)[ZiaString::dup(key)] = new vector_value;
			(*this->_values)[key]->push_back(new string(value));
		}

		map_next::iterator XmlContainer::ChildBegin(void) const
		{
			return (this->_next->begin());
		}

		map_next::iterator XmlContainer::ChildEnd(void) const
		{
			return (this->_next->end());
		}

		map_values::iterator XmlContainer::ValueBegin(void) const
		{
			return (this->_values->begin());
		}

		map_values::iterator XmlContainer::ValueEnd(void) const
		{
			return (this->_values->end());
		}

		bool  XmlContainer::Exists(const char *key) const
		{
			if ((*this->_next).find(key) != (*this->_next).end())
				return (true);
			return (false);
		}

		const string  *XmlContainer::Get(const char *key) const
		{
			map_values::iterator iter;
			if ((iter = (*this->_values).find(key)) != (*this->_values).end())
				return (iter->second->front());
			return (NULL);
		}

		const string  *XmlContainer::Get(const char *key, unsigned int value) const
		{
			map_values::iterator iter;
			if ((iter = (*this->_values).find(key)) != (*this->_values).end())
				if (iter->second->size() > value)
					return (iter->second->at(value));
			return (NULL);
		}

		const vector_value  *XmlContainer::GetVector(const char *key) const
		{
			map_values::iterator iter;
			if ((iter = (*this->_values).find(key)) != (*this->_values).end())
				return (iter->second);
			return (NULL);
		}

		XmlContainer  &XmlContainer::operator[](const char *key) const
		{
			return (*((*this->_next)[key]));
		}

		XmlContainer  *XmlContainer::operator [](int index) const
		{
			int count = 0;
			map_next::iterator iter = (*this->_next).begin();
			while (count != index && iter != (*this->_next).end())
			{
				count++;
				iter++;
			}
			if (iter != (*this->_next).end())
				return (iter->second);
			return (NULL);
		}

		void    XmlContainer::Clear(void)
		{
			map_values::iterator iter_v = this->ValueBegin();
			while (iter_v != this->ValueEnd())
			{
				delete iter_v->first;
				vector_value::iterator iter_vect = iter_v->second->begin();
				while (iter_vect != iter_v->second->end())
				{
					delete (*iter_vect);
					iter_vect = iter_v->second->erase(iter_vect);
				}
				delete iter_v->second;
				//TOFIX
#ifdef _WIN32
				iter_v = this->_values->erase(iter_v);
#endif
			}
			delete this->_values;
			this->_values = NULL;

			map_next::iterator iter_c = this->ChildBegin();
			while (iter_c != this->ChildEnd())
			{
				iter_c->second->Clear();
				delete iter_c->first;
				//TOFIX
#ifdef _WIN32
				iter_c = this->_next->erase(iter_c);
#endif
			}

			delete this->_next;
			this->_next = NULL;
		}
	}
}
