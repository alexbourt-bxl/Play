#include "Clipboard.h"

Clipboard::Data::Data()
{
	m_obj = null;
}

Clipboard::Data::~Data()
{
	Clear();
}

Clipboard::Data::operator bool()
{
	return m_obj != null;
}

Clipboard::Data::operator Object*()
{
	return m_obj;
}

Clipboard::Data& Clipboard::Data::operator = (Object* obj)
{
	m_obj = obj;
	return *this;
}

void Clipboard::Data::Clear()
{
	if (m_obj)
	{
		delete m_obj;
		m_obj = null;
	}
}