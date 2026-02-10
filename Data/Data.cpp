#include "Data.h"

#include <assert.h>

Data Data::Null;

Data::Data() : 
	m_object(null), 
	m_tag(_Tag::Null) 
{ 
	//
}

Data::Data(_Object* obj, const _Tag& tag)
{
	m_object = obj;
	m_tag = tag;
}

Data::Data(const Data& _data)
{
	Data& data = (Data&)_data;
	m_object = data.m_object;
	m_tag = data.m_tag;
}

Data::~Data()
{
	//if (m_object)
	//{
	//	delete m_object;
	//	m_object = null;
	//}
}

Object* Data::GetObject_()
{
	return m_object;
}

void Data::SetObject_(_Object* obj)
{
	//if (m_object)
	//	delete m_object;

	m_object = obj;
}

Tag& Data::GetTag()
{
	return m_tag;
}

void Data::SetTag(const _Tag& tag)
{
	m_tag = tag;
}

Data::operator bool()
{
	return m_tag != _Tag::Null;
}

bool Data::operator == (const Data& data)
{
	return m_tag == data.m_tag;
}

bool Data::operator != (const Data& data)
{
	return m_tag != data.m_tag;
}